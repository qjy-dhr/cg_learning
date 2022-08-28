#include "ImageWidget.h"

#include"ANN/ANN.h"

using namespace std;

ImageWidget::ImageWidget(void)
{
	ptr_image_ = new QImage();
	ptr_image_backup_ = new QImage();
	ptr_image_temp_ = new QImage();
	choose_ = false;
	current_ = -1;
}


ImageWidget::~ImageWidget(void)
{
	for (int i = 0; i < ptr_image_->height(); i++)
		delete change_[i];
	delete change_;
	delete ptr_image_;
	delete ptr_image_backup_;
	delete ptr_image_temp_;
}
//监听鼠标事件，提供输入数据点对(即直线)功能
//直线的起始点->p,直线的终止点->q
void ImageWidget::mousePressEvent(QMouseEvent* event)
{
	if (choose_ && Qt::LeftButton == event->button())
	{
		current_++;
		//设置定点	
		points_p.push_back(event->pos() - *points_relative);
		points_q.push_back(event->pos() - *points_relative);
	}
}

void ImageWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (choose_)
	{
		points_q[current_] = event->pos()- *points_relative;
	}
}

void ImageWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (!choose_)
		return;
	if (points_p[current_].x() < 0 || points_p[current_].y() < 0 ||
		points_p[current_].x() > ptr_image_->width() || points_p[current_].y() > ptr_image_->height() ||
		points_q[current_].x() < 0 || points_q[current_].y() < 0 ||
		points_q[current_].x() > ptr_image_->width() || points_q[current_].y() > ptr_image_->height())
	{
		points_p.pop_back();
		points_q.pop_back();
		current_--;
	}

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
	QRect rect = QRect((width() - ptr_image_->width()) / 2, (height() - ptr_image_->height()) / 2, ptr_image_->width(), ptr_image_->height());

	painter.drawImage(rect, *ptr_image_);

	if (choose_)
	{
		QPen pen;
		pen.setColor("red");
		pen.setWidth(8);
		painter.setPen(pen);

		for (size_t i = 0; i < points_p.size(); i++)
		{
			painter.drawLine(points_p[i]+ *points_relative, points_q[i]+ *points_relative);
		}
	}

	painter.end();
	update();
}

void ImageWidget::Open()
{
	// Open file
	QString fileName = QFileDialog::getOpenFileName(this, tr("Read Image"), ".", tr("Images(*.bmp *.png *.jpg)"));

	// Load file
	if (!fileName.isEmpty())
	{
		ptr_image_->load(fileName);
		*(ptr_image_backup_) = *(ptr_image_);
	}
	change_ = new bool* [ptr_image_->height()];
	for (int i = 0; i < ptr_image_->height(); i++)
		change_[i] = new bool[ptr_image_->width()];

	//ptr_image_->invertPixels(QImage::InvertRgb);
	//*(ptr_image_) = ptr_image_->mirrored(true, true);
	//*(ptr_image_) = ptr_image_->rgbSwapped();
	points_relative = new QPoint((width() - ptr_image_->width()) / 2, (height() - ptr_image_->height()) / 2);
	cout << "image size: " << ptr_image_->width() << ' ' << ptr_image_->height() << endl;
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

	ptr_image_->save(filename);
}

void ImageWidget::Invert()
{
	for (int i = 0; i < ptr_image_->width(); i++)
	{
		for (int j = 0; j < ptr_image_->height(); j++)
		{
			QRgb color = ptr_image_->pixel(i, j);
			ptr_image_->setPixel(i, j, qRgb(255 - qRed(color), 255 - qGreen(color), 255 - qBlue(color)));
		}
	}

	// equivalent member function of class QImage
	// ptr_image_->invertPixels(QImage::InvertRgb);
	update();
}

void ImageWidget::Mirror(bool ishorizontal, bool isvertical)
{
	QImage image_tmp(*(ptr_image_));
	int width = ptr_image_->width();
	int height = ptr_image_->height();

	if (ishorizontal)
	{
		if (isvertical)
		{
			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
				{
					ptr_image_->setPixel(i, j, image_tmp.pixel(width - 1 - i, height - 1 - j));
				}
			}
		}
		else			//仅水平翻转			
		{
			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
				{
					ptr_image_->setPixel(i, j, image_tmp.pixel(width - 1 - i, j));
				}
			}
		}

	}
	else
	{
		if (isvertical)		//仅垂直翻转
		{
			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
				{
					ptr_image_->setPixel(i, j, image_tmp.pixel(i, height - 1 - j));
				}
			}
		}
	}
	update();
}

void ImageWidget::TurnGray()
{
	for (int i = 0; i < ptr_image_->width(); i++)
	{
		for (int j = 0; j < ptr_image_->height(); j++)
		{
			QRgb color = ptr_image_->pixel(i, j);
			int gray_value = (qRed(color) + qGreen(color) + qBlue(color)) / 3;
			ptr_image_->setPixel(i, j, qRgb(gray_value, gray_value, gray_value));
		}
	}
	update();
}

void ImageWidget::Restore()
{
	choose_ = false;
	*(ptr_image_) = *(ptr_image_backup_);
	points_p.clear();
	points_q.clear();
	update();
}

void ImageWidget::Warp()
{
	choose_ = true;
	points_p.clear();
	points_q.clear();
	current_ = -1;
}

//RBF变换
void ImageWidget::RBFWarping()
{
	choose_ = false;
	if (points_p.empty())
	{
		return;
	}
	*ptr_image_temp_ = *ptr_image_;
	int w = ptr_image_->width();
	int h= ptr_image_->height();
	for (int i = 1; i < h - 1; i++)
		for (int j = 1; j < w - 1; j++)
			change_[i][j] = false;

	RBF_Warping rbf = RBF_Warping(points_p, points_q);
	QPoint t1, t2;
	ptr_image_->fill(Qt::white);                   //打底色

	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
		{
			t1 = QPoint(i, j);
			t2 = rbf.pToq(t1);
			if (t2.x() > 0 && t2.y() > 0 && t2.x() < w && t2.y() < h)
				change_[t2.y()][t2.x()] = true;
			ptr_image_->setPixel(t2, ptr_image_temp_->pixel(t1));
		}
	update();
}

//IDW变换
void ImageWidget::IDWWarping()
{
	choose_ = false;
	if (points_p.empty())
		return;
	*(ptr_image_temp_) = *(ptr_image_);
	int w = ptr_image_->width();
	int h = ptr_image_->height();
	for (int i = 1; i < h - 1; i++)
		for (int j = 1; j < w - 1; j++)
			change_[i][j] = false;
	IDW_Warping idw = IDW_Warping(points_p, points_q);
	QPoint t1, t2;
	ptr_image_->fill(Qt::white);
	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
		{
			t1 = QPoint(i, j);
			//bool参数设置求变换矩阵的方式
			t2 = idw.pToq(t1, false);
			if (t2.x() > 0 && t2.y() > 0 && t2.x() < w && t2.y() < h)
				change_[t2.y()][t2.x()] = true;
			ptr_image_->setPixel(t2, ptr_image_temp_->pixel(t1));
		}
	update();
}

void ImageWidget::AnnFillHole() {
	int w = ptr_image_->width();
	int h = ptr_image_->height();
	ANNpointArray ptsArr = annAllocPts(w * h, 2);
	int temp = 0;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (change_[i][j])
			{
				ptsArr[temp][0] = j;
				ptsArr[temp][1] = i;
				temp++;
			}
		}
	}
	constexpr size_t K = 4;                        
	ANNkd_tree* tree = new ANNkd_tree(ptsArr, temp, 2);//构建ANN树

	QColor color;
	for (int i = 0; i < w; i++)
	{
	for (int j = 0; j < h; j++)
		{
		ANNpoint queryPt = annAllocPt(2);
		ANNidxArray idxArr = new ANNidx[K];
		ANNdistArray distArr = new ANNdist[K];
		int r = 0, g = 0, b = 0;
		queryPt[0] = i;
		queryPt[1] = j;
		tree->annkSearch(queryPt, K, idxArr, distArr);
		delete[] distArr;
		int choose = K;
		for (int l = 0; l < K; l++)
		{
			int x = ptsArr[idxArr[l]][0];
			int y = ptsArr[idxArr[l]][1];
			color = ptr_image_->pixelColor(x,y);
			if (fabs(x - i) <= 2 && fabs(y - j) <= 2)   //设置阈值
			{
				r += color.red();
				g += color.green();
				b += color.blue();
			}
			else
			{
				choose--;
					}
				}
		if (choose > 0)
		{
			ptr_image_->setPixelColor(i,j, QColor(r / choose, g / choose, b / choose));
		}
		delete[] idxArr;
		}
	}
	annDeallocPts(ptsArr);
	delete tree;
}

