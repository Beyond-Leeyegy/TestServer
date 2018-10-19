#pragma once
#ifndef __GAME_H__
#define __GAME_H__

#include "structs.h"

class Game
{
public:
	/*
		定义游戏边界
	*/

	int width=1000;
	int height = 800;
	int start_x = 0;
	int start_y = 0;
	int end_x = 100;
	int end_y = 100;

	Game();
	Game(sPCLIENT_DATA Client1, sPCLIENT_DATA Client2);
	~Game();

	//两个参与游戏的用户
	sPCLIENT_DATA pClient1;
	sPCLIENT_DATA pClient2;

	//我方子弹集合
	list<Object*> lstMainTankBullets;

	//电脑坦克共享一个子弹集合
	list<Object*> lstBullets;

	// Bomb List
	list<Object*> lstBombs;

	// Tank list
	list<Tank*> lstTanks;

	/*
		发送all_pos数据包
		
		ALL_POS数据包结构：size: word->tag :word->主战坦克1数据{id:byte,x:int,y:int.dir:int,坦克生命值:byte}->友军主战坦克数据{数据同上}->子弹类数据{子弹个数:byte,loop{id:byte,x:int,y:int,dir:int,type:byte0代表我军集合，1代表敌军集合}}->
		电脑坦克数据{坦克数,loop{同上}}
		
	*/
	void onSend_ALL_POS();

	/*
		发送BOOM_ACT数据包

		//boom_act数据包结构：size:word->tag:word->id:byte->id:byte

	*/
	void onSend_BOOM_ACT(BYTE id_1, BYTE id_2);

	/*
		发送 FRIEND数据包

		FRIEND数据包结构：size:word->tag:word->ip:string->nickName:string

	*/
	void onSend_FRIEND();

	/*
		初始化，设置两个client都是inGame+发送一次OnSendFriend包
	*/
	void init();

	void Game_Logic();

	void Handling_Info();


	/*
		游戏主体运行逻辑
		while(!GameOver){
			game logic(including move ; may sending BOOM_ACT);
			sending all_pos;
			call select and handling infomation;
		}
	*/
	void runGame();

	/*
		检查碰撞
	*/
	void CheckCrash();

	/*
		销毁所有的列表对象
	*/
	void Dispose();



};


#endif
