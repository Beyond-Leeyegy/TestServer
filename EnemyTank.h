#ifndef __ENEMY_TANK__
#define __ENEMY_TANK__
//#include <WinSock2.h>
//
//#include <windows.h>
#include "Tank.h"

#define MAX_STEP_TURN	20
#define MAX_STEP_SHOOT	15

///*
//	new added 
//*/
//enum Dir { UP, DOWN, LEFT, RIGHT };

class EnemyTank : public Tank
{
public:
	/*
		enemytank的构造函数实际上委托给randomTank进行随机初始化坦克
	*/
	EnemyTank(int width, int height)
	{
		RandomTank(width, height);
	}

	~EnemyTank(){}

	
	void Move(int start_x, int end_x, int start_y, int end_y);
	void Shoot(list<Object*>& lstBullets);

	void set_stepCnt(int stepCnt);


protected:
	void CalculateSphere();
	void RandomTank(int width, int height);
	// 随机产生坦克方向 type： 1, 新方向必须与之前方向不同 2, 任意一个新方向
	void RandomDir(int type);

	int m_stepCnt;//大概走几步会改变方向
};

#endif