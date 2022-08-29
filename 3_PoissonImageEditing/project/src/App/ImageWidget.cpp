#include "ImageWidget.h"
#include <QImage>
#include <QPainter>
#include <QtWidgets> 
#include <iostream>
#include "ChildWindow.h"

using std::cout;
using std::endl;

ImageWidget::ImageWidget(ChildWindow* relatewindow)
{
	//image_ = new QImage();
	//image_backup_ = new QImage();

	draw_status_ = kNone;
	paste_status_ = kNull;
	is_choosing_ = false;
	is_pasting_ = false;

	scanline_ = NULL;
	poisson_ = NULL;

	point_start_ = QPoint(0, 0);
	point_end_ = QPoint(0, 0);
	inside_mask_.resize(0, 0);

	source_window_ = NULL;
}

ImageWidget::~ImageWidget(void)
{
	image_.release();
	image_backup_.release();
	image_temp_.release();
	if (scanline_ != NULL) delete scanline_;
	if (poisson_ != NULL) delete poisson_;
	//circle_path = QPainterPath();

}

int ImageWidget::ImageWidth()
{
	return image_.cols;
}

int ImageWidget::ImageHeight()
{
	return image_.rows;
}

void ImageWidget::set_draw_status_to_choose()
{
	draw_status_ = kChoose;	
}

void ImageWidget::set_draw_status_to_paste()
{
	draw_status_ = kPaste;
}

void ImageWidget::set_paste_status_to_normal()
{
	paste_status_ = kNormal;
}

void ImageWidget::set_paste_status_to_poisson()
{
	paste_status_ = kPoisson;
}

void ImageWidget::set_paste_status_to_mixpoisson()
{
	paste_status_ = kMixPoisson;
}
cv::Mat& ImageWidget::image()
{
	return image_;
}

void ImageWidget::set_source_window(ChildWindow* childwindow)
{
	source_window_ = childwindow;
}

void ImageWidget::paintEvent(QPaintEvent* paintevent)
{
	QPainter painter;
	painter.begin(this);

	// Draw background
	painter.setBrush(Qt::lightGray);
	QRect back_rect(0, 0, width(), height());
	painter.drawRect(back_rect);

	// Draw image
	QImage image_show = QImage((unsigned char*)(image_.data), image_.cols, image_.rows, image_.step, QImage::Format_RGB888);
	QRect rect = QRect(0, 0, image_show.width(), image_show.height());
	painter.drawImage(rect,image_show);

	// Draw choose region
	painter.setBrush(Qt::NoBrush);
	painter.setPen(Qt::red);
	if (circle_path.elementCount() != 0) {
	circle_path.lineTo(point_end_);
	painter.drawPath(circle_path);
	}
	painter.end();
    
}

void ImageWidget::mousePressEvent(QMouseEvent* mouseevent)
{
	if (Qt::LeftButton == mouseevent->button())
	{
		switch (draw_status_)
		{
		case kChoose:
			is_choosing_ = true;
			circle_path.moveTo(mouseevent->pos());
			point_start_ = point_end_ = mouseevent->pos();
			break;

		case kPaste:
		{
			is_pasting_ = true;

			// Start point in object image
			int xpos = mouseevent->pos().rx();
			int ypos = mouseevent->pos().ry();

			// Start point in source image
			int xsourcepos = source_window_->imagewidget_->scanline_->get_start().rx();
			int ysourcepos = source_window_->imagewidget_->scanline_->get_start().ry();

			// Width and Height of rectangle region
			int w = source_window_->imagewidget_->scanline_->get_end().rx();
			-source_window_->imagewidget_->scanline_->get_start().rx() + 1;
			int h = source_window_->imagewidget_->scanline_->get_end().ry()
				- source_window_->imagewidget_->scanline_->get_start().ry() + 1;

			inside_mask_ = source_window_->imagewidget_->inside_mask_;

			// Paste
			if ((xpos + w < image_.cols) && (ypos + h < image_.rows))
			{
				// Restore image

			//	*(image_) = *(image_backup_);
				switch (paste_status_)
				{
				case kNormal:
					source_window_->imagewidget_->poisson_->Paste(mouseevent->pos(), source_window_->imagewidget_->scanline_->get_start(),
						image_, source_window_->imagewidget_->image_);
					break;
				case kPoisson:
					source_window_->imagewidget_->poisson_->PoissonPaste(mouseevent->pos(), source_window_->imagewidget_->scanline_->get_start(),
						image_, source_window_->imagewidget_->image_);
					break;
				case kMixPoisson:
					source_window_->imagewidget_->poisson_->MixingPoissonPaste(mouseevent->pos(), source_window_->imagewidget_->scanline_->get_start(),
						image_, source_window_->imagewidget_->image_);
					break;
				default:
					break;
				}
			}
		}
		update();
		break;
		default:
			break;
		}
	}
}

void ImageWidget::mouseMoveEvent(QMouseEvent* mouseevent)
{
	switch (draw_status_)
	{
	case kChoose:
		// Store point position for rectangle region
		if (is_choosing_)
		{
			point_end_ = mouseevent->pos();
		}
		break;

	case kPaste:
		// Paste rectangle region to object image
		if (is_pasting_)
		{
			// Start point in object image
			int xpos = mouseevent->pos().rx();
			int ypos = mouseevent->pos().ry();

			// Start point in source image
			int xsourcepos = source_window_->imagewidget_->scanline_->get_start().rx();
			int ysourcepos = source_window_->imagewidget_->scanline_->get_start().ry();

			// Width and Height of rectangle region
			int w = source_window_->imagewidget_->scanline_->get_end().rx()
				- source_window_->imagewidget_->scanline_->get_start().rx() + 1;
			int h = source_window_->imagewidget_->scanline_->get_end().ry()
				- source_window_->imagewidget_->scanline_->get_start().ry() + 1;

			// Paste
			if ((xpos > 0) && (ypos > 0) && (xpos + w < image_.cols) && (ypos + h < image_.rows))
			{
				// Restore image 
				//*(image_) = *(image_backup_);
				image_ = image_temp_.clone();
				// Paste
				switch (paste_status_)
				{
				case kNormal:
					source_window_->imagewidget_->poisson_->Paste(mouseevent->pos(), source_window_->imagewidget_->scanline_->get_start(),image_, source_window_->imagewidget_->image_);
					break;
				case kPoisson:
					source_window_->imagewidget_->poisson_->PoissonPaste(mouseevent->pos(), source_window_->imagewidget_->scanline_->get_start(),image_, source_window_->imagewidget_->image_);
					break;
				case kMixPoisson:
					source_window_->imagewidget_->poisson_->MixingPoissonPaste(mouseevent->pos(), source_window_->imagewidget_->scanline_->get_start(),image_, source_window_->imagewidget_->image_);
					break;
				default:
					break;

				}
			}
		}
	default:
		break;
	}

	update();
}

void ImageWidget::mouseReleaseEvent(QMouseEvent* mouseevent)
{
	switch (draw_status_)
	{
	case kChoose:
		if (is_choosing_)
		{
			point_end_ = mouseevent->pos();
			is_choosing_ = false;
			scanline_ = new ScanLine;
			poisson_ = new Poisson;
			draw_status_ = kNone;

			scanline_->InitScanLine(circle_path);
			cout << "Init ScanLine finished!" << endl;
			poisson_->InitPoisson(scanline_->inside_mask_,image_);
			cout << "Init poisson finished!" << endl;
		}

	case kPaste:
		if (is_pasting_)
		{
			is_pasting_ = false;
			draw_status_ = kNone;
			image_temp_ = image_.clone();
		}

	default:
		break;
	}

	update();
}

void ImageWidget::Open(QString filename)
{
	// Load file
	if (!filename.isEmpty())
	{
		image_ = cv::imread(filename.toStdString());
		cv::cvtColor(image_, image_, cv::COLOR_BGR2RGB);
		image_backup_ = image_.clone();
		image_temp_ = image_.clone();
		cout << "image size: " << image_.cols << ' ' << image_.rows << endl;
	}
	update();
}

void ImageWidget::Save()
{
	SaveAs();
}

void ImageWidget::SaveAs()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), ".", tr("Images(*.bmp *.png *.jpg)"));
	if (filename.isNull())
	{
		return;
	}
	//image_->save(filename);
	cv::Mat image_save;
	cv::cvtColor(image_, image_save, cv::COLOR_BGR2RGB);
	cv::imwrite(filename.toLatin1().data(), image_save);
}

void ImageWidget::Invert()
{
	if (image_.empty())
		return;
	cv::MatIterator_<cv::Vec3b> iter, iterend;
	for (iter = image_.begin<cv::Vec3b>(), iterend = image_.end<cv::Vec3b>(); iter != iterend; ++iter)
	{
		(*iter)[0] = 255 - (*iter)[0];
		(*iter)[1] = 255 - (*iter)[1];
		(*iter)[2] = 255 - (*iter)[2];
	}
	image_temp_ = image_.clone();
	update();
}

void ImageWidget::Mirror(bool ishorizontal, bool isvertical)
{
	if (image_.empty())
		return;
	int width = image_.cols;
	int height = image_.rows;

	if (ishorizontal)
	{
		if (isvertical)
		{
			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
				{
					image_.at<cv::Vec3b>(j, i) = image_backup_.at<cv::Vec3b>(height - 1 - j, width - 1 - i);
				}
			}
		}
		else
		{
			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
				{
					image_.at<cv::Vec3b>(j, i) = image_backup_.at<cv::Vec3b>(j, width - 1 - i);
				}
			}
		}

	}
	else
	{
		if (isvertical)
		{
			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
				{
					image_.at<cv::Vec3b>(j, i) = image_backup_.at<cv::Vec3b>(height - 1 - j, i);
				}
			}
		}
	}
	image_temp_ = image_.clone();
	update();
}

void ImageWidget::TurnGray()
{
	if (image_.empty())
		return;
	cv::MatIterator_<cv::Vec3b> iter, iterend;
	for (iter = image_.begin<cv::Vec3b>(), iterend = image_.end<cv::Vec3b>(); iter != iterend; ++iter)
	{
		int itmp = ((*iter)[0] + (*iter)[1] + (*iter)[2]) / 3;
		(*iter)[0] = itmp;
		(*iter)[1] = itmp;
		(*iter)[2] = itmp;
	}
	image_temp_ = image_.clone();

	update();
}

void ImageWidget::Restore()
{
	image_ = image_backup_.clone();
	image_temp_ = image_backup_.clone();
	point_start_ = point_end_ = QPoint(0, 0);
	circle_path = QPainterPath();

	update();
	
}
