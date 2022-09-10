# homework 9. 路径追踪算法

#### 1. 作业要求

- 实现路径追踪算法
- 环境光贴图重要性采样
- 搭建场景（代码，json）并渲染

#### 2. 作业实现



#### 3. 算法原理

##### 3.1 渲染方程

$$
L _ {o}(\pmb{p},\pmb{\omega} _ {o})=L _ {e}(\pmb{p},\pmb{\omega} _ {o})+\int _ {\mathcal{H}^2(\pmb{n}(\pmb{p}))} f _ {r}(\pmb{p},\pmb{\omega} _ {i},\pmb{\omega} _ {o})L _ {i}(\pmb{p},\pmb{\omega} _ {i})\cos\theta _ {\pmb{\omega} _ {i},\pmb{n}(\pmb{p})}\mathbb{d}\pmb{\omega} _ {i}
$$

Path Tracing 方法是将求解渲染方程分为求直接光和间接光两部分的方法。

记
$$
\int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}L=\int _ {\mathcal{H}^2(\pmb{n}(\pmb{p}))} f _ {r}(\pmb{p},\pmb{\omega} _ {i},\pmb{\omega} _ {o})L\cos\theta _ {\pmb{\omega} _ {i},\pmb{n}(\pmb{p})}\mathbb{d}\pmb{\omega} _ {i}
$$

则

$$
L _ {o}(\pmb{p},\pmb{\omega} _ {o})=L _ {e}(\pmb{p},\pmb{\omega} _ {o})+\int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}L _ {i}(\pmb{p},\pmb{\omega} _ {i})
$$

反射方程为

$$
L _ {r}(\pmb{p},\pmb{\omega} _ {o})=\int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}L _ {i}(\pmb{p},\pmb{\omega} _ {i})
$$

$$
对于 L _ {i} 有关系式
$$

$$
L _ {i}(\pmb{p},\pmb{\omega} _ {i})=L _ {o}(\mathop{raytrace}(\pmb{p},\pmb{\omega _ {i}}),-\pmb{\omega _ {i}})
$$

> $$
> 其中 \mathop{raytrace} 是射线与场景的相交函数
> $$

$$
记 \mathop{raytrace}(\pmb{p},\pmb{\omega} _ {i}) 为 \pmb{p}^\prime，则有
$$

$$
L _ {i}(\pmb{p},\pmb{\omega} _ {i})=L _ {o}(\pmb{p}^\prime,-\pmb{\omega _ {i}})
$$

如此形成递归

$$
L _ {o}(\pmb{p},\pmb{\omega} _ {o})=L _ {e}(\pmb{p},\pmb{\omega} _ {o})+\int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}L _ {o}(\pmb{p}^\prime,-\pmb{\omega} _ {i})
$$

$$
将 L _ {r} 展开一次
$$

$$
\begin{aligned}
L _ {r}(\pmb{p},\pmb{\omega} _ {o})
&=\int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}\left(L _ {e}(\pmb{p}^\prime,-\pmb{\omega} _ {i})+L _ {r}(\pmb{p}^\prime,-\pmb{\omega} _ {i})\right)\newline 
&=\int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}L _ {e}(\pmb{p}^\prime,-\pmb{\omega} _ {i})
+\int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}L _ {r}(\pmb{p}^\prime,-\pmb{\omega} _ {i})
\end{aligned}
$$

将𝑝点沿𝜔𝑜方向的出射光，分成求直接光和间接光两部分。直接光是直接由光源发出的；间接光则是照到其他表面的得到的。

- $$
  \int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}L _ {e}(\pmb{p}^\prime,-\pmb{\omega} _ {i}) 称为直接光，记作 L _ {\text{dir}}(\pmb{p},\pmb{\omega} _ {o})
  $$

  

- $$
  \int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}L _ {r}(\pmb{p}^\prime,-\pmb{\omega} _ {i}) 称为间接光，记作 L _ {\text{indir}}(\pmb{p},\pmb{\omega} _ {o})
  $$

  

##### 3.2 直接光

$$
L _ {\text{dir}}(\pmb{x}\to\pmb{z})=\int _ A f _ {r}(\pmb{y}\to \pmb{x}\to\pmb{z})L _ {e}(\pmb{y}\to\pmb{x})G(\pmb{x}\leftrightarrow\pmb{y})\mathbb{d}A(\pmb{y})
$$

$$
其中积分域 A 为场景中所有的面积，但只有光源处 L _ {e}(\pmb{y}\to\pmb{x})\neq 0 记光源数 N _ {e}，
$$

$$
场景中的光源集为 \{L _ {e _ {i}}\} _ {i=1}^{N _ {e}} ，对应的区域集为 \{A(L _ {e _ {i}})\} _ {i=1}^{N _ {e}}，则可写为
$$

$$
L _ {\text{dir}}(\pmb{x}\to\pmb{z})=\sum _ {i=1}^{N _ {e}}\int _ {A(L _ {e _ {i}})} f _ {r}(\pmb{y}\to\pmb{x}\to\pmb{z})L _ {e}(\pmb{y}\to\pmb{x})G(\pmb{x}\to\pmb{y})\mathbb{d}A(\pmb{y})
$$

光源处的光重要性大于其他的光

##### 3.3 间接光

这时候的p0是非光源点（假如无交或交点是光源，已经在直接光中计算过了，因此这时候返回0）,用概率方法计算，通过BRDF的分布采样*N*个(实现时*N* = 1)方向，概率密度*p*(ωi)，从而 

递归即可

$$
L _ {r}(\pmb{p},\pmb{\omega} _ {o})=\int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}L _ {e}(\pmb{p}^\prime,-\pmb{\omega} _ {i})
+\int _ {\pmb{p},\pmb{\omega} _ {o},\pmb{\omega} _ {i}}L _ {r}(\pmb{p}^\prime,-\pmb{\omega} _ {i})
$$



##### **3.4** 蒙特卡洛积分、环境光贴图重要性采样、多重重要性采样

这一步在于优化直接光计算中环境光的采样，目的就是按照一个采样方案给出一个ωi和对应的概率密度*p*(ωi)。 

由大数定律，在采样数*N*充分大时，用Monte Carlo方法计算的结果的平均将以某种方式收敛到它的期望，选择合适的分布函数*p*(*x*)能加快结果的收敛速度，从而对于较小的*N*就有较好的效果。原来的方法采用了均匀分布采样，对于环境光贴图，取新的概率分布满足概率密度函数和Le正相关，从而使Monte Carlo方法构造的随机变量接近常数（即方差接近0）。 生成概率分布后，用Alias Method算法进行采样，可以降低采样的时间复杂度。

多重重要性采样
$$
一个重要性采样的策略就会有一个采样概率分布 p _ i(x)， 假设可供选择的策略为 N 个 \{p _ i(x)\} _ {i=1}^N
$$
，我们可以混合他们得到一个采样新策略

采样的伪代码如下

```c++
float sample(float rnd){
    if(rnd < w1)
        return sample1();
    else if(rnd < w1 + w2)
        return sample2();
    else if(rnd < w1 + w2 + w3)
        return sample3();
    ...
    else
        return sampleN();
}
```

##### 3.5 渲染方程的计算

我们要计算的是如下积分

$$
L _ {r}(\pmb{p},\pmb{\omega} _ {o})=L _ {\text{dir}}+L _ {\text{indir}}
$$

右侧积分式需要递归

利用蒙特卡洛积分可将积分转成采样

$$
\begin{aligned}
L _ {\text{dir}}(\pmb{x}\to\pmb{z})
&\approx\sum _ {i=1}^{N _ {e}}\sum _ {j=1}^{N _ {i}}\frac{f _ {r}(\pmb{y} _ {i}^{(j)}\to\pmb{x}\to\pmb{z})L _ {e}(\pmb{y} _ {i}^{(j)}\to\pmb{x})G(\pmb{x}\to\pmb{y} _ {i}^{(j)})}{p(\pmb{y} _ {i}^{(j)})}\newline 
L _ {\text{indir}}(\pmb{p},\pmb{\omega} _ {o})
&\approx\sum _ {k=1}^{N}\frac{f _ {r}(\pmb{p},\pmb{\omega} _ {i}^{(k)},\pmb{\omega} _ {o})L _ {r}(\pmb{p}^{\prime(k)},-\pmb{\omega})\cos\theta _ {\pmb{\omega} _ {i},\pmb{n}(\pmb{p})}}{p(\pmb{\omega} _ {i}^{(k)})}
\end{aligned}
$$

