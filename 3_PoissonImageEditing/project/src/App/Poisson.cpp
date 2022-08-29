#include "Poisson.h"
#include <ctime>

Poisson::Poisson()
{
	pixels_num_ = 0;
	index_matrix_.resize(0, 0);
	sparse_matrix_.resize(0, 0);
}

Poisson::~Poisson()
{
	
}

void Poisson::InitPoisson(Eigen::MatrixXi inside_mask,cv::Mat source_img)
{
	inside_mask_ = inside_mask;
	width_ = inside_mask_.rows();
	height_ = inside_mask_.cols();
	target_point_.setX(0);
	target_point_.setY(0);
	index_matrix_.resize(width_, height_);
	index_matrix_.setZero();
	for (int i = 0; i < width_; i++)
	{
		for (int j = 0; j < height_; j++)
		{
			if (inside_mask_(i, j) == 1)
			{
				index_matrix_(i, j) = pixels_num_;
				pixels_num_++;
			}
		}
	}
	
	sparse_matrix_.resize(pixels_num_, pixels_num_);

	sparse_matrix_.setZero();
	
	QVector<Eigen::Triplet<float>> coef;
	for (int i = 0; i < width_; i++)
	{
		for (int j = 0; j < height_; j++)
		{
			if (inside_mask_(i, j) == 1)
			{
				int index = index_matrix_(i, j);
				coef.push_back(Eigen::Triplet<float>(index, index, 4));
				if (i > 0 && inside_mask_(i - 1, j) == 1)
				{
					coef.push_back(Eigen::Triplet<float>(index, index_matrix_(i - 1, j), -1));
				}
				if (j > 0 && inside_mask_(i, j - 1) == 1)
				{
					coef.push_back(Eigen::Triplet<float>(index, index_matrix_(i, j - 1), -1));
				}
				if (i <width_ - 1 && inside_mask_(i + 1, j) == 1)
				{
					coef.push_back(Eigen::Triplet<float>(index, index_matrix_(i + 1, j), -1));
				}
				if (j < height_-1 && inside_mask_(i, j + 1) == 1)
				{
					coef.push_back(Eigen::Triplet<float>(index, index_matrix_(i, j + 1), -1));
				}
			}
		}
	}
	sparse_matrix_.setFromTriplets(coef.begin(), coef.end());

	sparse_matrix_.makeCompressed();

	Predecomposition();
}

void Poisson::Predecomposition()
{
	solver.compute(sparse_matrix_);
	if (solver.info() != Eigen::Success)
	{
		throw std::exception("Compute Matrix is error");
		return;
	}
}

void Poisson::Paste(QPoint target_point, QPoint source_point, cv::Mat& target_img_, cv::Mat& source_img_)
{
	target_point_ = target_point;
	source_point_ = source_point;
	for (int j = 0; j < width_; j++)
	{
		for (int k = 0; k < height_; k++)
		{
			if (inside_mask_(j, k) == 1)
			{
				target_img_.at<cv::Vec3b>(j + target_point_.y(), k + target_point_.x())[0] = source_img_.at<cv::Vec3b>(j + source_point_.y(), k + source_point_.x())[0];
				target_img_.at<cv::Vec3b>(j + target_point_.y(), k + target_point_.x())[1] = source_img_.at<cv::Vec3b>(j + source_point_.y(), k + source_point_.x())[1];
				target_img_.at<cv::Vec3b>(j + target_point_.y(), k + target_point_.x())[2] = source_img_.at<cv::Vec3b>(j + source_point_.y(), k + source_point_.x())[2];
			}
		}
	}
}

void Poisson::PoissonPaste(QPoint target_point, QPoint source_point, cv::Mat& target_img_, cv::Mat& source_img_)
{
	target_point_ = target_point;
	source_point_ = source_point;

	div_red_.resize(pixels_num_);
	div_green_.resize(pixels_num_);
	div_blue_.resize(pixels_num_);
	div_red_.setZero();
	div_green_.setZero();
	div_blue_.setZero();

	for (int i = 0; i < width_; i++)
	{
		for (int j = 0; j < height_; j++)
		{
			if (inside_mask_(i, j) == 1)
			{
				int index = index_matrix_(i, j);
				int x = source_point_.y() + i;
				int y = source_point_.x() + j;
				cv::Vec3i temp_vec = source_img_.at<cv::Vec3b>(x, y);
				temp_vec *= 4;
				temp_vec -= source_img_.at<cv::Vec3b>(x + 1, y);
				temp_vec -= source_img_.at<cv::Vec3b>(x - 1, y);
				temp_vec -= source_img_.at<cv::Vec3b>(x, y - 1);
				temp_vec -= source_img_.at<cv::Vec3b>(x, y + 1);

				div_red_(index_matrix_(i, j)) += temp_vec[0];
				div_green_(index_matrix_(i, j)) += temp_vec[1];
				div_blue_(index_matrix_(i, j)) += temp_vec[2];

				if (i == 0 || (i > 0 && inside_mask_(i - 1, j) == 0))
				{
					div_red_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y() - 1, j + target_point_.x())[0];
					div_green_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y() - 1, j + target_point_.x())[1];
					div_blue_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y() - 1, j + target_point_.x())[2];
				}
				if (i == width_ - 1 || (i < width_ - 1 && inside_mask_(i + 1, j) == 0))
				{
					div_red_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y() + 1, j + target_point_.x())[0];
					div_green_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y() + 1, j + target_point_.x())[1];
					div_blue_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y() + 1, j + target_point_.x())[2];
				}
				if (j == 0 || (j > 0 && inside_mask_(i, j - 1) == 0))
				{
					div_red_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y(), j + target_point_.x() - 1)[0];
					div_green_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y(), j + target_point_.x() - 1)[1];
					div_blue_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y(), j + target_point_.x() - 1)[2];
				}
				if (j == height_ - 1 || (j < height_ - 1 && inside_mask_(i, j + 1) == 0))
				{
					div_red_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y(), j + target_point_.x() + 1)[0];
					div_green_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y(), j + target_point_.x() + 1)[1];
					div_blue_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y(), j + target_point_.x() + 1)[2];
				}
			}
		}
	}

	Eigen::VectorXf vec_red(pixels_num_);
	Eigen::VectorXf vec_green(pixels_num_);
	Eigen::VectorXf vec_blue(pixels_num_);

	vec_red = solver.solve(div_red_);
	vec_green = solver.solve(div_green_);
	vec_blue = solver.solve(div_blue_);

	for (int j = 0; j < width_; j++)
	{
		for (int k = 0; k < height_; k++)
		{
			if (inside_mask_(j, k) == 1)
			{
				int index = index_matrix_(j, k);
				int red = vec_red(index);
				int green = vec_green(index);
				int blue = vec_blue(index);
				target_img_.at<cv::Vec3b>(j + target_point_.y(), k + target_point_.x())[0] = red > 255 ? 255 : (red < 0 ? 0 : red);
				target_img_.at<cv::Vec3b>(j + target_point_.y(), k + target_point_.x())[1] = green > 255 ? 255 : (green < 0 ? 0 : green);
				target_img_.at<cv::Vec3b>(j + target_point_.y(), k + target_point_.x())[2] = blue > 255 ? 255 : (blue < 0 ? 0 : blue);					
			}
		}
	}
}

double Poisson::VecLength(cv::Vec3i vec)
{
	return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

void Poisson::MixingPoissonPaste(QPoint target_point, QPoint source_point, cv::Mat& target_img_, cv::Mat& source_img_)
{
	target_point_ = target_point;
	source_point_ = source_point;

	div_red_.resize(pixels_num_);
	div_green_.resize(pixels_num_);
	div_blue_.resize(pixels_num_);
	div_red_.setZero();
	div_green_.setZero();
	div_blue_.setZero();

	for (int i = 0; i < width_; i++)
	{
		for (int j = 0; j < height_; j++)
		{
			if (inside_mask_(i, j) == 1)
			{
				int index = index_matrix_(i, j);
				int x = source_point_.y() + i;
				int y = source_point_.x() + j;
				int x_ = target_point_.y() + i;
				int y_ = target_point_.x() + j;
				cv::Vec3i vec, temp_vec, temp_vec_paste;

				temp_vec = source_img_.at<cv::Vec3b>(x, y);
				temp_vec -= source_img_.at<cv::Vec3b>(x + 1, y);
				temp_vec_paste = target_img_.at<cv::Vec3b>(x_, y_);
				temp_vec_paste -= target_img_.at<cv::Vec3b>(x_ + 1, y_);
				vec = VecLength(temp_vec) > VecLength(temp_vec_paste) ? temp_vec : temp_vec_paste;
				
				temp_vec = source_img_.at<cv::Vec3b>(x, y);
				temp_vec -= source_img_.at<cv::Vec3b>(x - 1, y);
				temp_vec_paste = target_img_.at<cv::Vec3b>(x_, y_);
				temp_vec_paste -= target_img_.at<cv::Vec3b>(x_ - 1, y_);
				vec += VecLength(temp_vec) > VecLength(temp_vec_paste) ? temp_vec : temp_vec_paste;

				temp_vec = source_img_.at<cv::Vec3b>(x, y);
				temp_vec -= source_img_.at<cv::Vec3b>(x, y + 1);
				temp_vec_paste = target_img_.at<cv::Vec3b>(x_, y_);
				temp_vec_paste -= target_img_.at<cv::Vec3b>(x_, y_ + 1);
				vec += VecLength(temp_vec) > VecLength(temp_vec_paste) ? temp_vec : temp_vec_paste;

				temp_vec = source_img_.at<cv::Vec3b>(x, y);
				temp_vec -= source_img_.at<cv::Vec3b>(x, y - 1);
				temp_vec_paste = target_img_.at<cv::Vec3b>(x_, y_);
				temp_vec_paste -= target_img_.at<cv::Vec3b>(x_, y_ - 1);
				vec += VecLength(temp_vec) > VecLength(temp_vec_paste) ? temp_vec : temp_vec_paste;

				div_red_(index_matrix_(i, j)) = vec[0];
				div_green_(index_matrix_(i, j)) = vec[1];
				div_blue_(index_matrix_(i, j)) = vec[2];


				if (i == 0 || (i > 0 && inside_mask_(i - 1, j) == 0))
				{
					div_red_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y() - 1, j + target_point_.x())[0];
					div_green_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y() - 1, j + target_point_.x())[1];
					div_blue_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y() - 1, j + target_point_.x())[2];
				}
				if (i == width_ - 1 || (i < width_ - 1 && inside_mask_(i + 1, j) == 0))
				{
					div_red_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y() + 1, j + target_point_.x())[0];
					div_green_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y() + 1, j + target_point_.x())[1];
					div_blue_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y() + 1, j + target_point_.x())[2];
				}
				if (j == 0 || (j > 0 && inside_mask_(i, j - 1) == 0))
				{
					div_red_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y(), j + target_point_.x() - 1)[0];
					div_green_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y(), j + target_point_.x() - 1)[1];
					div_blue_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y(), j + target_point_.x() - 1)[2];
				}
				if (j == height_ - 1 || (j < height_ - 1 && inside_mask_(i, j + 1) == 0))
				{
					div_red_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y(), j + target_point_.x() + 1)[0];
					div_green_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y(), j + target_point_.x() + 1)[1];
					div_blue_[index] += target_img_.at<cv::Vec3b>(i + target_point_.y(), j + target_point_.x() + 1)[2];
				}
			}
		}
	}

	Eigen::VectorXf vec_red(pixels_num_);
	Eigen::VectorXf vec_green(pixels_num_);
	Eigen::VectorXf vec_blue(pixels_num_);

	vec_red = solver.solve(div_red_);
	vec_green = solver.solve(div_green_);
	vec_blue = solver.solve(div_blue_);

	for (int j = 0; j < width_; j++)
	{
		for (int k = 0; k < height_; k++)
		{
			if (inside_mask_(j, k) == 1)
			{
				int index = index_matrix_(j, k);
				int red = vec_red(index);
				int green = vec_green(index);
				int blue = vec_blue(index);
				target_img_.at<cv::Vec3b>(j + target_point_.y(), k + target_point_.x())[0] = red > 255 ? 255 : (red < 0 ? 0 : red);
				target_img_.at<cv::Vec3b>(j + target_point_.y(), k + target_point_.x())[1] = green > 255 ? 255 : (green < 0 ? 0 : green);
				target_img_.at<cv::Vec3b>(j + target_point_.y(), k + target_point_.x())[2] = blue > 255 ? 255 : (blue < 0 ? 0 : blue);
			}
		}
	}
}