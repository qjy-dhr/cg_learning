#include <Engine/MeshEdit/Paramaterize.h>

#include <Engine/MeshEdit/MinSurf.h>

#include <Engine/Primitive/TriMesh.h>

#include <Eigen/Sparse>

using namespace Ubpa;

using namespace std;

using namespace Eigen;

Paramaterize::Paramaterize(Ptr<TriMesh> triMesh)
	: heMesh(make_shared<HEMesh<V>>())
{
	Init(triMesh);
}


void Paramaterize::Clear() {
	
	// TODO
	heMesh->Clear();
	triMesh = nullptr;
}

bool Paramaterize::Init(Ptr<TriMesh> triMesh) {

	// TODO
	Clear();

	if (triMesh == nullptr)
		return true;

	if (triMesh->GetType() == TriMesh::INVALID) {
		printf("ERROR::Paramaterize::Init:\n"
			"\t""trimesh is invalid\n");
		return false;
	}

	// init half-edge structure
	size_t nV = triMesh->GetPositions().size();
	vector<vector<size_t>> triangles;
	triangles.reserve(triMesh->GetTriangles().size());
	for (auto triangle : triMesh->GetTriangles())
		triangles.push_back({ triangle->idx[0], triangle->idx[1], triangle->idx[2] });
	heMesh->Reserve(nV);
	heMesh->Init(triangles);
	//1.���߽�
	if (!heMesh->IsTriMesh() || !heMesh->HaveBoundary()) {
		printf("ERROR::Paramaterize::Init:\n"
			"\t""trimesh is not a triangle mesh or hasn't a boundary\n");
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

bool Paramaterize::Run() {
	if (heMesh->IsEmpty() || !triMesh) {
		printf("ERROR::Paramaterize::Run\n"
			"\t""heMesh->IsEmpty() || !triMesh\n");
		return false;
	}

	getParamaterize();

	// half-edge structure -> triangle mesh
	size_t nV = heMesh->NumVertices();
	size_t nF = heMesh->NumPolygons();

	vector<pointf2> texcoords;
	for (auto v : heMesh->Vertices())
		texcoords.push_back(v->coord.cast_to<pointf2>());
	triMesh->Update(texcoords);

	cout << "Parameterize done" << endl;
	return true;
}

bool Paramaterize::GetMesh()
{
	if (heMesh->IsEmpty() || !triMesh) {
		printf("ERROR::Paramaterize::Show\n"
			"\t""heMesh->IsEmpty() || !triMesh\n");
		return false;
	}

	getParamaterize();

	// 6.��������ͼ�񣬸�����ʾ
	// half-edge structure -> triangle mesh
	size_t nV = heMesh->NumVertices();
	size_t nF = heMesh->NumPolygons();
	vector<pointf3> positions;
	vector<unsigned> indice;
	positions.reserve(nV);
	indice.reserve(3 * nF);

	// Set positions
	for (auto v : heMesh->Vertices()) {
		if (shape_type == 0 && v->Degree() == 2 && v->IsBoundary()) {
			// change the position of some special vertices
			if (abs(v->coord[0] - 1) < 1e-6)
				v->coord[0] += 0.01f;
			else if (abs(v->coord[0]) < 1e-6)
				v->coord[0] -= 0.01f;
			else if (abs(v->coord[1] - 1) < 1e-6)
				v->coord[1] += 0.01f;
			else if (abs(v->coord[1]) < 1e-6)
				v->coord[1] -= 0.01f;
		}

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

	cout << "Parameterize done" << endl;
	return true;
}
void Paramaterize::getParamaterize()
{
	// 2.���߽�ӳ�䵽�����α߽��Բ�߽�
	setBoundary();

	// 3.����ϡ�跽����
	std::vector<Triplet<double>> Laplace;
	size_t nV = heMesh->NumVertices();
	VectorXd b_x = VectorXd::Zero(nV);
	VectorXd b_y = VectorXd::Zero(nV);

	for (auto v : heMesh->Vertices()) {
		int i = static_cast<int>(heMesh->Index(v));

		if (v->IsBoundary()) {
			Laplace.push_back(Triplet<double>(i, i, 1));
			b_x(i) += (v->coord[0]);
			b_y(i) += (v->coord[1]);
			continue;
		}

		vector<double> weight_list_;
		double sum = 0;
		setWeightValue(v, weight_list_, sum);
		Laplace.push_back(Triplet<double>(i, i, sum));

		for (int j = 0; j < weight_list_.size(); j++) {
			auto neighbour = v->AdjVertices()[j];
			if (neighbour->IsBoundary()) {
				b_x(i) += weight_list_[j] * (neighbour->coord[0]);
				b_y(i) += weight_list_[j] * (neighbour->coord[1]);
			}
			else {
				int i_neighbour = static_cast<int>(heMesh->Index(neighbour));
				Laplace.push_back(Triplet<double>(i, i_neighbour, -weight_list_[j]));
			}
		}
	}

	// 4.���ϡ�跽����
	SparseMatrix<double> A(nV, nV);
	SimplicialLLT<SparseMatrix<double>> solver;
	A.setFromTriplets(Laplace.begin(), Laplace.end());
	solver.compute(A);

	VectorXd Vx = solver.solve(b_x);
	VectorXd Vy = solver.solve(b_y);

	// 5.���¶�������
	for (auto v : heMesh->Vertices()) {
		size_t i = heMesh->Index(v);
		pointf2 tmp = { Vx(i), Vy(i) };
		v->coord = tmp.cast_to<vecf2>();
	}
}

void Paramaterize::setBoundary()
{
	auto boundaries = heMesh->Boundaries()[0];
	float theta = 0;
	float dtheta = 360 / static_cast<float>(boundaries.size());

	switch (shape_type)
	{
	case 0:
		// Circle
		for (auto he : boundaries) {
			auto v = he->Origin();
			pointf2 tmp = { 0.5 + 0.5 * cos(theta * PI<double> / 180.), 0.5 + 0.5 * sin(theta * PI<double> / 180.) };
			v->coord = tmp.cast_to<vecf2>();
			theta += dtheta;
		}
		break;
	case 1:
		// Square
		theta = -45;
		for (auto he : boundaries) {
			auto v = he->Origin();
			if (theta >= -45 && theta < 45) {
				pointf2 tmp = { 1, 0.5 * tan(theta * PI<double> / 180.) + 0.5 };
				v->coord = tmp.cast_to<vecf2>();
			}
			else if (theta >= 45 && theta < 135) {
				pointf2 tmp = { 0.5 / tan(theta * PI<double> / 180.) + 0.5, 1 };
				v->coord = tmp.cast_to<vecf2>();
			}
			else if (theta >= 135 && theta < 225) {
				pointf2 tmp = { 0, -0.5 * tan(theta * PI<double> / 180.) + 0.5 };
				v->coord = tmp.cast_to<vecf2>();
			}
			else if (theta >= 225 && theta < 315) {
				pointf2 tmp = { -0.5 / tan(theta * PI<double> / 180.) + 0.5, 0 };
				v->coord = tmp.cast_to<vecf2>();
			}
			theta += dtheta;
		}
		break;
	default:
		break;
	}
}

bool Paramaterize::setShape(int type)
{
	shape_type = type;
	return true;
}

bool Paramaterize::setWeight(int type)
{
	weight_type = type;
	return true;
}

void Paramaterize::setWeightValue(V* vertex, vector<double>& weight_list_, double& sum)
{
	auto adjVertice_list_ = vertex->AdjVertices();
	sum = 0;

	switch (weight_type)
	{
	case 0:
		// Normal
		for (int i = 0; i < adjVertice_list_.size(); i++) {
			weight_list_.push_back(1);
		}
		sum = (double)vertex->Degree();
		break;
	case 1:
		// Cotangent
		for (int i = 0; i < adjVertice_list_.size(); i++) {
			auto v0 = adjVertice_list_[i];
			auto v1 = adjVertice_list_[i - 1 < 0 ? (int)adjVertice_list_.size() - 1 : i - 1];
			auto v2 = adjVertice_list_[i + 1 >= (int)adjVertice_list_.size() ? 0 : i + 1];

			double cos_alpha = vecf3::cos_theta(v0->pos - v1->pos, vertex->pos - v1->pos);
			double cos_beta = vecf3::cos_theta(v0->pos - v2->pos, vertex->pos - v2->pos);

			double tmp = pow(cos_alpha * cos_alpha / (1 - cos_alpha * cos_alpha), 0.5);
			tmp += pow(cos_beta * cos_beta / (1 - cos_beta * cos_beta), 0.5);
			weight_list_.push_back(tmp);
			sum += tmp;
		}
		break;
	default:
		break;
	}
}