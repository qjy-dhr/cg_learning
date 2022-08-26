#include "Rect.h"

Rect::Rect(Type shap):Shape(shap)
{
	type = shap;
}

Rect::~Rect()
{
}

void Rect::Draw(QPainter& painter)
{
	painter.setPen(QPen(Qt::black, 3));
	painter.drawRect(start.x(), start.y(),
		end.x() - start.x(), end.y() - start.y());
}
