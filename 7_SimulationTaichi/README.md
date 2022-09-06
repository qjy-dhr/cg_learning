

# homework 7. 基于 Taichi 的仿真



#### 1. 要求

- 构建不同的模拟场景★
  - 使用 `add_objec` 函数设计自己的模拟场景（可以改变模拟物体的形状、粒子数目、初始位置、初始速度等等）
- 探究参数空间：通过调整不同的参数设置体会不同参数对仿真结果的影响★★
  - 比如：探究杨氏模量E对雪模拟的影响，结合其背后的物理原理进行比较分析
- 探究其他算法和模型：通过改变物理模型、改变模拟算法实现新的模拟结果★★★
  - 比如：水冲击弹性物体的模拟
- 渲染与绘制（可选 ★★★★★）
  - 学习使用商业软件（如Houdini、Blender等）对结果进行渲染
  - 【注：3D粒子更合适光照渲染，因此此处不作要求】

#### 2. 实现

##### 2.1 模拟场景

###### 2.1.1 物体的形状

<img src="https://github.com/qjy-dhr/cg_learning/tree/main/7_SimulationTaichi/picture/image-20220905225108351.png" alt="image-20220905225108351" width="45%" />       <img src="https://github.com/qjy-dhr/cg_learning/tree/main/7_SimulationTaichi/picture/image-20220905225141531.png" alt="image-20220905225141531" width="45%" />

###### 2.1.2 粒子数目

n=5000                                                    n=10000                                               n=20000

<img src="https://github.com/qjy-dhr/cg_learning/tree/main/7_SimulationTaichi/picture/image-20220906160616363.png" alt="image-20220906160616363" width="33%" /><img src="https://github.com/qjy-dhr/cg_learning/tree/main/7_SimulationTaichi/picture/image-20220906160138383.png" alt="image-20220906160138383" width="33%" /><img src="https://github.com/qjy-dhr/cg_learning/tree/main/7_SimulationTaichi/picture/image-20220906160307090.png" alt="image-20220906160307090" width="33%" />

粒子数量越多，相同时间内可射入深度越小，当粒子越大时，会无限接近刚体。

###### 2.1.3 初始位置

h=0.65                                                    h=0.85                                                    h=0.45

<img src="https://github.com/qjy-dhr/cg_learning/tree/main/7_SimulationTaichi/picture/image-20220906155541321.png" alt="image-20220906155541321" width="33%" /><img src="https://github.com/qjy-dhr/cg_learning/tree/main/7_SimulationTaichi/picture/image-20220906155257890.png" alt="image-20220906155257890" width="33%" /><img src="https://github.com/qjy-dhr/cg_learning/tree/main/7_SimulationTaichi/picture/image-20220906155427224.png" alt="image-20220906155427224" width="33%" />

<img src="https://github.com/qjy-dhr/cg_learning/tree/main/7_SimulationTaichi/picture/image-20220906155607162.png" alt="image-20220906155607162" width="33%" /><img src="https://github.com/qjy-dhr/cg_learning/tree/main/7_SimulationTaichi/picture/image-20220906155330482.png" alt="image-20220906155330482" width="33%" /><img src="https://github.com/qjy-dhr/cg_learning/tree/main/7_SimulationTaichi/picture/image-20220906155501597.png" alt="image-20220906155501597" width="33%" />

###### 2.1.4 初始速度



##### 2.2 参数空间

###### 2.2.1 杨氏模量 E

E=1e3                                                     E=1e4                                                    E=5e4

<img src="https://github.com/qjy-dhr/cg_learning/tree/main/7_SimulationTaichi/picture/E1.jpg" alt="E1" width="33%" /><img src="https://github.com/qjy-dhr/cg_learning/tree/main/7_SimulationTaichi/picture/E2.png" alt="E2" width="33%" /><img src="https://github.com/qjy-dhr/cg_learning/tree/main/7_SimulationTaichi/picture/E3.png" alt="E3" width="33%" />

*E* *=* *σ*/*ε*，其中，*E* 表示杨氏模量，*σ* 表示正向应力，*ε* 表示正向应变。杨氏模量越大，在受到相同应力情况下，材料形变越小。雪块碰撞地面不会发生弹起，当杨氏模量较小时，内部弹力很小，直接被压扁。当杨氏模量稍大时，雪块会碎开。继续增大杨氏模量，当雪块在空中直接爆炸开。

###### 2.2.2 泊松比 nu

nu=0.1                                                    nu=0.3                                                  nu=0.4

<img src="https://github.com/qjy-dhr/cg_learning/tree/main/7_SimulationTaichi/picture/image-20220905220640391.png" alt="image-20220905220640391" width="33%" /><img src="https://github.com/qjy-dhr/cg_learning/tree/main/7_SimulationTaichi/picture/image-20220905220527128.png" alt="image-20220905220527128" width="33%" /><img src="https://github.com/qjy-dhr/cg_learning/tree/main/7_SimulationTaichi/picture/image-20220905220449693.png" alt="image-20220905220449693" width="33%" />

泊松比越大，物品内部粘连层度越好，内力越大。

##### 2.3 新的模拟结果

- 两股水流+两个弹性物体

<img src="https://github.com/qjy-dhr/cg_learning/tree/main/7_SimulationTaichi/picture/image-20220906162657984.png" alt="image-20220906162657984" width="45%" /><img src="https://github.com/qjy-dhr/cg_learning/tree/main/7_SimulationTaichi/picture/image-20220906162811055.png" alt="image-20220906162811055" width="45%" />

<img src="https://github.com/qjy-dhr/cg_learning/tree/main/7_SimulationTaichi/picture/image-20220906162913497.png" alt="image-20220906162913497" width="45%" /><img src="https://github.com/qjy-dhr/cg_learning/tree/main/7_SimulationTaichi/picture/image-20220906163105675.png" alt="image-20220906163105675" width="45%" />

时间：9.6-9.7
