#include "viewwidget.h"

ViewWidget::ViewWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	draw_status_ = false;
	shape_ = NULL;
	type_ = Shape::kDefault;
}

ViewWidget::~ViewWidget()
{
}

void ViewWidget::setLine()
{
	type_ = Shape::kLine;
}

void ViewWidget::setRect()
{
	type_ = Shape::kRect;
}

void ViewWidget::setPen()
{
	type_ = Shape::kPen;
}

void ViewWidget::backOnce()
{
	if (!draw_status_&& shape_list_.size() != 0) {
			shape_list_.pop_back();
	}
	
}

void ViewWidget::cleanAll()
{
	if(!draw_status_)
	shape_list_.clear();
}

void ViewWidget::savePic()
{
	if (!draw_status_ && shape_list_.size() != 0) {
	type_ = Shape::kDefault;
	//弹出文件保存对话框
	QString fileName = QFileDialog::getSaveFileName(this, tr("保存"), "new.jpg", "Image (*.jpg *.png *.bmp)");

	if (fileName.length() > 0)
	{
		QPixmap pixmap(size());//新建窗体大小的pixmap
		QPainter painter(&pixmap);//将pixmap作为画布
		painter.fillRect(QRect(0, 0, size().width(), size().height()), Qt::white);//设置绘画区域、画布颜色
		this->render(&painter);//将窗体渲染到painter，再由painter画到画布
		pixmap.copy(QRect(0, 30, size().width(), size().height() - 30)).save(fileName);//不包含工具栏
	}
	}	
}

void ViewWidget::setEllipse()
{
	type_ = Shape::kEllipse;
}

void ViewWidget::mousePressEvent(QMouseEvent* event)
{
	if (Qt::LeftButton == event->button())
	{
		switch (type_)
		{
		case Shape::kLine:
			shape_ = new Line(type_);
			break;
		case Shape::kDefault:
			break;
		case Shape::kRect:
			shape_ = new Rect(type_);
			break;
		case Shape::kEllipse:
			shape_ = new Ellipses(type_);
			break;
		case Shape::kPen:
			shape_ = new Pen(type_);
			break;
		}
		if (shape_ != NULL)
		{
			draw_status_ = true;
			start_point_ = end_point_ = event->pos();
			shape_->set_start(start_point_);
			shape_->set_end(end_point_);
		}
	}
	update();
}

void ViewWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (draw_status_ && shape_ != NULL)
	{
		end_point_ = event->pos();
		switch (type_) {
		case Shape::kPen:
			shape_->addList(end_point_);
			temp_list_.push_back(shape_);
		default: 
			shape_->set_end(end_point_);
		}	
	}
}

void ViewWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (shape_ != NULL)
	{
		draw_status_ = false;
		temp_list_.clear();//清空临时存放的
		switch (type_) {
		case Shape::kPen:
			shape_->addList(end_point_);
		default:
			shape_->set_end(end_point_);
		}
		shape_list_.push_back(shape_);
		shape_ = NULL;
	}
}

void ViewWidget::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.setPen(QPen(Qt::black, 3));

	for (int i = 0; i < shape_list_.size(); i++)
	{
		switch (shape_list_[i]->type) {
		case Shape::kPen: {
			QPoint start;
			QPoint end = shape_list_[i]->pointList[0];
			for (int j = 0; j < shape_list_[i]->pointList.size(); j++) {
				start = end;
				end = shape_list_[i]->pointList[j];
				painter.drawLine(start, end);
			}
			break;
		}
		default:
			shape_list_[i]->Draw(painter);
			break;
		}	
	}
	for (int i = 0; i < temp_list_.size(); i++)
	{
		if (draw_status_)
		{
			if (!temp_list_[i]->pointList.empty())
			{
				QPoint start;
				QPoint end = temp_list_[i]->pointList[0];
				for (int j = 0; j < temp_list_[i]->pointList.size(); j++) {
					start = end;
					end = temp_list_[i]->pointList[j];
					painter.drawLine(start, end);
				}
			}
		}
	}
	if (shape_ != NULL) {
		shape_->Draw(painter);
	}

	update();
}