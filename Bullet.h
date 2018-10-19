#ifndef __BULLET_H__
#define __BULLET_H__

//#include <WinSock2.h>
//
//#include <windows.h>
#include "Object.h"

class Bullet : public Object
{
public:
	Bullet();
	Bullet(Point pos, Dir dir, COLORREF color);
	~Bullet();
	void SetDir(Dir dir);
	//void Display();

	void Move(int start_x, int end_x, int start_y, int end_y);

	Dir getDir();

	/*void Boom(list<Object*>& lstBombs);
*/
	bool IsDisappear()
	{
		return m_bDisappear;
	}

	Rect GetSphere()
	{
		/*
			new added here
		*/
		CalculateSphere();
		return m_rectSphere;
	}

	void SetStep(int nStep)
	{
		m_step = nStep;
	}

	void SetDisappear()
	{
		m_bDisappear = true;
	}

protected:
	void CalculateSphere();
};

#endif