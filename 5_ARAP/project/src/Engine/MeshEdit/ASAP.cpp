#include <Engine/MeshEdit/ASAP.h>

#include <Engine/MeshEdit/MinSurf.h>

#include <Engine/Primitive/TriMesh.h>

#include <Eigen/Sparse>

#include <Engine/MeshEdit/Paramaterize.h>

using namespace Ubpa;
using namespace std;
using namespace Eigen;

ASAP::ASAP(Ptr<TriMesh> triMesh)
	: heMesh(make_shared<HEMesh<V>>())
{
	Init(triMesh);
}

void ASAP::Clear() {
	heMesh->Clear();
	triMesh = nullptr;
}

bool ASAP::Init(Ptr<TriMesh> triMesh) {
	Clear();

	if (triMesh == nullptr)
		return true;

	if (triMesh->GetType() == TriMesh::INVALID) {
		printf("ERROR::ASAP::Init:\n"
			"\t""trimesh is invalid\n");
		return false;
	}

	// init half-edge structure 初始化半边结构
	size_t nV = triMesh->GetPositions().size();
	vector<vector<size_t>> triangles;
	triangles.reserve(triMesh->GetTriangles().size());
	for (auto triangle : triMesh->GetTriangles())
		triangles.push_back({ triangle->idx[0], triangle->idx[1], triangle->idx[2] });
	heMesh->Reserve(nV);
	heMesh->Init(triangles);

	//1.判断边界
	if (!heMesh->IsTriMesh() || !heMesh->HaveBoundary()) {
		printf("ERROR::ASAP::Init:\n"
			"\t""trimesh is not a triangle mesh or hasn't a boundaries\n");
		heMesh->Clear();
		return false;
	}

	// triangle mesh's texcoords and positions ->  half-edge structure's texcoords and positions
	for (int i = 0; i < nV; i++) {
		auto v = heMesh->Vertices().at(i);
		v->coord = triMesh->GetTexcoords()[i].cast_to<vecf2>();
		v->pos = triMesh->GetPositions()[i].cast_to<vecf3>();
	}

	this->triMesh = triMesh;
	return true;
}

bool ASAP::Run() {
	if (heMesh->IsEmpty() || !triMesh) {
		printf("ERROR::ASAP::Run\n"
			"\t""heMesh->IsEmpty() || !triMesh\n");
		return false;
	}
	getParamaterize();

	// half-edge structure -> triangle mesh
	vector<pointf2> texcoords;
	for (auto v : heMesh->Vertices())
		texcoords.push_back(v->coord.cast_to<pointf2>());
	triMesh->Update(texcoords);
	return true;
}

bool ASAP::GetMesh()
{
	if (heMesh->IsEmpty() || !triMesh) {
		printf("ERROR::ASAP::Show\n"
			"\t""heMesh->IsEmpty() || !triMesh\n");
		return false;
	}

	getParamaterize();

	// half-edge structure -> triangle mesh
	vector<pointf3> positions;
	vector<unsigned> indice;
	positions.reserve(nV);
	indice.reserve(3 * nP);

	// Set positions
	for (auto v : heMesh->Vertices()) {
		positions.push_back(v->coord.cast_to<pointf3>());
	}

	for (auto f : heMesh->Polygons()) { // f is triangle
		for (auto v : f->BoundaryVertice()) { // vertices of the triangle
			indice.push_back(static_cast<unsigned>(heMesh->Index(v)));
		}
	}

	triMesh->Init(indice, positions);

	// Set texcoords
	vector<pointf2> texcoords;
	for (auto v : heMesh->Vertices())
		texcoords.push_back(v->coord.cast_to<pointf2>());
	triMesh->Update(texcoords);
	return true;
}
 
void ASAP::getParamaterize()
{
	nP = (int)(heMesh->NumPolygons());
    nV = (int)(heMesh->NumVertices());
	//TurnTo2D
	TurnTo2D();

	// 选择两个定点
	auto Hemesh_Bound = heMesh->Boundaries()[0];
	auto point1 = Hemesh_Bound[0]->Origin();
	auto point2 = Hemesh_Bound[Hemesh_Bound.size() / 2]->Origin();
	vector<double> two_points = {0, 0, 1, 1};

	// 构建方程
	vector<Triplet<double>> Coef_Triplet;
	VectorXd b = VectorXd::Zero(2 * nV + 2 * nP);

	for (auto triangles : heMesh->Polygons())
	{
		size_t i = heMesh->Index(triangles);
		double value = 0.0;		
		for (size_t j = 0; j < 3; j++)
		{
			int v_index = (int)(heMesh->Index(triangles->BoundaryVertice()[j]));
			value += cotangent_[i][j] * (delta_x_[i][j] * delta_x_[i][j] + delta_y_[i][j] * delta_y_[i][j]);
			if (v_index != heMesh->Index(point1) && v_index != heMesh->Index(point2))
			{
				Coef_Triplet.push_back(Triplet<double>(2 * nV + 2 * i, 2 * v_index,-cotangent_[i][j] * delta_x_[i][j] + cotangent_[i][j == 0 ? 2 : j - 1] * delta_x_[i][j == 0 ? 2 : j - 1]));
				Coef_Triplet.push_back(Triplet<double>(2 * nV + 2 * i, 2 * v_index + 1,-cotangent_[i][j] * delta_y_[i][j] + cotangent_[i][j == 0 ? 2 : j - 1] * delta_y_[i][j == 0 ? 2 : j - 1]));
				Coef_Triplet.push_back(Triplet<double>(2 * nV + 2 * i + 1, 2 * v_index,-cotangent_[i][j] * delta_y_[i][j] + cotangent_[i][j == 0 ? 2 : j - 1] * delta_y_[i][j == 0 ? 2 : j - 1]));
				Coef_Triplet.push_back(Triplet<double>(2 * nV + 2 * i + 1, 2 * v_index + 1,cotangent_[i][j] * delta_x_[i][j] - cotangent_[i][j == 0 ? 2 : j - 1] * delta_x_[i][j == 0 ? 2 : j - 1]));

				Coef_Triplet.push_back(Triplet<double>(2 * v_index, 2 * nV + 2 * i,-cotangent_[i][j] * delta_x_[i][j] + cotangent_[i][j == 0 ? 2 : j - 1] * delta_x_[i][j == 0 ? 2 : j - 1]));
				Coef_Triplet.push_back(Triplet<double>(2 * v_index + 1, 2 * nV + 2 * i,-cotangent_[i][j] * delta_y_[i][j] + cotangent_[i][j == 0 ? 2 : j - 1] * delta_y_[i][j == 0 ? 2 : j - 1]));
				Coef_Triplet.push_back(Triplet<double>(2 * v_index, 2 * nV + 2 * i + 1,-cotangent_[i][j] * delta_y_[i][j] + cotangent_[i][j == 0 ? 2 : j - 1] * delta_y_[i][j == 0 ? 2 : j - 1]));
				Coef_Triplet.push_back(Triplet<double>(2 * v_index + 1, 2 * nV + 2 * i + 1,cotangent_[i][j] * delta_x_[i][j] - cotangent_[i][j == 0 ? 2 : j - 1] * delta_x_[i][j == 0 ? 2 : j - 1]));
			}

			else if (v_index == heMesh->Index(point2))
			{
				b(2 * (size_t)nV + 2 * (size_t)i) -= (-cotangent_[i][j] * delta_x_[i][j] + cotangent_[i][j == 0 ? 2 : j - 1] * delta_x_[i][j == 0 ? 2 : j - 1]) * two_points[2];
				b(2 * (size_t)nV + 2 * (size_t)i) -= (-cotangent_[i][j] * delta_y_[i][j] + cotangent_[i][j == 0 ? 2 : j - 1] * delta_y_[i][j == 0 ? 2 : j - 1]) * two_points[3];
				b(2 * (size_t)nV + 2 * (size_t)i + 1) -= (-cotangent_[i][j] * delta_y_[i][j] + cotangent_[i][j == 0 ? 2 : j - 1] * delta_y_[i][j == 0 ? 2 : j - 1]) * two_points[2];
				b(2 * (size_t)nV + 2 * (size_t)i + 1) -= (cotangent_[i][j] * delta_x_[i][j] - cotangent_[i][j == 0 ? 2 : j - 1] * delta_x_[i][j == 0 ? 2 : j - 1]) * two_points[3];
			}
		}

		Coef_Triplet.push_back(Triplet<double>(2 * nV + 2 * i, 2 * nV + 2 * i, value));
		Coef_Triplet.push_back(Triplet<double>(2 * nV + 2 * i + 1, 2 * nV + 2 * i + 1, value));
	}

	for (size_t i = 0; i < nV; i++)
	{
		auto adjedges = heMesh->Vertices()[i]->OutHEs();
		double value = 0;
		if (i == heMesh->Index(point1) || i == heMesh->Index(point2))
		{
			Coef_Triplet.push_back(Triplet<double>(2 * i, 2 * i, 1));
			Coef_Triplet.push_back(Triplet<double>(2 * i + 1, 2 * i + 1, 1));
			b(2 * (size_t)i) = i == heMesh->Index(point1) ? two_points[0] : two_points[2];
			b(2 * (size_t)i + 1) = i == heMesh->Index(point1) ? two_points[1] : two_points[3];
			continue;
		}
         //对左右三角形进行处理
		for (auto adj_v: adjedges)
		{
			double coe_ij = 0;
			auto triangle = adj_v->Polygon();
			size_t t = 0;
			if (triangle != nullptr)
			{
				t = heMesh->Index(triangle);
				int m = 0;
				while (heMesh->Index(triangle->BoundaryVertice()[m]) != i) {
					m++;
				}
				coe_ij += cotangent_[t][m];
			}
			triangle = adj_v->Pair()->Polygon();
			if (triangle != nullptr)
			{
				t = heMesh->Index(triangle);
				int m = 0;
				while (heMesh->Index(triangle->BoundaryVertice()[m]) != i) {
					m++;
				}
				coe_ij += cotangent_[t][m == 0 ? 2 : m - 1];
			}
			// 填充
			value += coe_ij;
			int v_index = (int)(heMesh->Index(adj_v->End()));
			if (v_index != heMesh->Index(point1) && v_index != heMesh->Index(point2))
			{
				Coef_Triplet.push_back(Triplet<double>(i * 2, 2 * v_index, -coe_ij));
				Coef_Triplet.push_back(Triplet<double>(i * 2 + 1, 2 * v_index + 1, -coe_ij));
			}
			else if (v_index == heMesh->Index(point2))
			{
				b((size_t)i * 2) += (coe_ij) * two_points[2];
				b((size_t)i * 2 + 1) += (coe_ij) * two_points[3];
			}
		}

		Coef_Triplet.push_back(Triplet<double>(i * 2, i * 2, value));
		Coef_Triplet.push_back(Triplet<double>(i * 2 + 1, i * 2 + 1, value));
	}

	SparseMatrix<double> Coef_matrix(2 * nV + 2 *nP, 2 * nV + 2 * nP);
	SimplicialLLT<SparseMatrix<double>> solver;
	Coef_matrix.setFromTriplets(Coef_Triplet.begin(), Coef_Triplet.end());
	// 求解
	solver.compute(Coef_matrix);
	VectorXd X = solver.solve(b);

	// 更新点
	for (size_t i = 0; i < nV; i++) {
		if (i != heMesh->Index(point1) && i != heMesh->Index(point2)) {
			pointf2 value = { X(static_cast<Index>(2 * i)), X(static_cast<Index>(2 * i + 1)) };
			heMesh->Vertices()[i]->coord = value.cast_to<vecf2>();
		}
		else if (i == heMesh->Index(point1)) {
			pointf2 value = { two_points[0], two_points[1] };
			heMesh->Vertices()[i]->coord = value.cast_to<vecf2>();
		}
		else {
			pointf2 value = { two_points[2], two_points[3] };
			heMesh->Vertices()[i]->coord = value.cast_to<vecf2>();
		}
	}
}
bool ASAP::Equal_Vec(vecf3 vec1, vecf3 vec2)
{
	return abs(vec1[0] - vec2[0]) < MINI && abs(vec1[1] - vec2[1]) < MINI && abs(vec1[2] - vec2[2]) < MINI;
}

void ASAP::TurnTo2D()
{
	cotangent_.resize(nP);
	vector<vecf2> points2;

	for (auto triangle : heMesh->Polygons()) {
		size_t i = heMesh->Index(triangle);
		points2.clear();
		auto v0 = triangle->BoundaryVertice()[0];
		auto v1 = triangle->BoundaryVertice()[1];
		auto v2 = triangle->BoundaryVertice()[2];
		points2.push_back(vecf2(0, 0));
		points2.push_back(vecf2((v1->pos - v0->pos).norm(), 0));
		float x = (v2->pos - v0->pos).dot(v1->pos - v0->pos) / (v1->pos - v0->pos).norm();
		points2.push_back(vecf2(x, pow(pow((v2->pos - v0->pos).norm(), 2) - pow(x, 2), 0.5)));
		// 获得二维的坐标
		if (Equal_Vec(v0->pos, v1->pos) || Equal_Vec(v1->pos, v2->pos) || Equal_Vec(v2->pos, v0->pos))
		{
			cotangent_[i] = { 1, 1, 1 };
			delta_x_.push_back({ 1, 1, 1 });
			delta_y_.push_back({ 1, 1, 1 });
			continue;
		}

		delta_x_.push_back({ (double)points2[0][0] - points2[1][0], (double)points2[1][0] - points2[2][0], (double)points2[2][0] - points2[0][0] });
		delta_y_.push_back({ (double)points2[0][1] - points2[1][1], (double)points2[1][1] - points2[2][1], (double)points2[2][1] - points2[0][1] });
		
		//获得 cotangent
		for (int j = 0; j < 3; j++) {
			double cos = (points2[j] - points2[j == 0 ? 2 : j - 1]).cos_theta(points2[j == 2 ? 0 : j + 1] - points2[j == 0 ? 2 : j - 1]);
			double sin = sqrt(1 - cos * cos);
			cotangent_[i].push_back(cos > 0 ? (cos / sin) : -(cos / sin)); // store the angle of edge i, i+1. idx is V
		}
	}
}