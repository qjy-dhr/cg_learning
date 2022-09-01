#pragma once

#include <Basic/HeapObj.h>
#include <UHEMesh/HEMesh.h>
#include <UGM/UGM>

#include <Engine/MeshEdit/ASAP.h>
#include <Engine/Primitive/TriMesh.h>
#include <Engine/MeshEdit/Paramaterize.h>
#include <Eigen/Sparse>

#include <Eigen/Dense>
#include <Eigen/Sparse>

namespace Ubpa {
	class TriMesh;
	class MinSurf;

	// mesh boundary == 1
	class ARAP : public HeapObj {
	public:
		ARAP(Ptr<TriMesh> triMesh);
	public:
		static const Ptr<ARAP> New(Ptr<TriMesh> triMesh) {
			return Ubpa::New<ARAP>(triMesh);
		}
	public:
		void Clear();
		bool Init(Ptr<TriMesh> triMesh);

		bool Run();			
		bool getMesh();		
		void SetIteration(int time);

	private:
		class V;
		class E;
		class P;
		class V : public TVertex<V, E, P> {
		public:
			vecf2 coord;
			vecf3 pos;
		};
		class E : public TEdge<V, E, P> { };
		class P : public TPolygon<V, E, P> {
		public:
			Eigen::Matrix2d L;
			std::vector<vecf2> x_flatten; 
			std::vector<double> cot;	// (i,i+1)
		};

	private:
		void getParamaterize();
		void TurnTo2D();
		void LocalSetUSolveL();
		void GlobalSetLSolveU(Eigen::SimplicialLLT<Eigen::SparseMatrix<double>> &solver);
		
		void GlobalMatrixA(Eigen::SparseMatrix<double> &A);
		bool Equal_Vec(vecf3 vec1, vecf3 vec2);
		
	private:
		size_t nP, nV;
		Ptr<TriMesh> triMesh;
		const Ptr<HEMesh<V>> heMesh;	// vertice order is same with triMesh
		size_t iteration_times;
		std::vector<size_t> fixed_vertices_;  //在迭代中，求解u的系数矩阵是固定不变的，所以只需要对矩阵进行一次分解，后面迭代可以复用。
		std::vector<vecf2> fixed_coords_;
	};
}