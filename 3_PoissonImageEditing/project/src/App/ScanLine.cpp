#include "ScanLine.h"

ScanLine::ScanLine()
{
	m_pEDGEs = NULL;
}

ScanLine::~ScanLine()
{

}

QPoint ScanLine::get_start()
{
	return QPoint(start_x_, start_y_);
}

QPoint ScanLine::get_end()
{
	return QPoint(end_x_, end_y_);
}

void ScanLine::InitScanLine(QPainterPath path) {
	GetPoints(path);
	GetInsideMask();
}

//初始化边点集
void ScanLine::GetPoints(QPainterPath path)
{
	points_ = path.toFillPolygon().toPolygon();
	x_min_ = y_min_ = 100000;
	x_max_ = y_max_ = -1;
	for (int i = 0; i < points_.size(); i++)
	{
		if (points_[i].x() < x_min_)
			x_min_ = points_[i].x();
		if (points_[i].x() > x_max_)
			x_max_ = points_[i].x();
		if (points_[i].y() < y_min_)
			y_min_ = points_[i].y();
		if (points_[i].y() > y_max_)
			y_max_ = points_[i].y();
	}
	width_ = x_max_ - x_min_ + 1;
	height_ = y_max_ - y_min_ + 1;
	start_x_ = x_min_;
	start_y_ = y_min_;
	end_x_ = x_max_;
	end_y_ = y_max_;
	inside_mask_.resize(height_, width_);
	inside_mask_.setZero();
	m_PtsNum = points_.size();
}

//得到预分解矩阵
void ScanLine::GetInsideMask()
{
	inside_mask_.setZero();
	GetEDGEs();
	InitNET();
	InitAET();
	FillPolygon();
}

// 获得
void ScanLine::GetEDGEs()
{
	if (m_pEDGEs)
	{
		delete[] m_pEDGEs; m_pEDGEs = NULL;
	}
	m_pEDGEs = new EDGE[m_PtsNum];

	for (int i = 0; i < m_PtsNum - 1; i++)
	{
		if (points_[i].y() > points_[i + 1].y())
		{
			m_pEDGEs[i].up = points_[i];
			m_pEDGEs[i].down = points_[i + 1];
		}
		else
		{
			m_pEDGEs[i].up = points_[i+1];
			m_pEDGEs[i].down = points_[i];
		}
		m_pEDGEs[i].edge.y_max = m_pEDGEs[i].up.y();
		m_pEDGEs[i].edge.x = m_pEDGEs[i].down.x();
		if (m_pEDGEs[i].up.y() == m_pEDGEs[i].down.y())
			m_pEDGEs[i].edge.dx = DBL_MAX;
		else
			m_pEDGEs[i].edge.dx = ((double)m_pEDGEs[i].up.x()-(double)m_pEDGEs[i].down.x())/((double)m_pEDGEs[i].up.y()-(double)m_pEDGEs[i].down.y());
	}

	if (points_[0].y() > points_[m_PtsNum - 1].y())
	{
		m_pEDGEs[m_PtsNum - 1].up = points_[0];
		m_pEDGEs[m_PtsNum - 1].down = points_[m_PtsNum - 1];
	}
	else
	{
		m_pEDGEs[m_PtsNum - 1].up = points_[m_PtsNum - 1]; 
		m_pEDGEs[m_PtsNum - 1].down = points_[0];
	}

	m_pEDGEs[m_PtsNum - 1].edge.y_max = m_pEDGEs[m_PtsNum - 1].up.y();
	m_pEDGEs[m_PtsNum - 1].edge.x = m_pEDGEs[m_PtsNum - 1].down.x();
	if (m_pEDGEs[m_PtsNum - 1].up.y() == m_pEDGEs[m_PtsNum - 1].down.y())
		m_pEDGEs[m_PtsNum - 1].edge.dx = DBL_MAX;
	else
		m_pEDGEs[m_PtsNum - 1].edge.dx =((double)m_pEDGEs[m_PtsNum - 1].up.x() - (double)m_pEDGEs[m_PtsNum - 1].down.x()) / ((double)m_pEDGEs[m_PtsNum - 1].up.y() - (double)m_pEDGEs[m_PtsNum - 1].down.y());
}

//	Init new edge table
void ScanLine::InitNET()
{
	NET_.clear();

	for (int i = 0; i < y_max_-y_min_+1; i++)
	{
		NET_.append(QVector<Edge>(0));
	}
	//	加边
	for (int i = 0; i < m_PtsNum; i++)
	{
		int scanline = m_pEDGEs[i].down.y() - y_min_;
		NET_[scanline].push_back(m_pEDGEs[i].edge);
	}
	// 排序，根据xmin
	for (int n = y_min_; n < y_max_; n++)
	{
		int index = n - y_min_;
		int sz = NET_[index].size();
		for (int i = 0; i < sz - 1; i++)
		{
			for (int k = i + 1; k < sz; k++)
			{
				if (NET_[index][i].x > NET_[index][k].x)
				{
					Edge temp = NET_[index][i];
					NET_[index][i] = NET_[index][k];
					NET_[index][k] = temp;
				}
			}
		}
	}
}

// 得到 待激活边表
void ScanLine::InitAET()
{
	AET_.clear();
	for (int i = 0; i < y_max_ - y_min_ + 1; i++)
	{
		AET_.append(QVector<Edge>(0));
	}
}

// 生成
void ScanLine::FillPolygon()
{
	cv::Mat img_mask(height_ + 1, width_ + 1, CV_8UC1, cv::Scalar(255));
	for (scan_index = y_min_; scan_index < y_max_; scan_index++)
	{
		MoveNewEdgeFromNET();
		RemoveEdges();
		SortAET();
		FillScanLine();
		UpdateNET();
	}
}

//移动边从NET到AET
void ScanLine::MoveNewEdgeFromNET()
{
	int index = scan_index - y_min_;
	for (int i = 0; i < NET_[index].size(); i++)
	{
		AET_[index].push_back(NET_[index][i]);
	}
}

//删除AET中之前的边
void ScanLine::RemoveEdges()
{
	int index = scan_index - y_min_;
	for (int i = 0; i < AET_[index].size(); i++)
	{
		if (scan_index == AET_[index][i].y_max)
		{
			AET_[index].remove(i);
			i--;
		}
	}
}

//AET表排序
void ScanLine::SortAET()
{
	int index = scan_index - y_min_;
	int sz = AET_[index].size();
	for (int i = 0; i < sz - 1; i++)
	{
		for (int k = i + 1; k < sz; k++)
		{
			if (AET_[index][i].x > AET_[index][k].x)
			{
				Edge temp = AET_[index][i];
				AET_[index][i] = AET_[index][k];
				AET_[index][k] = temp;
			}
		}
	}
}

// 对扫描线插值
void ScanLine::FillScanLine()
{
	int index = scan_index - y_min_;
	for (int i = 0; i < AET_[index].size() - 1; i += 2)
	{
		for (int x0 = AET_[index][i].x; x0 < (int)(AET_[index][i + 1].x); x0++)
		{
			inside_mask_(index, x0 - x_min_) = 1;
		}
	}
}

// 更新NET表
void ScanLine::UpdateNET()
{
	int index = scan_index - y_min_;
	int sz = AET_[index].size();
	for (int i = 0; i < sz; i++)
	{
		AET_[index][i].x += AET_[index][i].dx;
		AET_[index + 1].push_back(AET_[index][i]);
	}
}