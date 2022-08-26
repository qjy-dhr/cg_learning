#include "Shape.h"

Shape::Shape(Type shape)
{
	type = shape;
}

Shape::~Shape()
{
}

void Shape::set_start(QPoint s)
{
	start = s;
}

void Shape::set_end(QPoint e)
{
	end = e;
}
void Shape::addList(QPoint p)
{
	start = end;
	end = p;
	pointList.push_back(p);
}
