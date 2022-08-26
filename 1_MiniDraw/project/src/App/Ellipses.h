#pragma once

#include "Shape.h"

class Ellipses : public Shape {
public:
	Ellipses(Type shap);
	~Ellipses();

	void Draw(QPainter& painter);
};
