#  homework 6. 质点弹簧系统仿真

#### 1. 要求

- 在给定的网格框架上完成作业，实现
  - 质点弹簧仿真模型的欧拉隐式方法
  - 质点弹簧仿真模型的加速模拟方法
- 学习使用 Tetgen 库生成四面体剖分

#### 2. 实现

- 这次作业我实现了两种对弹簧质点模型做仿真的方法：隐式欧拉法和老师给的加速的方法。
- 增加了一些UI 的交互使操作更加方便。

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/6_MassSpring/picture/1.png" alt="image-20220904160429468" width="33%" />

#### 3. 原理

##### 3.1 隐式欧拉法

  隐式欧拉法区分于显式欧拉法(后者更符合直觉)，它使用下一步的变量值来更新下一步的变量值，使用牛顿法求解。在交互中，固定的点的方法是将其外力设为0。

##### 3.2 加速方法

  隐式欧拉法的主要代价是每迭代更新内力及其导数，这导致速度很慢，无法加速。所以我们引入额外的变量 d 作为一个具有额外参数的扩充的弹簧原长，当表达式最小时，d 恰好也达到正确的值。这样一来，就不必再去计算内力。

  优化过程分 Local 和 Global 两步：Local 阶段优化 d，直接使得𝑑𝑖与𝑝𝑖1 − 𝑝𝑖2共线即可；阶段优化，求得最优点：（𝑴 + ℎ2𝑳)𝒙 = ℎ2𝑱𝒅 − 𝒃 = ℎ2 𝑱𝒅 +𝑴𝒚 − ℎ2𝝋。并且其中𝑴 + 𝒉𝟐𝑳可以预分解，可以大大提升计算速度。



#### 4. 实验结果

同一劲度系数下，隐式欧拉法收敛得更慢，加速方法更为丝滑。在我的电脑上（配置低）更为明显。

在对四面体的实验中，隐式欧拉法和加速方法的结果似乎有较大的差异。加速方法下，四面体运动的幅度似乎很小，即使调低了劲度系数还是比隐式欧拉法的幅度更小。

###### 隐式欧拉

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/6_MassSpring/picture/2.png" alt="image-20220904160819602" width="50%" />

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/6_MassSpring/picture/3.png" alt="image-20220904160819602" width="50%" />



###### 加速方法

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/6_MassSpring/picture/4.png" alt="image-20220904160950877" width="50%" />

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/6_MassSpring/picture/5.png" alt="image-20220904160950877" width="50%" />





#### 5.不足&bug

-   在弹性系数设置过大的时候，欧拉方法会出现变形的情况，问题在于在计算欧拉矩阵时，弹性系数的影响大于外力，导致的bug。
- 在欧拉方法的循环推出条件上，有待调试和改进。

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/6_MassSpring/picture/6.png" alt="image-20220904161202938" width="50%" />



录屏文件在 /video 下

相关.obj .stl文件在 /project/resource 下

时间 9.4-9.6
