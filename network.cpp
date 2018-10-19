/*
	
	FILE:network.cpp
	
	用于套接字处理的源程序文件

*/

#include "def.h"
#include "structs.h"
#include "network.h"
#include "packet.h"
#include "linked_list.h"
#include "utils.h"
#include "message.h"

#include <string.h>
#include <stdio.h>

/*
	服务器主套接字
*/
extern SOCKET g_SOCK;

/*
	客户数据链表
*/
extern sPCLIENT_DATA g_ClientList;

/*
	总连接数
*/
extern int g_TotalClient;

/*
	跨平台：for win32
*/
#ifdef WIN32
void nonblock(SOCKET s) {
	u_long u10n = 1L;
	ioctlsocket(s, FIONBIO, (unsigned long *)&u10n);
}

#else


/*
	设置套接字为Noblocking
	跨平台：for linux/freeBSD
*/
void nonblock(SOCKET s) {
	int flags;

	flags = fcntl(s, F_GETFL, 0);
	flags |= O_NONBLOCK;
	if (fcntl(s, F_SETFL, flags) < 0)
		log("* nonblock Error \r\n");
}

#endif
/*
初始化服务器listen套接字
*/
SOCKET InitServerSock(int port, int backlog) {
	struct  sockaddr_in sa;
	SOCKET sock = socket(AF_INET,SOCK_STREAM,0);

	if (sock < 0) {
		log(" InitServerSock(),socket(..)failed [PORT:%d]..\r\n", port);
		return -1;
	}

#ifndef WIN32
	int opt = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0)
	{
		log("* Error: setsockopt REUSEADDR \r\n");
		exit(1);
	}
#endif // !WIN32

	//struct linger ld;
	struct linger ld;
	ld.l_onoff = 0;
	ld.l_linger = 0;
	
	if (setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&ld, sizeof(ld)) < 0) {
		log("* Error: setsockopt SO_LINGER...\r\n");
	}



	memset((char*)&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = INADDR_ANY;

	/*
		bind
	*/
	if (bind(sock, (const struct sockaddr *) &sa, sizeof(sa)) < 0) {

		log("InitServerSock(),bind(..) failed..[PORT:%d]\r\n", port);
		closesocket(sock);
		return -1;
	}
//	printf("listen:%d",sock);
	log("initServerSock:listen sock [%d]\r\n",sock);	
	nonblock(sock);
	listen(sock,backlog);
	return sock;
}

/*
	新的连接请求的处理
	step1. accept()
	step2. 将新生成的套接字设置成nonBlocking
	step3. 初始化sPCLIENT_DATA 
	step4. 将新生成的client加入到用户链表，更新连接的用户数g_TotalClient
	step5. 记载日志
*/
void AcceptNewClient(SOCKET MotherSocket) {

	struct sockaddr_in peer;   //将记录申请连接的用户的ip地址等信息
	SOCKET newDesc;

#if defined (WIN32)
	/*
	WIN32
	*/
	int peersize;
#else
	/*
	linux
	*/
	socklen_t peersize;
#endif

	peersize = sizeof(peer);

	newDesc = accept(MotherSocket,(struct sockaddr*)&peer,&peersize);
//	newDesc = accept(MotherSocket,NULL,NULL);
	/*
		Error: accpet返回值异常
	*/
	if (newDesc < 0) {
		log("Accept new client failed!\r\n");
		return;
	}
	/*
		设置nonBlocking
	*/
	nonblock(newDesc);

	/*
		分配内存给新用户
	*/
	sPCLIENT_DATA newClient = (sPCLIENT_DATA )malloc(sizeof(sCLIENT_DATA));

	/*
		Error:内存分配失败
	*/
	if (!newClient) {
		closesocket(newDesc);
		return;
	}

	/*
		初始化新用户的数据
	*/
	newClient->m_scok = newDesc;

	strcpy(newClient->m_IP, (char *)inet_ntoa(peer.sin_addr));
	
//	log("new connection accepted from %s",newClient->m_IP);

	//recv data 
	*newClient->m_recvBuff = '\0'; //可见->的运算优先与*
	newClient->m_recvPos = 0;
	newClient->m_recvSize = 0;


	*newClient->m_Name = '\0';

	//send data
	*newClient->m_sendBuff = '\0';
	newClient->m_sendSize = 0;

	// set time
	newClient->m_lastRecvTime = timeGetTime();

	// set pointer
	newClient->m_prev = NULL;
	newClient->m_next = NULL;

	MainTank mainTank;
	newClient->mainTank = mainTank;

	INSERT_TO_LIST(g_ClientList, newClient, m_prev, m_next); // ???m_next m_prev作何用？？？
	g_TotalClient++;

	/*
		记载日志
	*/
	log("*Accept New Connection: %d [%s] \r\n", newDesc, newClient->m_IP);




}

/*
断开连接
*/
void DisconnectClient(sPCLIENT_DATA pClient) {
	REMOVE_FROM_LIST(g_ClientList, pClient, m_prev, m_next);
	if (pClient->m_scok != INVALID_SOCKET)
		closesocket(pClient->m_scok);

	free(pClient);
	pClient = NULL;
}

/*
把传送的数据复制到缓冲器m_sendBuff
*/
void SendData(sPCLIENT_DATA pClient, const char *data, int size) {
	if (pClient->m_sendSize + size > dMAX_SOCK_BUFF) {
		log("overflow in client [%d] 's m_sendBuff may resulting in data omitting! \r\n", pClient->m_scok);
		return;
	}

	memcpy(&pClient->m_sendBuff[pClient->m_sendSize], data, size);
	pClient->m_sendSize += size;

}

/*
	给所有连接中的客户端发送数据(只放到相应客户的数据缓存区，等待服务器处理（发送）)
*/
void SendToAll(const char* data, int size) {
	sPCLIENT_DATA client, next_client;

	//????????
	LIST_WHILE(g_ClientList, client, next_client,m_next);
	SendData(client, data, size);
	LIST_WHILEEND(g_ClientList, client, next_client);
}

/*
	清空发送缓冲器——将某客户待发送的数据发送出去——结果可能全部处理，也有可能只处理了一部分
	使用场景：某个套接字有发送需求的时候，调用此函数
*/
int FlushSendBuff(sPCLIENT_DATA pClient) {

	int sendSize;   //记录真正发送出去的数据的长度；单位字节（服务器-》客户）
	sendSize = send(pClient->m_scok, pClient->m_sendBuff, pClient->m_sendSize, 0);


	/*
		Error: caused by send function
	*/
	if (sendSize <= 0) {
		log("Error:client[%d] call send function but failed...", pClient->m_scok);
		return -1;
	}


	/*
		if some data sent failed
	*/
	if (sendSize < pClient->m_sendSize) {
		memmove(&pClient->m_sendBuff[0], &pClient->m_sendBuff[sendSize], pClient->m_sendSize - sendSize);
		pClient->m_sendSize -= sendSize;
	}
	else
	{
		*pClient->m_sendBuff = '\0';
		pClient->m_sendSize = 0;
	}
	
	log("[SEND::%d::%s]send:%d bytes  left:%d bytes \r\n\r\n",pClient->m_scok,pClient->m_IP,sendSize,pClient->m_sendSize);	

	return sendSize;
	

}

/*
	某个套接字有read的需求的时候，调用此函数
*/
BOOL RecvFromClient(sPCLIENT_DATA pClient) {
	int recvSize;
	char recvBuff[dMAX_SOCK_BUFF];

	recvSize = recv(pClient->m_scok, recvBuff, 1024, 0);
	if (recvSize == 0){
		log("[Error reading::disconnect::%d]:%s try to read socket while no data \r\n",pClient->m_scok,pClient->m_IP);
		return 0;
}
	if (recvSize < 0) {

		/*
			出现除Nonblock以外的错误时（因为采用的是nonblocking模式，当没有数据的时候不是阻塞等待，而是立马返回错误代码，对于这种情况，不必要结束连接状态），结束连接状态
		*/
#if defined(Win32)
	/*
		WIN32
	*/
		if (WSAGetLastError() != WSAEWOULDBLOCK)
			return 0;
#else
		/*
			LINUX
		*/
		if (errno != EWOULDBLOCK){
		log("[Error handling::disconnect::%d]:%s unknown error while read socket\r\n",pClient->m_scok,pClient->m_IP);	
		return 0; } //不是因为nonblocking机制而出现的错误——真的错误
#endif
		else
		{
			return 1; //假的错误——由nonBlocking机制引发
		}
	}
	/*
		Buffer Overflow
	*/
	if ((pClient->m_recvSize + recvSize) >= dMAX_SOCK_BUFF) {
		log("[reading buffer overflow::disconnect::%d]:%s try to read while resulting in overflow\r\n",pClient->m_scok,pClient->m_IP);
		return 0;
	}

	// record time 
	pClient->m_lastRecvTime = timeGetTime();

	memcpy(&pClient->m_recvBuff[pClient->m_recvSize], recvBuff, recvSize);
	pClient->m_recvSize += recvSize;

	return 1;		// 一切正常
}
