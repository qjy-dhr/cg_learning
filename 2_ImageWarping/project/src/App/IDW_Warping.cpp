#include "IDW_Warping.h"
#include<iostream>
using namespace std;

IDW_Warping::IDW_Warping()
{
}

IDW_Warping::IDW_Warping(vector<QPoint> points_p_, vector<QPoint> points_q_)
{
	u_ = 2;
	points_p = points_p_;
	points_q = points_q_;
	size_ = points_p.size();

	//初始化矩阵T
	T_ = new double* [2];
	for (int i = 0; i < 2; i++)
		T_[i] = new double[size_ * 2];

	//计算变换矩阵T
	for (int i = 0; i < size_; i++)
		getT(points_p[i], i);
}

IDW_Warping::~IDW_Warping()
{
	delete T_[0];
	delete T_[1];
	delete[] T_;
}

double IDW_Warping::getDis(QPoint p, QPoint q)
{
	return (p.x() - q.x()) * (p.x() - q.x()) + (p.y() - q.y()) * (p.y() - q.y());
}

void IDW_Warping::getSum(QPoint p)
{
	sum_ = 0;
	for (size_t i = 0; i < size_; i++)
	{
		if (getDis(p, points_p[i]) == 0)
			continue;
		sum_ += 1.0 / pow(getDis(p, points_p[i]), u_ / 2.0);
	}
}

double IDW_Warping::getWi(QPoint p, int i)
{
	getSum(p);
	if (p == points_p[i])
		return 1;
	return 1.0 / pow(getDis(p, points_p[i]), u_ / 2.0) / sum_;
}

void IDW_Warping::getT(QPoint p, int i)
{
	double det = 0, wij;
	double t1 = 0, t2 = 0, t3 = 0, t4, px, py, qx, qy;
	T_[0][2 * i] = T_[0][2 * i + 1] = T_[1][2 * i] = T_[1][2 * i + 1] = 0;

	for (int j = 0; j < size_; j++)
	{
		if (j == i)
			continue;
		wij = getWi(p, j);
		px = points_p[j].x() - points_p[i].x();
		py = points_p[j].y() - points_p[i].y();
		t1 += wij * px * px;
		t2 += wij * py * py;
		t3 += wij * px * py;
	}

	det = t1 * t2 - t3 * t3;

	if (det == 0)
	{
		for (int j = 0; j < size_; j++)
		{
			if (j == i)
				continue;
			wij = getWi(p, j);
			px = points_p[j].x() - points_p[i].x();
			py = points_p[j].y() - points_p[i].y();
			det += wij * (px * px + py * py);
		}
		for (int j = 0; j < size_; j++)
		{
			if (j == i)
				continue;
			wij = getWi(p, j);
			px = points_p[j].x() - points_p[i].x();
			py = points_p[j].y() - points_p[i].y();
			qx = points_q[j].x() - points_q[i].x();
			qy = points_q[j].y() - points_q[i].y();
			T_[0][2 * i] += wij * (px * qx + py * qy);
			T_[0][2 * i + 1] += wij * (py * qx - px * qy);
		}
		T_[0][2 * i] /= det;
		T_[1][2 * i + 1] = T_[0][2 * i];
		T_[0][2 * i + 1] /= det;
		T_[1][2 * i] = -T_[0][2 * i + 1];
	}
	else
	{
		t1 = t2 = t3 = t4 = 0;
		for (int j = 0; j < size_; j++)
		{
			if (j == i)
				continue;
			wij = getWi(p, j);
			px = points_p[j].x() - points_p[i].x();
			py = points_p[j].y() - points_p[i].y();
			qx = points_q[j].x() - points_q[i].x();
			qy = points_q[j].y() - points_q[i].y();
			t1 += wij * qx * px;
			t2 += wij * py * py;
			t3 += wij * qx * py;
			t4 += wij * px * py;
		}
		T_[0][2 * i] = (t1 * t2 - t3 * t4) / det;

		t1 = t2 = t3 = t4 = 0;
		for (int j = 0; j < size_; j++)
		{
			if (j == i)
				continue;
			wij = getWi(p, j);
			px = points_p[j].x() - points_p[i].x();
			py = points_p[j].y() - points_p[i].y();
			qx = points_q[j].x() - points_q[i].x();
			qy = points_q[j].y() - points_q[i].y();
			t1 += wij * qx * py;
			t2 += wij * px * px;
			t3 += wij * qx * px;
			t4 += wij * px * py;
		}
		T_[0][2 * i + 1] = (t1 * t2 - t3 * t4) / det;

		t1 = t2 = t3 = t4 = 0;
		for (int j = 0; j < size_; j++)
		{
			if (j == i)
				continue;
			wij = getWi(p, j);
			px = points_p[j].x() - points_p[i].x();
			py = points_p[j].y() - points_p[i].y();
			qx = points_q[j].x() - points_q[i].x();
			qy = points_q[j].y() - points_q[i].y();
			t1 += wij * qy * px;
			t2 += wij * py * py;
			t3 += wij * qy * py;
			t4 += wij * px * py;
		}
		T_[1][2 * i] = (t1 * t2 - t3 * t4) / det;

		t1 = t2 = t3 = t4 = 0;
		for (int j = 0; j < size_; j++)
		{
			if (j == i)
				continue;
			wij = getWi(p, j);
			px = points_p[j].x() - points_p[i].x();
			py = points_p[j].y() - points_p[i].y();
			qx = points_q[j].x() - points_q[i].x();
			qy = points_q[j].y() - points_q[i].y();
			t1 += wij * qy * py;
			t2 += wij * px * px;
			t3 += wij * qy * px;
			t4 += wij * px * py;
		}
		T_[1][2 * i + 1] = (t1 * t2 - t3 * t4) / det;
	}
}


QPoint IDW_Warping::getFiPoint(QPoint p, int i, bool simple)
{
	QPoint result;

	double x = p.x() - points_p[i].x();
	double y = p.y() - points_p[i].y();

	if (simple)
	{
		result.rx() = points_q[i].x() + x;
		result.ry() = points_q[i].y() + y;
	}
	else
	{
		result.rx() = points_q[i].x() + x * T_[0][2 * i] + y * T_[0][2 * i + 1];
		result.ry() = points_q[i].y() + x * T_[1][2 * i] + y * T_[1][2 * i + 1];
	}

	return result;
}

QPoint IDW_Warping::pToq(QPoint p, bool simple = true)
{
	double x = 0, y = 0, wi;
	QPoint fip;

	for (size_t i = 0; i < size_; i++)
	{
		fip = getFiPoint(p, i, simple);
		wi = getWi(p, i);
		x += fip.x() * wi;
		y += fip.y() * wi;
	}

	return QPoint(x, y);
}
