#pragma once
#include "ScanLine.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <Eigen/Sparse>
#include<Eigen/IterativeLinearSolvers>

class Poisson
{
public:
	Poisson();

	~Poisson();

	void InitPoisson(Eigen::MatrixXi inside_mas,cv::Mat source_img);

	void Paste(QPoint paste_point, QPoint source_point, cv::Mat& target_img_, cv::Mat& source_img_);

	void PoissonPaste(QPoint paste_point, QPoint source_point, cv::Mat& target_img_, cv::Mat& source_img_);

	void MixingPoissonPaste(QPoint paste_point, QPoint source_point, cv::Mat& target_img_, cv::Mat& source_img_);

private:

	void Predecomposition();

	double VecLength(cv::Vec3i vec);

private:
	QPoint source_point_, target_point_;

	int pixels_num_;

	int width_, height_;

	Eigen::MatrixXi index_matrix_;

	Eigen::SparseMatrix<float> sparse_matrix_;

	Eigen::MatrixXi inside_mask_;

	Eigen::SimplicialLLT<Eigen::SparseMatrix<float>> solver;

	Eigen::VectorXf div_red_;

	Eigen::VectorXf div_green_ ;

	Eigen::VectorXf div_blue_ ;
};