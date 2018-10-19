
#include "Bullet.h"

#include "Bomb.h"

Bullet::Bullet()
{
}
void Bullet::SetDir(Dir dir)
{
	m_dir = dir;
}

Dir Bullet::getDir() {
	return m_dir;
}

Bullet::Bullet(Point pos, Dir dir, COLORREF color)
{
	m_pos = pos;
	m_dir = dir;
	m_color = color;

	m_step = 6;

	m_bDisappear = false;

	CalculateSphere();
}

Bullet::~Bullet()
{

}


//// »æÍ¼
//void Bullet::Display()
//{
//	COLORREF fill_color_save = getfillcolor();
//	COLORREF color_save = getcolor();
//
//	setfillcolor(m_color);
//	setcolor(m_color);
//
//	fillcircle(m_pos.GetX(), m_pos.GetY(), 4);
//		
//	setcolor(color_save);
//	setfillcolor(fill_color_save);
//}

// ÒÆ¶¯

void Bullet::Move(int start_x, int end_x, int start_y, int end_y)
{
	switch (m_dir)
	{
	case UP:
		m_pos.SetY(m_pos.GetY() - m_step);
		CalculateSphere();
		if (m_rectSphere.GetStartPoint().GetY() < start_y)
		{
			m_pos.SetY(start_y);
			m_bDisappear = true;
		}
		break;
	case DOWN:
		m_pos.SetY(m_pos.GetY() + m_step);
		CalculateSphere();
		if (m_rectSphere.GetEndPoint().GetY() > end_y)
		{
			m_pos.SetY(end_y);
			m_bDisappear = true;
		}
		break;
	case LEFT:
		m_pos.SetX(m_pos.GetX() - m_step);
		CalculateSphere();
		if (m_rectSphere.GetStartPoint().GetX() < start_x)
		{
			m_pos.SetX(start_x);
			m_bDisappear = true;
		}
		break;
	case RIGHT:
		m_pos.SetX(m_pos.GetX() + m_step);
		CalculateSphere();
		if (m_rectSphere.GetEndPoint().GetX() >end_x)
		{
			m_pos.SetX(end_x);
			m_bDisappear = true;
		}
		break;
	default:
		break;
	}
}

//void Bullet::Boom(list<Object*>& lstBombs)
//{
//	lstBombs.push_back(new Bomb(m_pos, SMALL));
//}

void Bullet::CalculateSphere()
{
	m_rectSphere.Set(m_pos.GetX() - 2, m_pos.GetY() - 2, m_pos.GetX() + 2, m_pos.GetY() + 2);
}