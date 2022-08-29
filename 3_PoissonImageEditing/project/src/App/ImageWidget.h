﻿#pragma once
#include <QWidget>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <Poisson.h>
#include <QPainterPath>


class ChildWindow;
QT_BEGIN_NAMESPACE
class QImage;
class QPainter;
QT_END_NAMESPACE

enum DrawStatus
{
	kChoose, 
	kPaste, 
	kNone
};
enum PasteStatus
{
	kNormal,            //正常的粘贴
	kPoisson,           //Poisson粘贴
	kMixPoisson,         //MixPoisson粘贴
	kNull
};

class ImageWidget :
	public QWidget
{
	Q_OBJECT

public:
	ImageWidget(ChildWindow *relatewindow);
	~ImageWidget(void);

	int ImageWidth();											// Width of image
	int ImageHeight();											// Height of image
	void set_draw_status_to_choose();
	void set_draw_status_to_paste();
	void set_paste_status_to_normal();                          //设置粘贴的状态
	void set_paste_status_to_poisson();
	void set_paste_status_to_mixpoisson();

	//QImage* image();
	cv::Mat& image();
	void set_source_window(ChildWindow* childwindow);

protected:
	void paintEvent(QPaintEvent *paintevent);
	void mousePressEvent(QMouseEvent *mouseevent);
	void mouseMoveEvent(QMouseEvent *mouseevent);
	void mouseReleaseEvent(QMouseEvent *mouseevent);

public slots:
	// File IO
	void Open(QString filename);								// Open an image file, support ".bmp, .png, .jpg" format
	void Save();												// Save image to current file
	void SaveAs();												// Save image to another file

	// Image processing
	void Invert();												// Invert pixel value in image
	void Mirror(bool horizontal=false, bool vertical=true);		// Mirror image vertically or horizontally
	void TurnGray();											// Turn image to gray-scale map
	void Restore();												// Restore image to origin

public:
	QPoint						point_start_;					// Left top point of rectangle region
	QPoint						point_end_;						// Right bottom point of rectangle region
	QPainterPath                circle_path;

private:

	cv::Mat					image_;
	cv::Mat					image_backup_;
	cv::Mat					image_temp_;

	// Pointer of child window
	ChildWindow					*source_window_;				// Source child window

	Poisson* poisson_;

	Eigen::MatrixXi inside_mask_;

	ScanLine* scanline_;

	// Signs
	DrawStatus					draw_status_;					// Enum type of draw status
	PasteStatus                 paste_status_;                  //选择粘贴的状态
	bool						is_choosing_;
	bool						is_pasting_;
};

