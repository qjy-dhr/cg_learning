#pragma once

#include <ui_viewwidget.h>

#include "Shape.h"
#include "Line.h"
#include "Rect.h"
#include "Ellipses.h"
#include "Pen.h"


#include <qevent.h>
#include <qpainter.h>
#include <QWidget>
#include <QFileDialog>

#include <vector>

class ViewWidget : public QWidget
{
	Q_OBJECT

public:
	ViewWidget(QWidget* parent = 0);
	~ViewWidget();

private:
	Ui::ViewWidget ui;

private:
	bool draw_status_;//画画状态
	QPoint start_point_;//起始位置
	QPoint end_point_;//终止位置
	Shape::Type type_;//画图类型
	Shape* shape_;
	std::vector<Shape*> shape_list_;
	std::vector<Shape*> temp_list_;//存放临时的线


public:
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

public:
	void paintEvent(QPaintEvent*);
signals:
public slots:
	void setLine();
	void setRect();
	void setPen();
	void setEllipse();
	void savePic();
	void cleanAll();
	void backOnce();
};
