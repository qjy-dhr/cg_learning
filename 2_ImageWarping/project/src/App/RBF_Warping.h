#pragma once
#include <vector>
#include <QPoint>
#include <Eigen/Dense>
#include<iostream>

using namespace std;
using namespace Eigen;

class RBF_Warping
{
public:
	RBF_Warping();

	RBF_Warping(vector<QPoint>& p_points, vector<QPoint>& q_points);

	~RBF_Warping();

public:
	QPoint pToq(QPoint p);					   //计算RBF算法将p点映射到的点

private:
	double getDis(QPoint p, QPoint q);		   //计算两点的距离

	void getDisMartix();				       //计算2个p点形成的矩阵

	void getRi();							   //计算每个p数据点对应的Ri

	void getAi();							   //计算出线性方程组中的解ax,ay

private:
	double u_;						           //计算公式中的一个参数-1

	int size_;						           //输入数据点对的数量
	 
	double* Dis_;						       //距离矩阵

	double* Ri_;					           //Ri数组(存储每个p数据点对应的R值)

	vector<QPoint> points_p;		           //输入的数据点对

	vector<QPoint> points_q;		           //转换而得的数据点对q

	VectorXd Ax_;					           //方程组的解

	VectorXd Ay_;
};
