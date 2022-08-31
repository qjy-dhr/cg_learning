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
		bool Run();			                                     // 参数化，改变纹理坐标，用于显示mesh
		bool GetMesh();		                                     // 参数化，并改变三角网格定点坐标，展示参数化结果

	private:
		class V;                                                 //模仿极小曲面实现
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
		void setBoundary();				                        // 将边界映射到正方形边界或圆边界
		void setWeightValue(V* vertex, std::vector<double>& weight_list_, double& sum);

	private:
		int shape_type = 0;					                    // 0->circle，1->square,
		int weight_type = 0;			                        // 1->uniform，0->cotangent, 
		Ptr<TriMesh> triMesh;
		Ptr<HEMesh<V>> heMesh;                                  // heMesh 与 triMesh同步
	};
}
