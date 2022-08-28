# homework 2. 图像变形 Image Warping

#### 要求

- 实现RBF,IDW两种算法
- 解决白缝问题（用ANN）

#### 实现

- 程序截图

![image-20220828100818285](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20220828100818285.png)

#### 按键解释

- 首先点击Choose，选择定点及变形方向。
- 点击RBF/IDW，实现图片变形，此时可以看到变形后的图片有白缝。
- 点击fill，解决白缝问题。

#### RBF效果

- 定点

<img src="C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20220828101138268.png" alt="image-20220828101138268" style="zoom:25%;" />



- 变形

  <img src="D:\桌面\中科大\work\cg_learning\2_ImageWarping\picture\3.bmp" alt="3" style="zoom:25%;" />

  

- 填充

<img src="D:\桌面\中科大\work\cg_learning\2_ImageWarping\picture\4.bmp" alt="4" style="zoom:25%;" />

#### IDW效果

- 定点

<img src="C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20220828101814983.png" alt="image-20220828101814983" style="zoom:25%;" />

- 变形

<img src="D:\桌面\中科大\work\cg_learning\2_ImageWarping\picture\6.bmp" alt="6" style="zoom:25%;" />

- 填充

<img src="D:\桌面\中科大\work\cg_learning\2_ImageWarping\picture\7.bmp" alt="7" style="zoom:25%;" />



p.s.在添加ANN依赖后，如出现“由于找不到UANN_annd.dll，无法继续执行代码，重新安装程序可能会解决此问题”的报错，只需将

..\project\build\_deps\uann-src\bin下的UANN_annd.dll复制到..\project\bin下即可。若
