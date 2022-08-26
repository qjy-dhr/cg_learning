#pragma once

#include "Shape.h"

class Pen : public Shape {
public:
	Pen(Type shap);
	~Pen();

	void Draw(QPainter& painter);
};
