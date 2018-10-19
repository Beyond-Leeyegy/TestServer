#ifndef __OBJECT_H__
#define __OBJECT_H__
//#include <WinSock2.h>
//
//#include <windows.h>
#include <list>
#include "Rect.h"
#include "Point.h"
#include "def.h"


//using namespace std;

enum Dir { UP, DOWN, LEFT, RIGHT };

class Object
{
public:
	// 绘图
	//virtual void Display() = 0;

	// 移动
	virtual void Move(int start_x, int end_x, int start_y, int end_y) = 0;

	// 爆炸
	//virtual void Boom(list<Object*>& lstBombs) = 0;

	// 设置消失
	virtual void SetDisappear() = 0;

	// 判断是否消失
	virtual bool IsDisappear() = 0;

	virtual Rect GetSphere() = 0;

	Dir GetDir();

	BYTE get_id();
	void set_id(BYTE id);

	Point get_pos();

	void set_pos(int pos_x,int pos_y);
	void SetDir(Dir dir);

	COLORREF get_color();

protected:
	

	
	
	
	
	// 计算势力范围
	virtual void CalculateSphere() = 0;


	

	//add for online 
	BYTE id;

	// 位置
	Point m_pos;

	// 势力范围
	Rect m_rectSphere;

	

	// 颜色
	COLORREF m_color;//colorref实际上是一个32为整数，但是仅仅在win下支持，因此在linux端专程COLORREF//DEF.H中进行定义
	// 方向
	Dir m_dir;
	// 存在状态
	bool m_bDisappear;
	// 单次前进步长
	int m_step;

};

#endif