/*
	FILE: message.cpp

	处理接收的数据包的函数

*/
#include "def.h"
#include "structs.h"
#include "network.h"
#include "packet.h"
#include "linked_list.h"
#include "utils.h"
#include "message.h"
#include "game.h"
#include "Bullet.h"
#include <list>
#include <string.h>
#include <stdio.h>

/*
	维持连接
*/
void PACKET_AliveCheck(sPCLIENT_DATA pClient) {

}

/*
	修改绰号
*/
void PACKET_ChangeNick(sPCLIENT_DATA pClient) {
	char nick[64];
	GetString(pClient->m_recvBuff, nick, pClient->m_recvPos);
	strcpy(pClient->m_Name, nick);


}

/*
	聊天消息处理（客户端-》服务器）
	@param: sPCLIENT_DATA
	@功能：将客户端发给服务器的消息广播所有存在的连接

*/
//void PACKET_RecvMsg(sPCLIENT_DATA pClient) {
//	char msg[dMAX_SOCK_BUFF];
//	int packetPos;					
//	char packet[dMAX_SOCK_BUFF];	//packet to be sent
//	char tmp[dMAX_SOCK_BUFF];       // content to be sent
//
//
//	//step1. get msg
//	GetString(pClient->m_recvBuff, msg, pClient->m_recvPos);
//
//	//step2. complete content to be sent
//	sprintf(tmp,"%s : %s\r\n",pClient->m_Name,msg);
//
//	//step3.complete packet to be sent
//	packetPos = 2;
//	PutWord(packet, dPACKET_SEND_MSG, packetPos); // tag
//	PutString(packet, tmp, packetPos);			  // data
//	PutSize(packet, packetPos);					  // size
//
//	//step4. send packet to all
//	SendToAll(packet, packetPos);
//}

/*
	以下处理接收数据包：仅仅根据数据包对对象的状态进行改变
*/

/*
	处理接收数据包：IS_READY
*/
void HANDLE_IS_READY(sPCLIENT_DATA pClient) {
	//将该客户的状态置成1，表示已经准备开始游戏
	pClient->is_Ready = 1;

}

/*
处理接收数据包：MAINTANK_POS

数据包结构：size->tag->{Maintank.id:byte,x int ,y int,dir int}->
*/

void HANDLE_MAINTANK_POS(sPCLIENT_DATA pClient) {
	//接收到了client的主坦克的信息
	BYTE id;
	int pos_x, pos_y;
	int dir;

	int pos = 4;

	//getDATA
	id = GetByte(pClient->m_recvBuff, pos);
	pos_x = GetInteger(pClient->m_recvBuff, pos);
	pos_y = GetInteger(pClient->m_recvBuff, pos);

	dir = GetInteger(pClient->m_recvBuff, pos);

	//change state
	pClient->mainTank.SetDir((Dir)dir);
	pClient->mainTank.set_id(id);
	pClient->mainTank.set_pos(pos_x, pos_y);
	
	return;

}

/*
处理接收数据包：NEW_BULLET
*/
/*
数据包结构：size->tag->{Bullet.id:byte , x int ,y int,dir int}->
*/
void HANDLE_NEW_BULLET(sPCLIENT_DATA pClient,Game& game) {
	//get data 
	//接收到了client的主坦克的信息
	BYTE id;
	int pos_x, pos_y;
	int dir;

	int pos = 4;
	id = GetByte(pClient->m_recvBuff, pos);
	pos_x = GetInteger(pClient->m_recvBuff, pos);
	pos_y = GetInteger(pClient->m_recvBuff, pos);

	dir = GetInteger(pClient->m_recvBuff, pos);


	//new bullet
	Point point;
	point.Set(pos_x, pos_y);
	Bullet* pBullet = new Bullet(point, (Dir)dir, pClient->mainTank.get_color());
	pBullet->SetStep(20);
	pBullet->set_id(id);
	game.lstMainTankBullets.push_back(pBullet);

}


/*
	数据处理：对四种数据包进行不同的处理
	（连接状态确认；
	  修改nickname；
	  聊天消息（客户-》服务器）；
	  聊天消息（服务器-》客户））

	  @功能：处理某用户所有等待被处理的数据包
	  NOTICE:每处理一个数据包，用户的m_recvBuff就会删除已经处理的数据包；同时更新m_recvSize
*/
BOOL ReadRecvBuff(sPCLIENT_DATA pClient,Game& game) {
	WORD msgSize;
	WORD tag;
	
	log("[READ:%d]:%s\r\n\r\n",pClient->m_scok,pClient->m_IP);


	while (pClient->m_recvSize > 0) {
		pClient->m_recvPos = 0;

		//step1. get packet_size and tag  
		msgSize = GetWord(pClient->m_recvBuff, pClient->m_recvPos);
		tag = GetWord(pClient->m_recvBuff, pClient->m_recvPos);

		/*
			当目前m_recvBuff内数据的长度小于数据包的总长度时
		*/
		if (pClient->m_recvSize < msgSize) {
			return 1;							//???
		}

		/*
			数据包处理
		*/
		switch (tag)
		{
		case dPACKET_ALIVE_CHECK:
			PACKET_AliveCheck(pClient);
	
			break;
		case dPACKET_CHANGE_NICK:
			PACKET_ChangeNick(pClient);
			break;
		case dPACKET_IS_READY: 
			HANDLE_IS_READY(pClient);
			log("[HANDLED_IS_READY:%d]:%s\r\n\r\n",pClient->m_scok,pClient->m_IP);
			break;
		case dPACKET_NEW_BULLET:
			HANDLE_NEW_BULLET(pClient,game);
			log("[HADNLED_NEW_BULLET:%d],%s\r\n\r\n",pClient->m_scok,pClient->m_IP);
			break;
		case dPACKET_MAINTANK_POS: 
			HANDLE_MAINTANK_POS(pClient);
			log("[HANDLED_MAINTANK_POS:%d]:%s\r\n\r\n",pClient->m_scok,pClient->m_IP);
			break;
		}


		/*
			表示已经处理过了，因此空间进行迁移
		*/
		memmove(&pClient->m_recvBuff[0],
			&pClient->m_recvBuff[msgSize],
			pClient->m_recvSize - msgSize);
		pClient->m_recvSize -= msgSize;
		log("[HANDLE_SIZE::%d],%dleft\r\n\r\n",msgSize,pClient->m_recvSize);
	}
	return 1;
}
