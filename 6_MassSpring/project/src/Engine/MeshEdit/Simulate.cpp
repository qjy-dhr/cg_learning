#include <Engine/MeshEdit/Simulate.h>


using namespace Ubpa;
using namespace std;

void Simulate::Clear() {
	this->positions.clear();
	this->velocity.clear();
	this->isFixed.clear();
}
bool Simulate::Init()
{
	this->velocity.resize(positions.size());
	for (int i = 0; i < positions.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			this->velocity[i][j] = 0.f;
		}
	}
	//初始化弹簧原长和邻接表
	linkedlist.clear();
	original_length.clear();

	nV = positions.size();
	nE = edgelist.size() / 2;

	linkedlist.resize(nV);
	original_length.resize(nE);

	for (size_t i = 0; i < nE; i++)
	{
		int vi = edgelist[2 * i], vj = edgelist[2 * i + 1];
		linkedlist[vi].push_back(vj);
		linkedlist[vj].push_back(vi);
		original_length[i] = (positions[vi]- positions[vj]).norm();
	}

	//初始化固定点
	isFixed.clear();
	isFixed.resize(nV);
	for (auto i : isFixed) {
		i = false;
	}
	for (int idx : fixed_id)
	{
		isFixed[idx] = true;
	}

	//Found M + h2 * L
	vector<Triplet<float>> tripletlistM;
	KM_hhL.resize(nV, nV);
	for (int i = 0; i < nV; i++)
	{
		if (isFixed[i])
		{
			tripletlistM.push_back(Triplet<float>(i, i, 1.f));
			continue;
		}
		float iicoef = M;
		for (int j : linkedlist[i])
		{
			float Lij = stiff * h * h;
			iicoef += Lij;
			tripletlistM.push_back(Triplet(i, j, -Lij));
		}
		tripletlistM.push_back(Triplet<float>(i, i, iicoef));
	}
	KM_hhL.setFromTriplets(tripletlistM.begin(), tripletlistM.end());

	//Found J
	vector<Triplet<float>> tripletlistJ;
	J.resize(nV, nE);
	for (int i = 0; i < nE; i++)
	{
		tripletlistJ.push_back(Triplet<float>(edgelist[2 * i], i, stiff));
		tripletlistJ.push_back(Triplet<float>(edgelist[2 * i + 1], i, -stiff));
	}
	J.setFromTriplets(tripletlistJ.begin(), tripletlistJ.end());

	d.resize(nE, 3);
	A_ReLU.compute(KM_hhL);
	Phi.resize(nV, 3);
	for (int i = 0; i < nV; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Phi(i, j) = -h * h * gravity[j] * M;
		}
	}
	return true;
}

bool Simulate::Run() {

	SimulateOnce();
	return true;
}

void Simulate::SetLeftFix()
{
	//固定网格x坐标最小点
	fixed_id.clear();
	double x = 100000;
	for (int i = 0; i < positions.size(); i++)
	{
		if (positions[i][0] < x)
		{
			x = positions[i][0];
		}
	}

	for (int i = 0; i < positions.size(); i++)
	{
		if (abs(positions[i][0] - x) < 1e-5)
		{
			fixed_id.push_back(i);
		}
	}

	Init();
}
void Simulate::SetUpFix()
{
	//固定网格y坐标最小点
	fixed_id.clear();
	double y = 0;
	for (int i = 0; i < positions.size(); i++)
	{
		if (positions[i][1] > y)
		{
			y = positions[i][1];
		}
	}

	for (int i = 0; i < positions.size(); i++)
	{
		if (abs(positions[i][1] - y) < 1e-5)
		{
			fixed_id.push_back(i);
		}
	}

	Init();
}
void Simulate::SetUpLRFix()
{
	//固定网格y坐标最小点
	fixed_id.clear();
	double y = 0;
	for (int i = 0; i < positions.size(); i++)
	{
		if (positions[i][1] > y)
		{
			y = positions[i][1];
		}
	}

	double xmin = 100000, xmax = 0;
	for (int i = 0; i < positions.size(); i++)
	{
		if (positions[i][0] < xmin)
		{
			xmin = positions[i][0];
		}
		if (positions[i][0] > xmax)
		{
			xmax = positions[i][0];
		}
	}

	for (int i = 0; i < positions.size(); i++)
	{
		if (abs(positions[i][1] - y) < 1e-5)
		{
			if (abs(positions[i][0] - xmin) < 1e-5|| abs(positions[i][0] - xmax) < 1e-5)
				fixed_id.push_back(i);
		}
	}

	Init();
}

void Simulate::SimulateOnce() {
	// TODO
	switch (simulate_type)
	{
	case SimulateType::ImplicitEuler:
		ImplicitEuler();
		break;

	case SimulateType::ProjectiveDynamic:
		ProjectiveDynamic();
		break;
	}
}

void Simulate::ImplicitEuler()
{
	vector<Triplet<float>> tripletlist;
	MatrixXf X[2];
	X[0].resize(nV, 3); X[1].resize(nV, 3);
	MatrixXf Y;
	Y.resize(nV, 3);
	
	for (int i = 0; i < nV; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Y(i, j) = positions[i][j] + h * velocity[i][j] + h * h * gravity[j] / M;
		}
	}
	X[0] = Y;
	SparseMatrix<float> g_m;
	g_m.resize(3 * nV, 3 * nV);
	SimplicialLLT<SparseMatrix<float>> g_ReLU;
	Matrix3f I;
	I.fill(0.f);
	I(0, 0) = I(1, 1) = I(2, 2) = 1.f;
	int cnt = 0;
	float delta;
	do{
		//生成矩阵
		tripletlist.clear();
		tripletlist.reserve(nE * 3 * 9);
		for (int i = 0; i < nV; i++)
		{
			Matrix3f iitemp = M * I;
			for (int j : linkedlist[i])
			{
				Matrix<float, 1, 3> r_v = X[cnt % 2].row(i) - X[cnt % 2].row(j);
				float r_len = r_v.norm() + 0.0001f;
				Matrix3f temp = (-h * h) * ((stiff * (original_length[i] / r_len - 1.f)) * I - 
					(stiff * original_length[i] / r_len / r_len / r_len) * (r_v.transpose() * r_v));
				iitemp += temp;
				for (int p = 0; p < 3; p++)
				{
					for (int q = 0; q < 3; q++)
					{
						tripletlist.push_back(Triplet<float>(i * 3 + p, j * 3 + q, -temp(p, q)));
					}
				}
			}
			for (int p = 0; p < 3; p++)
			{
				for (int q = 0; q < 3; q++)
				{
					tripletlist.push_back(Triplet<float>(i * 3 + p, i * 3 + q, iitemp(p, q)));
				}
			}
		}
		g_m.setFromTriplets(tripletlist.begin(), tripletlist.end());
		g_ReLU.compute(g_m);

		//Get g(x)
		MatrixXf g;
		MatrixXf f;
		f.resize(nV, 3);
		f.fill(0.f);
		for (int i = 0; i < nE; i++)
		{
			int vi = edgelist[2 * i], vj = edgelist[2 * i + 1];
			Matrix<float, 1, 3> r_vec = X[cnt % 2].row(vj) - X[cnt % 2].row(vi);
			float r_len = r_vec.norm() + 0.0001f;
			Matrix<float, 1, 3> force = (stiff * (r_len - original_length[i]) / r_len) * r_vec;
			f.row(vi) += force;
			f.row(vj) -= force;
		}
		g.resize(nV, 3);
		g = M * (X[cnt % 2] - Y) - (h * h) * f;

		//计算差值
		MatrixXf v;
		v.resize(g.rows() * 3, 1);
		for (int i = 0; i < g.rows(); i++)
		{
			v(3 * i, 0) = g(i, 0);
			v(3 * i + 1, 0) = g(i, 1);
			v(3 * i + 2, 0) = g(i, 2);
		}
		MatrixXf U= g_ReLU.solve(v);
		MatrixXf deltaX;
		deltaX.resize(U.rows() / 3, 3);
		for (int i = 0; i < deltaX.rows(); i++)
		{
			deltaX(i, 0) = U(3 * i, 0);
			deltaX(i, 1) = U(3 * i + 1, 0);
			deltaX(i, 2) = U(3 * i + 2, 0);
		}
		deltaX = -deltaX;
		X[(cnt + 1) % 2] = X[cnt % 2] + deltaX;
		cnt++;
		delta = deltaX.norm();
		if (isinf(delta) || isnan(delta))
			return ;
	} while (delta > 10e-8 && cnt <= 10);

	//转移模拟结果
	for (int i = 0; i < nV; i++)
	{
		for (int j = 0; j < 3; j++)
			velocity[i][j] = (X[cnt % 2](i, j) - positions[i][j]) / h;
		if (!isFixed[i])
			for (int j = 0; j < 3; j++)
				positions[i][j] = X[cnt % 2](i, j);
	}
	for (int idx : fixed_id)
	{
		velocity[idx] = pointf3(0.f, 0.f, 0.f);
	}
}

void Simulate::ProjectiveDynamic()
{
	int iterk = 0;
	MatrixX3f Y;
	Y.resize(nV, 3);
	for (int i = 0; i < nV; i++)
		for (int j = 0; j < 3; j++)
			Y(i, j) = positions[i][j] + h * velocity[i][j];
	MatrixX3f X = Y;
	Phi.resize(nV, 3);
	for (int i = 0; i < nV; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Phi(i, j) = -h * h * gravity[j] / M;
		}
	}
	MatrixX3f deltaX; //迭代数据
	while (true)
	{
		//local
		for (int i = 0; i < nE; i++)
		{
			size_t vi = edgelist[2 * i], vj = edgelist[2 * i + 1];
			Matrix<float, 1, 3> r_vec = X.row(vi) - X.row(vj);
			d.row(i) = original_length[i] / r_vec.norm() * r_vec;
		}
		MatrixX3f vecB = (h * h) * J * d + M * Y - Phi;
		for (auto i : fixed_id)
		{
			for (int j = 0; j < 3; j++)
				vecB(i, j) = positions[i][j];
		}
		deltaX = A_ReLU.solve(vecB) - X;
		X = X + deltaX;

		float delta = deltaX.norm() / nV;
		if (iterk > 5)
			break;
		iterk++;
	}

	for (int i = 0; i < nV; i++)
	{
		for (int j = 0; j < 3; j++)
			velocity[i][j] = (X(i, j) - positions[i][j]) / h;
		if (!isFixed[i])
			for (int j = 0; j < 3; j++)
				positions[i][j] = X(i, j);
	}
	for (int idx : fixed_id)
	{
		velocity[idx] = pointf3(0.f, 0.f, 0.f);
	}
}
void Simulate::SetSimulateType(SimulateType type) {
	simulate_type = type;
	Init();
}