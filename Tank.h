#ifndef __TANK_H__
#define __TANK_H__

//#include <WinSock2.h>
//
//#include <windows.h>

#include "Object.h"
#include "Bomb.h"


#include <list>


using namespace std;

class Tank : public Object
{
public:
	Tank()
	{
		m_pos.Set(300, 300);

		this->CalculateSphere();

		//m_color = YELLOW;
		m_dir = UP;
		m_step = 2;

		m_bDisappear = false;
		m_bNeedShoot = false;
	}

	~Tank(){}

	//void Display()
	//{
	//	// Display
	//}
	
	void Move(int start_x, int end_x, int start_y, int end_y)
	{
		// Move
	}

	//void Boom(list<Object*>& lstBombs)
	//{
	//	lstBombs.push_back(new Bomb(m_pos, LARGE));
	//}

	// 射击
	void Shoot(list<Object*>& lstBullets)
	{
		// Shoot
	}

	void SetDisappear()
	{
		m_bDisappear = true;
	}

	bool IsDisappear()
	{
		return m_bDisappear;
	}
	
	Rect GetSphere()
	{
		return m_rectSphere;
	}

	bool NeedShoot()
	{
		return m_bNeedShoot;
	}

protected:
	void CalculateSphere()
	{
		// Calculate Sphere
	}

	bool m_bNeedShoot;
};

#endif