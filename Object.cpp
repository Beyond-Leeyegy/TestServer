#include "Object.h"
#include <stdio.h>
void Object::set_id(BYTE new_id)
{
	id = new_id;
}
BYTE Object::get_id()
{
	return id;
}

Point Object::get_pos() {
	return m_pos;
}

void Object::set_pos(int pos_x,int pos_y) {
	m_pos.SetX(pos_x);
	m_pos.SetY(pos_y);

}
void Object::SetDir(Dir dir) {
	m_dir = dir;
}

COLORREF Object::get_color() {
	return 	m_color;
}

Dir Object::GetDir() {
	return m_dir;
}