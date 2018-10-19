#pragma once
/*
	FILE: message.h



*/

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "structs.h"
#include "game.h"

#define dPACKET_ALIVE_CHECK		0	//用于连接状态的确认
#define dPACKET_CHANGE_NICK		1	//绰号的修改
#define dPACKET_MAINTANK_POS	2   //客户-》服务器  用来报告自己的位置
#define dPACKET_IS_READY		3	//客户-》服务器 表示自己准备开始游戏
#define dPACKET_NEW_BULLET		4	//客户-》服务器 新的子弹创建的时候就会传输该数据包(id,x,y,dir)
#define dPACKET_ALL_POS			5	//服务器-》客户  所有元素的位置信息（each item:id,x,y,dir）(tank / bullet / )
#define dPACKET_BOOM_ACT		6   //服务器-》客户 执行爆炸的元素（坦克 & 炸弹）（id,boom_x,boom_Y）
#define dPACKET_FRIEND			7	//服务器-》客户 服务器找到了一个与之共同作战的队友，信息:对方的nickName和ip地址



//new added

/*
维持连接
*/
void PACKET_AliveCheck(sPCLIENT_DATA pClient);

/*
修改绰号
*/
void PACKET_ChangeNick(sPCLIENT_DATA pClient);

/*
聊天消息处理（客户端-》服务器）
@param: sPCLIENT_DATA
@功能：将客户端发给服务器的消息广播所有存在的连接

*/
//void PACKET_RecvMsg(sPCLIENT_DATA pClient);



/*
	处理接收数据包：IS_READY
*/

void HANDLE_IS_READY(sPCLIENT_DATA pClient);

/*
	处理接收数据包：MAINTANK_POS
*/
void HANDLE_MAINTANK_POS(sPCLIENT_DATA pClient);

/*
	处理接收数据包：NEW_BULLET
*/
void HANDLE_NEW_BULLET(sPCLIENT_DATA pClient, Game& game);

/*
数据处理：对四种数据包进行不同的处理
（连接状态确认；
修改nickname；
聊天消息（客户-》服务器）；
聊天消息（服务器-》客户））
*/
BOOL ReadRecvBuff(sPCLIENT_DATA pClient,Game& game);


#endif // !__MESSAGE_H__
