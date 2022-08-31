#pragma once

#include <Basic/HeapObj.h>
#include <UHEMesh/HEMesh.h>
#include <UGM/UGM>

namespace Ubpa {
	class TriMesh;
	class MinSurf;

	// mesh boundary == 1
	class Paramaterize : public HeapObj {
	public:
		Paramaterize(Ptr<TriMesh> triMesh);
	public:
		static const Ptr<Paramaterize> New(Ptr<TriMesh> triMesh) {
			return Ubpa::New<Paramaterize>(triMesh);
		}
	public:
		void Clear();
		bool Init(Ptr<TriMesh> triMesh);

		bool setShape(int type);
		bool setWeight(int type);
		bool Run();			                                     // ���������ı��������꣬������ʾmesh
		bool GetMesh();		                                     // �����������ı��������񶨵����꣬չʾ���������

	private:
		class V;                                                 //ģ�¼�С����ʵ��
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
		void getParamaterize();
		void setBoundary();				                        // ���߽�ӳ�䵽�����α߽��Բ�߽�
		void setWeightValue(V* vertex, std::vector<double>& weight_list_, double& sum);

	private:
		int shape_type = 0;					                    // 0->circle��1->square,
		int weight_type = 0;			                        // 1->uniform��0->cotangent, 
		Ptr<TriMesh> triMesh;
		Ptr<HEMesh<V>> heMesh;                                  // heMesh �� triMeshͬ��
	};
}
