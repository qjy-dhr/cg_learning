#pragma once

#include "Shape.h"

class Rect : public Shape {
public:
	Rect(Type shap);
	~Rect();

	void Draw(QPainter& painter);
};

