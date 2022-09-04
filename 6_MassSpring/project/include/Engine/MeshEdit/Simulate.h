#pragma once

#include <Basic/HeapObj.h>
//#include <Engine/Primitive/MassSpring.h>
#include <UGM/UGM>
#include <Eigen/Sparse>

using namespace Eigen;

namespace Ubpa {
	enum class SimulateType
	{
		ImplicitEuler,             //模拟类型，欧拉隐式方法
		ProjectiveDynamic,         //加速方法
	};
	class Simulate : public HeapObj {
	public:
		Simulate(const std::vector<pointf3>& plist,
			const std::vector<unsigned>& elist) {
			edgelist = elist;
			this->positions.resize(plist.size());
			for (int i = 0; i < plist.size(); i++)
			{
				for (int j = 0; j < 3; j++)
				{
					this->positions[i][j] = plist[i][j];
				}
			}
		};
	public:
		static const Ptr<Simulate> New(const std::vector<pointf3>& plist,
			const std::vector<unsigned> &elist) {
			return Ubpa::New<Simulate>(plist, elist);
		}
	public:
		// clear cache data
		void Clear();

		// init cache data (eg. half-edge structure) for Run()
		//bool ReInit();
		bool Init();

		// call it after Init()
		bool Run();
		
		const std::vector<pointf3>& GetPositions() const { return positions; };

		const float GetStiff() { return stiff; };
		void SetStiff(float k) { stiff = k; Init();};
		const float GetTimeStep() { return h; };
		void SetTimeStep(float k) { h = k; Init();};
		std::vector<unsigned>& GetFix() { return this->fixed_id; };
		void SetFix(const std::vector<unsigned>& f) { this->fixed_id = f; Init();};
		const std::vector<pointf3>& GetVelocity() { return velocity; };
		//void SetVelocity(const std::vector<pointf3>& v) { velocity = v; };

		pointf3& GetGravity() { return gravity; };

		void SetLeftFix(); //固定最左面
		void SetUpLRFix(); // 固定上面左右两个点
		void SetUpFix(); // fix the points on the upper


		void SetSimulateType(SimulateType type);                     //选择模拟方法

		//bool Isfixed(size_t x);

	private:
		// kernel part of the algorithm
		void SimulateOnce();

		void ImplicitEuler();
		void ProjectiveDynamic();


	private:
		float h = 0.03f;                                             //步长
		float stiff= 200.f;                                          //弹性系数;
		float M = 1.0f;                                              //结点质量
		Ubpa::pointf3 gravity= pointf3(0.f, 0.f, 0.f);               //重力
		std::vector<unsigned> fixed_id;  //fixed point id

		//mesh data
		std::vector<unsigned> edgelist;
		std::vector<std::list<size_t>> linkedlist;

		//simulation data
		std::vector<pointf3> positions;
		std::vector<pointf3> velocity;

		std::vector<bool> isFixed;

		std::vector<double> original_length;                         //弹簧原长

		SimulateType simulate_type = SimulateType::ImplicitEuler;    //默认方法为欧拉

		SparseMatrix<float> KM_hhL;
		SparseMatrix<float> J;

		MatrixX3f	d;                                                //迭代数据d
		MatrixX3f	Phi;
		SparseLU<SparseMatrix<float>> A_ReLU;
			
		size_t nE ;
		size_t nV ;
	};
}
