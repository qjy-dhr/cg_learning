#include "Ellipses.h"

Ellipses::Ellipses(Type shap) :Shape(shap)
{
	type = shap;
}


Ellipses::~Ellipses()
{
}

void Ellipses::Draw(QPainter& painter)
{
	painter.setPen(QPen(Qt::black, 3));
	painter.drawEllipse(start.x(), start.y(),
		end.x() - start.x(), end.y() - start.y());
}
