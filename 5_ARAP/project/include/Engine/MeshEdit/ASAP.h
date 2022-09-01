#pragma once

#include <Basic/HeapObj.h>
#include <UHEMesh/HEMesh.h>
#include <UGM/UGM>
#include <vector>
#define MINI 1e-6

namespace Ubpa {
	class TriMesh;
	class MinSurf;

	// mesh boundary == 1
	class ASAP : public HeapObj {
	public:
		ASAP(Ptr<TriMesh> triMesh);
	public:
		static const Ptr<ASAP> New(Ptr<TriMesh> triMesh) {
			return Ubpa::New<ASAP>(triMesh);
		}
	public:
		void Clear();
		bool Init(Ptr<TriMesh> triMesh);

		bool Run();		                                         // ���������ı��������꣬������ʾmesh
		bool GetMesh();		                                     // �����������ı��������񶨵����꣬չʾ���������

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
		class P : public TPolygon<V, E, P> { };

	private:
		void getParamaterize();                                   //��������������
		void TurnTo2D();                                          //���ش�3Dת��2D
		bool Equal_Vec(vecf3 vec1, vecf3 vec2);

	private:
		Ptr<TriMesh> triMesh;
		const Ptr<HEMesh<V>> heMesh;	 // vertice order is same with triMesh
		
		size_t nP, nV;
		std::vector<std::vector<double>> cotangent_;
		std::vector<std::vector<double>> delta_x_;
		std::vector<std::vector<double>> delta_y_;
	};
}