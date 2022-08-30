# homework 4. 极小曲面和网格参数化👍

#### 要求

- 初步了解 \*.obj 格式的 3D 数据（\*.obj, \*.mtl)
  - 使用 Win10 自带的 “3D 查看器” 查看 obj 数据文件
  - 安装并使用开源3D几何处理框架 [MeshLab](http://www.meshlab.net) 查看 3D 数据文件 （MeshLab 功能多，可多学习使用）
- 学习三角网格的数据结构及操作
  - 使用 UEngine 网格程序框架
  - 寻找**非封闭**网格曲面的边界
- 实现极小曲面与网格参数化
  - 极小曲面：边界固定，求解稀疏方程组
  - 参数化：边界映射到平面凸多边形，求解稀疏方程组
- 巩固使用 Eigen 库求解稀疏线性方程组

#### 实现

在UEngine中添加功能：

- 求给定边界的极小曲面；
- 非封闭网格曲面的参数化（边界圆形或方形，权重选取）；
- 显示纹理映射。 

#### 具体思路：

##### 极小曲面 

三角网格顶点vi的微分坐标为

vi *−* ∑ j**∈**N*(*i) ωjvj

N(i)：和vi邻接的所有的顶点。

ωj：权重。在求极小曲面的时候取了1/di的权重。

di：顶点vi的度。

- 先确定一个边界上空间坐标或纹理坐标， 然后通过Laplace坐标构建求解稀疏方程组，得到空间坐标或纹理坐标。

##### 网格参数化同理（区别边界形状）

#### 实现效果

##### ball 效果

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/4_MinSurfMeshPara/picture/ball/ball-3d.png" alt="ball-3d" width="10%" />

pic1:init

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/4_MinSurfMeshPara/picture/ball/ball-surf.jpg" alt="ball-surf" width="10%" />

pic2:minisurf

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/4_MinSurfMeshPara/picture/ball/cc-par.png" alt="cc-par" width="10%" />

pic3:circle-cotangent 参数化

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/4_MinSurfMeshPara/picture/ball/cc-ball.png" alt="cc-ball" width="10%" />

pic4:circle-cotangent 纹理映射

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/4_MinSurfMeshPara/picture/ball/cu-par.png" alt="cu-par" width="10%" />

pic5:circle-uniform 参数化

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/4_MinSurfMeshPara/picture/ball/cu-ball.png" alt="cu-ball" width="10%" />

pic6:circle-uniform 纹理映射

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/4_MinSurfMeshPara/picture/ball/sc-par.png" alt="sc-par" width="10%" />

pic7:square-uniform 参数化

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/4_MinSurfMeshPara/picture/ball/sc-ball.png" alt="sc-ball" width="10%" />

pic8:square-uniform 纹理映射

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/4_MinSurfMeshPara/picture/ball/su-par.png" alt="su-par" width="10%" />

pic9:square-cotangent 参数化

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/4_MinSurfMeshPara/picture/ball/su-ball.png" alt="su-ball" width="10%" />

pic10 :square-cotangent 纹理映射

------

##### rabit 效果

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/4_MinSurfMeshPara/picture/rabit/rabit-3d.png" alt="rabit-3d" width="10%" />

pic1:init

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/4_MinSurfMeshPara/picture/rabit/cc-par-rabit.png" alt="rabit-surf" width="10%" />

pic2:minisurf

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/4_MinSurfMeshPara/picture/rabit/cc-par-rabit.png" alt="cc-par-rabit" width="10%" />

pic3:circle-cotangent 参数化

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/4_MinSurfMeshPara/picture/rabit/cc-rabit.png" alt="cc-rabit" width="10%" />

pic4:circle-cotangent 纹理映射

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/4_MinSurfMeshPara/picture/rabit/su-par-rabit.png" alt="su-par-rabit" width="10%" />

pic5:square-uniform 参数化

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/4_MinSurfMeshPara/picture/rabit/su-rabit.png" alt="su-rabit" width="10%" />

pic6:square-uniform 纹理映射

------

##### face 效果

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/4_MinSurfMeshPara/picture/face/face-3d.png" alt="face-3d" width="10%" />

pic1:init

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/4_MinSurfMeshPara/picture/face/face-surf.png" alt="face-surf" width="10%" />

pic2:minisurf

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/4_MinSurfMeshPara/picture/face/cc-par-face.png" alt="cc-par-face" width="10%" />

pic3:circle-cotangent 参数化

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/4_MinSurfMeshPara/picture/face/su-par-face.png" alt="su-par-face" width="10%" />

pic4:square-uniform 参数化

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/4_MinSurfMeshPara/picture/face/face-par.png" alt="face-par" width="10%" />

pic5:纹理映射。p.s.一点小尝试，效果很吓人。😢

