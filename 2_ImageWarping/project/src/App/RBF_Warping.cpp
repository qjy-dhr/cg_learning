#include "RBF_Warping.h"


RBF_Warping::RBF_Warping()
{
}

RBF_Warping::RBF_Warping(vector<QPoint>& points_p_, vector<QPoint>& points_q_)
{
	points_p = points_p_;
	points_q = points_q_;
	u_ = -1;
	size_ = points_p.size();
	Dis_ = new double[size_ * size_];
	Ri_ = new double[size_];
	getDisMartix();
	getRi();
	getAi();
}

RBF_Warping::~RBF_Warping()
{
	delete Dis_;
	delete Ri_;
}

double RBF_Warping::getDis(QPoint p, QPoint q)
{
	return (p.x() - q.x()) * (p.x() - q.x()) + (p.y() - q.y()) * (p.y() - q.y());
}

void RBF_Warping::getDisMartix()
{
	for (size_t i = 0; i < size_; i++)
	{
		Dis_[i * size_ + i] = 0;
		for (size_t j = i + 1; j < size_; j++)
		{
			Dis_[i * size_ + j] = Dis_[j * size_ + i] = getDis(points_p[i], points_p[j]);
		}
	}
}

void RBF_Warping::getRi()
{
	double min;
	//Ri=Dis中每行(或每列)最小的值(除去其距自身的距离->Dis_[i*size+i])
	for (size_t i = 0; i < size_; i++)
	{
		min = INT_MAX;
		for (size_t j = 0; j < size_; j++)
		{
			if (min > Dis_[i * size_ + j] && i != j)
				min = Dis_[i * size_ + j];
		}
		Ri_[i] = min;
	}
}

void RBF_Warping::getAi()
{
	Matrix<double, Dynamic, Dynamic> K(size_, size_);
	VectorXd By(size_);
	VectorXd Bx(size_);
	for (size_t i = 0; i < size_; i++)
	{
		By(i) = points_q[i].y() - points_p[i].y();
		Bx(i) = points_q[i].x() - points_p[i].x();
		for (size_t j = 0; j < size_; j++)
		{
			K(i, j) = pow(Dis_[j * size_ + i] + Ri_[j], u_);
		}
	}
	Ax_ = K.colPivHouseholderQr().solve(Bx);
	Ay_ = K.colPivHouseholderQr().solve(By);
}

QPoint RBF_Warping::pToq(QPoint p)
{
	double x = p.x(), y = p.y(), t;
	for (int i = 0; i < size_; i++)
	{
		t = pow(getDis(p, points_p[i]) + Ri_[i], u_);
		x += Ax_(i) * t;
		y += Ay_(i) * t;
	}
	return QPoint(x, y);
}