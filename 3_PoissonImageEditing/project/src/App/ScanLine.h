#pragma once
# include <QtGui>
#include <Eigen/Dense>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <cfloat>

typedef struct Edge
{
	double x;
	double dx;
	double y_max;
}Edge;

typedef struct EDGE
{
	QPoint up;
	QPoint down;
	Edge edge;
}EDGE;

class ScanLine
{
public:

	ScanLine();

	~ScanLine();

public:
	void InitScanLine(QPainterPath path);

	QPoint get_start();

	QPoint get_end();

private:

	void GetPoints(QPainterPath path);

	void GetInsideMask();	//	Get inside mask

	void GetEDGEs();
	void InitNET();
	void InitAET();

	void SortAET();

	void MoveNewEdgeFromNET();
	void RemoveEdges();

	void FillScanLine();
	void UpdateNET();
	void FillPolygon();



public:
	Eigen::MatrixXi inside_mask_;

private:
	QVector <QPoint> points_;
	EDGE* m_pEDGEs;
	int m_PtsNum;
	int scan_index;
	QList<QVector<Edge>> NET_;	//	New edge table
	QList<QVector<Edge>> AET_;	//	active edge table
	int x_min_, x_max_, y_min_, y_max_;
	int width_, height_;
	int start_x_, start_y_, end_x_, end_y_ ;

};