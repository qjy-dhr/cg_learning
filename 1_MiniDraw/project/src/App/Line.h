#pragma once

#include "Shape.h"

class Line : public Shape {
public:
	Line(Type shape);
	~Line();

	void Draw(QPainter& painter);
};
