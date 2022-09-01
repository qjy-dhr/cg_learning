# homework 5. ARAP参数化 😢

#### 1. 要求

- 在给定的网格框架上完成作业，实现
  - ASAP (As-similar-as-possible) 参数化算法
  - ARAP (As-rigid-as-possible) 参数化算法
  - Hybrid 参数化方法（可选）
- 对各种参数化方法（包括作业4的Floater方法、ASAP/ARAP方法等）进行比较
- 继续学习和巩固三角网格的数据结构及编程
- 学习和实现矩阵的 SVD 分解
- 进一步巩固使用 Eigen 库求解大型稀疏线性方程组



#### 2. 实现

- 使用求解稀疏方程组的方法实现的ASAP参数化；
- 以ASAP参数化为初值，使用Local/Global实现ARAP参数化；
- 相关纹理映射的显示

#### 3. 算法原理笔记

##### 3.1 不固定边界参数化

​    对于网格中每个三角形，都有一个自己局部的等距参数化𝑥𝑡 = {𝑥𝑡0, 𝑥𝑡1, 𝑥𝑡2}，在原网格三角形与原网格参数化后的三角形之间存在唯一的线性映射，即 Jacobian 矩阵。为了衡量参数化对图形的扭曲程度，我们可以引入辅助线性变换 Lt，并定义能量函数：

𝐸(𝑢, 𝐿) = ∑𝐴𝑡‖𝐽𝑡(𝑢) − 𝐿𝑡‖?

即不固定边界参数化的过程就是找到合适的 （u,L）, 满足

(𝑢, 𝐿) = arg min(𝑢,𝐿) 𝐸(𝑢, 𝐿)

```
u：参数化后的集合𝑢 = {𝑢1, … , 𝑢𝑡}

L：辅助线性变换，如旋转、仿射。
```

显然，对于不同的L，求解方法不同。

##### 3.2  As Similar As Possible (ASAP) 算法

​    该方法保持了参数化后角度的不变性，尽可能的的保角，因此这里我们所期望所在的线性变换族尽可能接近相似变换 𝐿选取[ 𝑎    𝑏  

​                                                                                     −𝑏    𝑎]

​    ASAP采用求解稀疏方程组的方法，偏导数等于0。

​    其中矩阵大小为2(nV + nP) 阶，其中nV为mesh中三角形的数量。前 

2nV行存放ux,uy的方程，后2nP行存放a,b的方程。

​    论文中指出，ASAP 可能得到平凡解，要避免平凡解的出现，需要选取至少俩锚点，要选取相隔距离尽可能远的两个边界上的点作为锚点。

##### 3.3 As Rigid As Possible (ARAP)算法

​    该方法最大化的保持参数化后图形的刚性，尽可能的的保形。因此这里限制𝐿只属于旋转变换，𝐿选取[ cos 𝜃   sin 𝜃 

​                                               − sin 𝜃   cos𝜃]: 𝜃 ∈ [0,2𝜋)

因为方程是非线性的，不能用求偏导的方式，因此ARAP采用了"Local/Global"的迭代方法。

具体步骤如下

1. 初始化

复用ASAP的映射函数，映射方法：3D-2D

原三角形为x0,x1,x2,∠x1x0x2=θ,则映射后：u0=（0，0，0）,u1=（x0x1,0,0）,u2=（x0x2cosθ,x0x2sinθ,0）

2. Local 阶段，固定𝑢，求𝐿𝑡；

对Jt进行SVD分解，有*J* =UDV*T*，det(UV*T*) 圩 *>* 地

   3.在 Global 阶段，固定𝐿𝑡，求𝑢

如此反复迭代，最终收敛。

#### 4. 实现效果

Mesh

| name     | Init                                                         | ASAP Mesh                                                    | ARAP Mesh (10)                                               |
| -------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| Beetle   | ![init](D:\ustc\homework\cg_learning\5_ARAP\picture\Beetle_ABF\init.png) | ![ASAPMesh](D:\ustc\homework\cg_learning\5_ARAP\picture\Beetle_ABF\ASAPMesh.png) | ![ARAPMesh](D:\ustc\homework\cg_learning\5_ARAP\picture\Beetle_ABF\ARAPMesh.png) |
| Cow      | ![init](D:\ustc\homework\cg_learning\5_ARAP\picture\Cow_dABF\init.png) | ![ASAPMesh](D:\ustc\homework\cg_learning\5_ARAP\picture\Cow_dABF\ASAPMesh.png) | ![ARAPMesh](D:\ustc\homework\cg_learning\5_ARAP\picture\Cow_dABF\ARAPMesh.png) |
| Gargoyle | ![init](D:\ustc\homework\cg_learning\5_ARAP\picture\Gargoyle_ABF\init.jpg) | ![ASAPMesh ](D:\ustc\homework\cg_learning\5_ARAP\picture\Gargoyle_ABF\ASAPMesh .png) | ![ARAPMesh](D:\ustc\homework\cg_learning\5_ARAP\picture\Gargoyle_ABF\ARAPMesh.png) |
| Isis     | ![init](D:\ustc\homework\cg_learning\5_ARAP\picture\Isis_dABF\init.png) | ![ASAPMesh](D:\ustc\homework\cg_learning\5_ARAP\picture\Isis_dABF\ASAPMesh.png) | ![ARAPMesh](D:\ustc\homework\cg_learning\5_ARAP\picture\Isis_dABF\ARAPMesh.png) |



| name     | CC                                                           | SC                                                           | ASAP                                                         | ARAP(15)                                                     |
| :------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| Beetle   | ![CC](D:\ustc\homework\cg_learning\5_ARAP\picture\Beetle_ABF\CC.png) | ![SC](D:\ustc\homework\cg_learning\5_ARAP\picture\Beetle_ABF\SC.png) | ![ASAP](D:\ustc\homework\cg_learning\5_ARAP\picture\Beetle_ABF\ASAP.png) | ![ARAP_15](D:\ustc\homework\cg_learning\5_ARAP\picture\Beetle_ABF\ARAP_15.png) |
| Cow      | ![CC](D:\ustc\homework\cg_learning\5_ARAP\picture\Cow_dABF\CC.png) | ![SC](D:\ustc\homework\cg_learning\5_ARAP\picture\Cow_dABF\SC.png) | ![ASAP](D:\ustc\homework\cg_learning\5_ARAP\picture\Cow_dABF\ASAP.png) | ![ARAP_15](D:\ustc\homework\cg_learning\5_ARAP\picture\Cow_dABF\ARAP_15.png) |
| Gargoyle | ![CC](D:\ustc\homework\cg_learning\5_ARAP\picture\Gargoyle_ABF\CC.png) | ![SC](D:\ustc\homework\cg_learning\5_ARAP\picture\Gargoyle_ABF\SC.png) | ![ASAP](D:\ustc\homework\cg_learning\5_ARAP\picture\Gargoyle_ABF\ASAP.jpg) | ![ARAP_15](D:\ustc\homework\cg_learning\5_ARAP\picture\Gargoyle_ABF\ARAP_15.png) |
| Isis     | ![CC](D:\ustc\homework\cg_learning\5_ARAP\picture\Isis_dABF\CC.png) | ![SC](D:\ustc\homework\cg_learning\5_ARAP\picture\Isis_dABF\SC.png) | ![ASAP](D:\ustc\homework\cg_learning\5_ARAP\picture\Isis_dABF\ASAP.png) | ![ARAP_15](D:\ustc\homework\cg_learning\5_ARAP\picture\Isis_dABF\ARAP_15.png) |



| Iteration times | 5                                                            | 15                                                           | 30                                                           | 50                                                           |
| --------------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| Beetle          | ![ARAP_5](D:\ustc\homework\cg_learning\5_ARAP\picture\Beetle_ABF\ARAP_5.png) | ![ARAP_15](D:\ustc\homework\cg_learning\5_ARAP\picture\Beetle_ABF\ARAP_15.png) | ![ARAP_30](D:\ustc\homework\cg_learning\5_ARAP\picture\Beetle_ABF\ARAP_30.png) | ![ARAP_50](D:\ustc\homework\cg_learning\5_ARAP\picture\Beetle_ABF\ARAP_50.png) |
| Cow             | ![ARAP_5](D:\ustc\homework\cg_learning\5_ARAP\picture\Cow_dABF\ARAP_5.png) | ![ARAP_15](D:\ustc\homework\cg_learning\5_ARAP\picture\Cow_dABF\ARAP_15.png) | ![ARAP_30](D:\ustc\homework\cg_learning\5_ARAP\picture\Cow_dABF\ARAP_30.png) | ![ARAP_50](D:\ustc\homework\cg_learning\5_ARAP\picture\Cow_dABF\ARAP_50.png) |
| Gargoyle        | ![ARAP_5](D:\ustc\homework\cg_learning\5_ARAP\picture\Gargoyle_ABF\ARAP_5.png) | ![ARAP_15](D:\ustc\homework\cg_learning\5_ARAP\picture\Gargoyle_ABF\ARAP_15.png) | ![ARAP_30](D:\ustc\homework\cg_learning\5_ARAP\picture\Gargoyle_ABF\ARAP_30.png) | ![ARAP_50](D:\ustc\homework\cg_learning\5_ARAP\picture\Gargoyle_ABF\ARAP_50.png) |
| Isis            | ![ARAP_5](D:\ustc\homework\cg_learning\5_ARAP\picture\Isis_dABF\ARAP_5.png) | ![ARAP_15](D:\ustc\homework\cg_learning\5_ARAP\picture\Isis_dABF\ARAP_15.png) | ![ARAP_30](D:\ustc\homework\cg_learning\5_ARAP\picture\Isis_dABF\ARAP_30.png) | ![ARAP_50](D:\ustc\homework\cg_learning\5_ARAP\picture\Isis_dABF\ARAP_50.png) |

##### 不足

​    代码比较混乱，想到的能改进的点有，1.可以抽象出一个初始化的基类，用于所有参数化方法的初始化。2.采用三角形网格结构进行计算.......或许以后会来填坑。

时间 9.1-9.4

