/*
	FILE: main.cpp

	Main
*/

#include "def.h"
#include "structs.h"
#include "linked_list.h"
#include "network.h"
#include "utils.h"
#include "message.h"
#include "structs.h"
#include  "game.h"
#include <pthread.h>



struct arg {
	sPCLIENT_DATA client1;
	sPCLIENT_DATA client2;
};
 
typedef struct arg * arg_thread;

/*
	全局变量
*/
int g_ProcessLife = TRUE;

/*
	服务器的基本套接字
*/
SOCKET g_SOCK = INVALID_SOCKET;

/*
	客户数据链表
*/
sPCLIENT_DATA g_ClientList = NULL;

/*
	总连接者数
*/
int g_TotalClient = 0;

/*
	全局函数
*/
void ProcessLoop();


void* start_game_thread(void * arg) {
	arg_thread args = (arg_thread)arg;
	Game game(args->client1, args->client2);
	game.init();
	game.runGame();
}


/*
	Win32的Main处理
*/

#ifdef WIN32
#else
	/*
		LINUX
	*/
	
/*
	当发生服务器程序结束的Signal时候调用该函数
*/
void DestroySignal(int sigNum) {
//	log("sth bad happen");
//	printf("you sees see");
	g_ProcessLife = FALSE;
}

/*
	Signal机制设定
*/
void InitSignal() {

	/*
		处理SIGPIPE
		原因：SIGPIPE会强制关闭进程而不给出任何提示，这样程序员无从确定信息，为了避免这种情况，需要进行设置
		触发SIGPIPE的场景：比如尝试向已经关闭的套接字中写数据
	*/
	struct sigaction act;
	act.sa_handler = SIG_IGN;
	act.sa_flags &= ~SA_RESETHAND;
	sigaction(SIGPIPE, &act, NULL);

	/*
		当发生强制结束服务器运行的Signal时候，调用正常结束服务器运行函数的Signal的设置
	*/
	signal(SIGINT,DestroySignal);
	signal(SIGKILL,DestroySignal);
	signal(SIGQUIT, DestroySignal);
	signal(SIGTERM, DestroySignal);  //这里似乎与linux现在的定义不大相同。
}

/*
	LINUX /FressBSD的 Main处理
*/
int 
main() {
	InitSignal();
	log("IAM HERE\r\n");
	ProcessLoop();
	return TRUE;
}

#endif

/*
	服务器基本数据初始化函数
*/
void InitServerData() {

}


/*
	服务器基本数据删除函数
*/

void DestroyServerData() {
	
}



/*
	WIN32和LINUX共同的主循环
*/

void ProcessLoop() {
	sPCLIENT_DATA tmp1,tmp2;
	arg_thread arg = (arg_thread)malloc(sizeof(struct arg));

#ifdef WIN32
	//omitting
#endif

	/*
		服务器套接字初始化
	*/
	g_SOCK = InitServerSock(dSERVER_PORT, dMAX_LISTEN);
	log("init finished,got mother  sock:%d\r\n",g_SOCK);
	int fd;
	if (g_SOCK == INVALID_SOCKET)
	{
		log("INVALID SOCKET\r\n");
		printf("INVALID SOCKET\n\n\n\n");
		return;
	}
	
	log("VALID DATA\r\n");
//	listen(g_SOCK,1);
//	
//	while(1){
//	fd = accept(g_SOCK,NULL,NULL);
//	close(fd);
//}

	/*
	
		服务器数据初始化
	*/

	InitServerData();

	fd_set read_set;
	fd_set write_set;
	fd_set exc_set;
	struct timeval tv;
	SOCKET nfds;



	/*
		设置未检测到信号就立即跳转到下一个套接字的信号的检测
	*/
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	sPCLIENT_DATA client, next_client;

	while ((g_ProcessLife))
	{
#ifdef WIN32
		//OMITTING
#endif

			/*
				fd_set初始化
			*/
			FD_ZERO(&read_set);
			FD_ZERO(&write_set);
			FD_ZERO(&exc_set);

			/*
				将主套接字加到read_set来监听新的连接请求
			*/
			FD_SET(g_SOCK,&read_set);
			nfds = g_SOCK;


			/*
				求套接字的最大值
			*/
			LIST_WHILE(g_ClientList,client,next_client,m_next);
			if (client->m_scok > nfds)
				nfds = client->m_scok;

			/*
				需要监听的套接字与fd_Set之间的绑定
				只监听不在游戏状态中的用户
			*/
			if (!client->in_Game) {
				FD_SET(client->m_scok, &read_set);
				FD_SET(client->m_scok, &write_set);
				FD_SET(client->m_scok, &exc_set);
			}

			LIST_WHILEEND(g_ClientList, client, next_client);
			
			/*
				call select
			*/
				if (select(nfds + 1, &read_set, &write_set, &exc_set, &tv) < -1) {
					log("select Error!\r\n");
					continue;
			}
			
			/*
				new added here
			*/
			//log("found the max port %d\r\n",nfds);

			


			if (FD_ISSET(g_SOCK, &read_set)){
				log("deteced new intention to connect to server\r\n");
				AcceptNewClient(g_SOCK);
			}

			/*
				例外错误处理和数据recv（接收）
			*/
			LIST_WHILE(g_ClientList, client, next_client, m_next);

			/*
				只处理不在游戏状态的用户
			*/
			if (client->in_Game)
				continue;
			/*
				error handling
			*/
			if (FD_ISSET(client->m_scok, &exc_set)) {
				DisconnectClient(client);
				log("[Error Handling::disconnect::%d]:%s\r\n",client->m_scok,client->m_IP);
				LIST_SKIP(client, next_client);
			}

			/*
				超过1分钟没有反应，断开连接

			*/
			if (client->m_lastRecvTime + 60000 <= timeGetTime()) {
				DisconnectClient(client);
				log("[disconnect::%d]:%s over 1 min no response\r\n",client->m_scok,client->m_IP);
				LIST_SKIP(client, next_client);
			}

			/*
				尝试接收数据（客户端-》服务器）
				如果有套接字试图向服务器发送数据，那么就尝试接收这些数据,存储到缓存区中
			*/
			if (FD_ISSET(client->m_scok, &read_set)) {

				if (!RecvFromClient(client)) {
					DisconnectClient(client);
					LIST_SKIP(client, next_client);
				}
			}

			/*
				服务器处理某客户发送过来待处理的数据
				如果客户-》服务器缓存器存在数据，那么就处理这些数据
			*/

			/*
			
				@readrecvBuff:message.h 
				here just handle is_ready
			*/
			Game tmp;
			if (client->m_recvSize) { 
				if (!ReadRecvBuff(client,tmp))
				{
					DisconnectClient(client);
					LIST_SKIP(client, next_client);
				}


			}

			LIST_WHILEEND(g_ClientList, client, next_client);

			/*
				尝试发送数据（服务器-》客户端）
				如果服务器套接字试图向客户端写数据的时候并且 服务器-》客户缓存器存在数据，那么就尝试发送这些数据
			*/
			LIST_WHILE(g_ClientList, client, next_client, m_next);
			/*
			只处理不在游戏状态的用户
			*/
			if (client->in_Game)
				continue;

			if (client->m_sendSize && FD_ISSET(client->m_scok, &write_set)) {
				//???其实不清楚为什么这里的isset会置write_set，因为如果服务器向发些啥数据，先调用sendData写到缓冲区（自定义的），然后才调用flushSendBuff发出去
				if (FlushSendBuff(client) < 0) {
					DisconnectClient(client);
					LIST_SKIP(client, next_client);
				}
			}

			LIST_WHILEEND(g_ClientList, client, next_client);


			/*
				!in_game && is_ready
			*/
			int count = 0;

			LIST_WHILE(g_ClientList, client, next_client, m_next);
			if (!client->in_Game && client->is_Ready) {
			//	count = (count + 1) % 2;
				if (count == 0) {
					tmp1 = client;
				}
				else if (count == 1) {
					tmp2 = client;
					
					//log("[debug::%d::%d]\r\n\r\n\r\n",tmp1->m_scok,tmp2->m_scok);		

					arg->client1 = tmp1;
					arg->client2 = tmp2;
					
					arg->client1->in_Game = 1;
					arg->client2->in_Game = 1;					
	
					pthread_t t1;
					pthread_create(&t1, NULL, start_game_thread, arg);
				//	//start_game_thread;
				}
				count = (count + 1 )%2;
			}
			LIST_WHILEEND(g_ClientList, client, next_client);

}

	/*
		服务器数据的删除
	
	*/
	DestroyServerData();

#ifdef WIN32
//omitting data here

#endif




}
