#pragma once
#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include "def.h"
#include "MainTank.h"

#define dMAX_SOCK_BUFF 1024	// buffer length
#define dNAME_LEN 50		// name length 
#define dIP_LEN 20			// IP length

struct client_data
{
	/*
		用于数据传送的套接字等各种数据变量
	*/

	// network communication
	int m_scok;			// 网络连接套接字
	char m_IP[dIP_LEN];     // IP  address
	
	// recv data from client
	char m_recvBuff[dMAX_SOCK_BUFF];	//A packet
	int  m_recvSize;	//总的数据包集合的长度（一个客户可能有多个数据包等待被处理）
	int  m_recvPos;     //相应数据的地址偏移（相对于m_recvBuff首地址）

	// send data to client
	char m_sendBuff[dMAX_SOCK_BUFF];	//A packet
	int  m_sendSize;

	/*
		new added 
	*/
	int in_Game = 0; //代表该用户是否正在进行游戏

	/*
		new added
	*/
	MainTank mainTank;

	/*
		new added here
	*/
	int is_Ready = 0;//代表该连接的客户是否准备进入游戏


	
	/*
		 name
	 */
	char m_Name[dNAME_LEN];	
	
	
	
	/*
		用于连接状态确认的最后recv时间值
	*/
	unsigned long m_lastRecvTime;



	/*
		用于链表连接的指针
	*/
	struct client_data *m_prev;
	struct client_data *m_next;

};

/*
	结构体重命名
*/

typedef struct client_data sCLIENT_DATA, *sPCLIENT_DATA;


#endif 
