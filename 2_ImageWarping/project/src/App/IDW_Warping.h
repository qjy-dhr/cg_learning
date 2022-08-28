#pragma once
#include <vector>
#include "qpoint.h"
using namespace std;
class IDW_Warping
{
public:
	IDW_Warping();

	IDW_Warping(vector<QPoint> p_points, vector<QPoint> q_points);

	~IDW_Warping();

public:
	QPoint pToq(QPoint p, bool simple);							//计算IDW算法将p点映射到的点,simple=true：将每个局部近似函数fi(x)的变换矩阵T作为2阶单位矩阵,simple=false：计算每个局部近似函数fi(x)的变换矩阵T

private:
	double getDis(QPoint p, QPoint q);				            //计算2点距离的平方

	void getSum(QPoint p);						                //计算σi(x)=1/(di(x))^u的和

	void getT(QPoint p, int i);					                //局部近似函数fi(x)的变换矩阵T

	double getWi(QPoint p, int i);								//每个数据点对的权重

	QPoint getFiPoint(QPoint p, int i, bool simple);			//计算p点在局部近似函数fi(x)映射后的点

private:
	double u_;													//计算公式中的参数 2

	double** T_;												//变换矩阵T

	int size_;													//输入数据点对的数量

	double sum_;												//σi(x)=1/(di(x))^u的和

	vector<QPoint> points_p;									//输入的数据点对

	vector<QPoint> points_q;

};