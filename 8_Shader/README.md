

# homework 8. 着色器

#### 1. 作业要求

- 实现 normal map 和 displacement map
- 利用 displacement map 在 vertex shader 中进行简单降噪
- 实现点光源阴影（可选）

#### 2. 实现

- 法线贴图和置换贴图的使用。
- 用置换贴图简单去噪，其中增加按键↑↓来 控制置换系数*λ*的大小，用以观察其大小对去噪效果的影响。

#### 3. 实现原理

##### 3.1 法线贴图

  基本方法是通过将置换贴图沿法线方向将顶点进行移动，并且将法线贴图应用在顶点上，以使得结果更加真实。

  在切线空间中，法线永远指着正z方向，需转换成视图空间下，在片段着色器中使用TBN矩阵，用计算得到的T、B、N，把切线坐标空间的向量转换到视图坐标空间。

##### 3.2 置换贴图

  置换贴图用于改变顶点的位置。黑色 (0) 表示不动，白色 (1) 表示沿法移动，将置换贴图的值转换成顶点偏移量，具体的偏移程度由像素值和系数*λ*决定，变换公式为:

displacement = λ* (bias+scale * pixel value)

  由于置换贴图只改变了顶点的位置，不改变顶点的法向，需要添加相应的法线贴图。

##### 3.3 置换贴图去噪 

  核心就是对每个顶点进行合理的偏移，具体步骤为：

- 计算每个顶点的偏移量

- 将偏移量投影到法向上

- 对每一个顶点进行偏移

    实验中发现obj文件有边界，导致通过上述方法生成displacementmap后，在边界处会产生缝隙，出现这样的原因是模型的顶点数远小于1024*1024，这会造成置换贴图非常稀疏。我采用ANN库的最邻近算法进行插值得到完整的置换贴图。a对于不是顶点的像素点，使用最邻近算法找到最近顶点，将其深度赋值给它。

最终步骤为：

- 计算每个顶点的偏移量
- 将偏移量投影到法向上
- 对每一个顶点进行偏移
- 将这个偏移量存储到顶点对应的纹理贴图中；
- 对于纹理贴图中的其他点，采用ANN插值出对应像素值；
- 用像素缩放的方法生成相应的置换贴图。 

#### 4.实验结果

##### 4.1 法线贴图

cg_normalmap

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/8_Shader/picture/cg_normalmap.jpg" alt="cg_normalmap" width="30%" />

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/8_Shader/picture/image-20220908225034386.png" alt="image-20220908225034386" width="50%" />



##### 4.2 置换贴图

cg_displacementmap

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/8_Shader/picture/cg_displacementmap.jpg" alt="cg_displacementmap" width="30%" />

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/8_Shader/picture/image-20220908225213689.png" alt="image-20220908225213689" width="50%" /><img src="https://github.com/qjy-dhr/cg_learning/blob/main/8_Shader/picture/image-20220908225323683.png" alt="image-20220908225323683" width="50%" />

bias=-1，scale=2  ； bias=0，scale=1

##### 4.3 置换贴图去噪

去噪前 去噪后

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/8_Shader/picture/image-20220908234219587.png" alt="image-20220908234219587" width="50%" /><img src="https://github.com/qjy-dhr/cg_learning/blob/main/8_Shader/picture/image-20220908234334960.png" alt="image-20220908234334960" width="50%" />

去除噪声的 Displacement Map

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/8_Shader/picture/image-20220908235114665.png" alt="image-20220908235114665" width="50%" />

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/8_Shader/picture/image-20220909092254462.png" alt="image-20220909092254462"  width="50%" />

可以看到，尽管使用ANN进行了插值，还是会有一点缝隙。



时间：9.6-9.8
