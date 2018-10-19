#ifndef __SHAPE_H__
#define __SHAPE_H__


//#include <WinSock2.h>
//
//#include <windows.h>
#include "Rect.h"


//#include "Shape.h"

//class Rect;
//class Point;
//class b;

class Shape
{
public:
	static bool CheckPointInRect(Point point, Rect& rect);
	static bool CheckIntersect(Rect rectA, Rect rectB);
};

#endif