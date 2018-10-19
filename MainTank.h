#ifndef __MAIN_TANK__
#define __MAIN_TANK__
//#include <WinSock2.h>
//
//#include <windows.h>
#include "Tank.h"
#include "Object.h"


class MainTank : public Tank
{
public:
	MainTank() : Tank()
	{
		m_pos.Set(300, 300);

		this->CalculateSphere();

		//m_color = YELLOW;
		m_dir = UP;	
		m_step = 8;
	}

	~MainTank(){}
	
	MainTank& operator = (const MainTank &m){
		id = m.id;
		life = m.life;
		m_bDisappear = m.m_bDisappear;
		m_bNeedShoot = m.m_bNeedShoot;
		m_color = m.m_color;
		m_dir = m.m_dir;
		m_pos = m.m_pos;
		m_rectSphere = m.m_rectSphere;
		m_step = m.m_step;

		return *this;
}	

	void SetDir(Dir dir);
	Dir GetDir()
	{
		return m_dir;
	}

	

	//void Display();
	void Move(int start_x, int end_x, int start_y, int end_y);
	void Shoot(list<Object*>& lstBullets);
	//void Boom(list<Object*>& lstBombs);

	BYTE GetLife();
	void SetLife(BYTE newLife);

protected:
	void CalculateSphere();

	//// 绘制坦克主体
	//void DrawTankBody();

	//new added 
	BYTE life = 3;

	
};

#endif
