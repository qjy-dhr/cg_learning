#include "Pen.h"

Pen::Pen(Type shap) :Shape(shap)
{
	type = shap;
}

Pen::~Pen()
{
}

void Pen::Draw(QPainter& painter)
{
	painter.setPen(QPen(Qt::black, 3));
	painter.drawLine(start, end);
}
