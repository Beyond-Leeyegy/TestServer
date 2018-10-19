#pragma once
/*
	FILE:network.h

	套接字处理函数的头文件

*/
#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "def.h"
#include "structs.h"

/*
	设置套接字为Noblocking
*/
void nonblock(SOCKET s);

/*
	初始化服务器listen套接字
*/
SOCKET InitServerSock(int port, int backlog);

/*
	新的连接请求的处理
*/
void AcceptNewClient(SOCKET MotherSocket);

/*
	断开连接
*/
void DisconnectClient(sPCLIENT_DATA pClient);

/*
	把传送的数据复制到缓冲器
*/
void SendData(sPCLIENT_DATA pClient, const char *data, int size);

/*
	给所有连接中的客户端发送数据
*/
void SendToAll(const char* data, int size);

/*
	清空发送缓冲器
*/
int FlushSendBuff(sPCLIENT_DATA pClient);

/*
	recv
*/
BOOL RecvFromClient(sPCLIENT_DATA pClient);

#endif // !__NETWORK_H__

