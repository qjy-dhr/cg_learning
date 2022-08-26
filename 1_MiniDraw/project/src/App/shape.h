#pragma once

#include <QtGui>

class Shape
{
public:
	enum Type
	{
		kDefault = 0,
		kLine = 1,
		kRect = 2,
		kEllipse = 3,
		kPen = 4,
	};
	Shape(Type type);
	virtual ~Shape();
	virtual void Draw(QPainter& paint) = 0;
	void set_start(QPoint s);
	void set_end(QPoint e);
	void addList(QPoint p);
public:
	Type type;
	std::vector<QPoint> pointList;
protected:
	QPoint start;
	QPoint end;
};

