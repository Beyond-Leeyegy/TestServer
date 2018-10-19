#pragma once
/*
	FILE:packet.h
	
	数据包处理函数的头文件

*/


/*
	packet一般结构（变长）：
	packet_size(WORD)-packet_tag(WORD)-data(变长)

	对于传输的data是纯粹的聊天数据，那么data可以进一步切分
	packet_size(WORD)-packet_tag(WORD)-str_length(WORD)-str(变长)

*/


#ifndef  __PACKET_H__
#define  __PACKET_H__

#include "def.h"
/*
	在pPacket参数的nPos放置BYTE类型的value的值
*/
void  PutByte(char *pPacket, BYTE value, int & nPos);

/*
	吧pPacket参数nPos位置的值以BYTE类型返回
*/

BYTE  GetByte(char* pPacket, int& nPos);


/*
在pPacket参数的nPos放置WORD类型的value的值
*/
void PutWord(char *pPacket, WORD value, int & nPos);

/*
吧pPacket参数nPos位置的值以WORD类型返回
*/

WORD GetWord(char* pPacket, int& nPos);

/*
在pPacket参数的nPos放置DWORD类型的value的值
*/
void PutDword(char *pPacket, DWORD value, int & nPos);

/*
吧pPacket参数nPos位置的值以DWORD类型返回
*/

DWORD GetDword(char* pPacket, int& nPos);

/*
在pPacket参数的nPos放置int类型的value的值
*/
void  PutInteger(char *pPacket, int value, int & nPos);

/*
吧pPacket参数nPos位置的值以int类型返回
*/

int  GetInteger(char* pPacket, int& nPos);

/*
在pPacket参数的nPos放置short类型的value的值
*/
void PutShort(char *pPacket, short value, int & nPos);

/*
吧pPacket参数nPos位置的值以short类型返回
*/

short GetShort(char* pPacket, int &nPos);

/*
在pPacket参数的nPos放置字符串
*/
void PutString(char *pPacket, char *str , int & nPos);

/*
吧pPacket参数nPos位置的字符串复制到pBuffer（包括NULL的处理）
*/

void GetString(char* pPacket, char *pBuffer, int& nPos);

/*
	设置数据包的总长度
*/
void PutSize(char *pPacket, WORD nPos);

#endif // ! __PACKET_H__

