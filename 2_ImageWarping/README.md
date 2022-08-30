# homework 2. 图像变形 Image Warping

#### 要求

- 实现RBF,IDW两种算法
- 解决白缝问题（用ANN）

#### 实现

- 程序截图

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/2_ImageWarping/picture/1.png" width="30%" />

#### 按键解释

- 首先点击Choose，选择定点及变形方向。
- 点击RBF/IDW，实现图片变形，此时可以看到变形后的图片有白缝。
- 点击fill，解决白缝问题。

#### RBF效果

- 定点

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/2_ImageWarping/picture/2.png" width="30%" />



- 变形

  <img src="https://github.com/qjy-dhr/cg_learning/blob/main/2_ImageWarping/picture/3.bmp" alt="3" width="30%" />

  

- 填充

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/2_ImageWarping/picture/4.bmp" alt="4" width="30%" />

#### IDW效果

- 定点

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/2_ImageWarping/picture/5.png" alt="image-20220828101814983" width="30%" />

- 变形

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/2_ImageWarping/picture/6.bmp" alt="6" width="30%" />

- 填充

<img src="https://github.com/qjy-dhr/cg_learning/blob/main/2_ImageWarping/picture/7.bmp" alt="7" width="30%" />



p.s.在添加ANN依赖后，若出现“由于找不到UANN_annd.dll，无法继续执行代码，重新安装程序可能会解决此问题”的报错，只需将

..\project\build\_deps\uann-src\bin下的UANN_annd.dll复制到..\project\bin下即可。

时间 8.21-8.24
