#include "Line.h"

Line::Line(Type shap):Shape(shap)
{
	type = shap;
}
Line::~Line()
{
}

void Line::Draw(QPainter& painter)
{
	painter.drawLine(start, end);
}
