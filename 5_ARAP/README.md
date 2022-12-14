# homework 5. ARAPåæ°å ð¢

#### 1. è¦æ±

- å¨ç»å®çç½æ ¼æ¡æ¶ä¸å®æä½ä¸ï¼å®ç°
  - ASAP (As-similar-as-possible) åæ°åç®æ³
  - ARAP (As-rigid-as-possible) åæ°åç®æ³
  - Hybrid åæ°åæ¹æ³ï¼å¯éï¼
- å¯¹åç§åæ°åæ¹æ³ï¼åæ¬ä½ä¸4çFloateræ¹æ³ãASAP/ARAPæ¹æ³ç­ï¼è¿è¡æ¯è¾
- ç»§ç»­å­¦ä¹ åå·©åºä¸è§ç½æ ¼çæ°æ®ç»æåç¼ç¨
- å­¦ä¹ åå®ç°ç©éµç SVD åè§£
- è¿ä¸æ­¥å·©åºä½¿ç¨ Eigen åºæ±è§£å¤§åç¨ççº¿æ§æ¹ç¨ç»



#### 2. å®ç°

- ä½¿ç¨æ±è§£ç¨çæ¹ç¨ç»çæ¹æ³å®ç°çASAPåæ°åï¼
- ä»¥ASAPåæ°åä¸ºåå¼ï¼ä½¿ç¨Local/Globalå®ç°ARAPåæ°åï¼
- ç¸å³çº¹çæ å°çæ¾ç¤º

#### 3. ç®æ³åçç¬è®°

##### 3.1 ä¸åºå®è¾¹çåæ°å

â    å¯¹äºç½æ ¼ä¸­æ¯ä¸ªä¸è§å½¢ï¼é½æä¸ä¸ªèªå·±å±é¨çç­è·åæ°åð¥ð¡ = {ð¥ð¡0, ð¥ð¡1, ð¥ð¡2}ï¼å¨åç½æ ¼ä¸è§å½¢ä¸åç½æ ¼åæ°ååçä¸è§å½¢ä¹é´å­å¨å¯ä¸ççº¿æ§æ å°ï¼å³ Jacobian ç©éµãä¸ºäºè¡¡éåæ°åå¯¹å¾å½¢çæ­æ²ç¨åº¦ï¼æä»¬å¯ä»¥å¼å¥è¾å©çº¿æ§åæ¢ Ltï¼å¹¶å®ä¹è½éå½æ°ï¼

ð¸(ð¢, ð¿) = âð´ð¡âð½ð¡(ð¢) â ð¿ð¡â?

å³ä¸åºå®è¾¹çåæ°åçè¿ç¨å°±æ¯æ¾å°åéç ï¼u,Lï¼, æ»¡è¶³

(ð¢, ð¿) = arg min(ð¢,ð¿) ð¸(ð¢, ð¿)

```
uï¼åæ°ååçéåð¢ = {ð¢1, â¦ , ð¢ð¡}

Lï¼è¾å©çº¿æ§åæ¢ï¼å¦æè½¬ãä»¿å°ã
```

æ¾ç¶ï¼å¯¹äºä¸åçLï¼æ±è§£æ¹æ³ä¸åã

##### 3.2  As Similar As Possible (ASAP) ç®æ³

â    è¯¥æ¹æ³ä¿æäºåæ°ååè§åº¦çä¸åæ§ï¼å°½å¯è½ççä¿è§ï¼å æ­¤è¿éæä»¬ææææå¨ççº¿æ§åæ¢æå°½å¯è½æ¥è¿ç¸ä¼¼åæ¢ ð¿éå[ ð    ð  

â                                                                                     âð    ð]

â    ASAPéç¨æ±è§£ç¨çæ¹ç¨ç»çæ¹æ³ï¼åå¯¼æ°ç­äº0ã

â    å¶ä¸­ç©éµå¤§å°ä¸º2(nV + nP) é¶ï¼å¶ä¸­nVä¸ºmeshä¸­ä¸è§å½¢çæ°éãå 

2nVè¡å­æ¾ux,uyçæ¹ç¨ï¼å2nPè¡å­æ¾a,bçæ¹ç¨ã

â    è®ºæä¸­æåºï¼ASAP å¯è½å¾å°å¹³å¡è§£ï¼è¦é¿åå¹³å¡è§£çåºç°ï¼éè¦éåè³å°ä¿©éç¹ï¼è¦éåç¸éè·ç¦»å°½å¯è½è¿çä¸¤ä¸ªè¾¹çä¸çç¹ä½ä¸ºéç¹ã

##### 3.3 As Rigid As Possible (ARAP)ç®æ³

â    è¯¥æ¹æ³æå¤§åçä¿æåæ°ååå¾å½¢çåæ§ï¼å°½å¯è½ççä¿å½¢ãå æ­¤è¿ééå¶ð¿åªå±äºæè½¬åæ¢ï¼ð¿éå[ cos ð   sin ð 

â                                               â sin ð   cosð]: ð â [0,2ð)

å ä¸ºæ¹ç¨æ¯éçº¿æ§çï¼ä¸è½ç¨æ±åå¯¼çæ¹å¼ï¼å æ­¤ARAPéç¨äº"Local/Global"çè¿­ä»£æ¹æ³ã

å·ä½æ­¥éª¤å¦ä¸

1. åå§å

å¤ç¨ASAPçæ å°å½æ°ï¼æ å°æ¹æ³ï¼3D-2D

åä¸è§å½¢ä¸ºx0,x1,x2,â x1x0x2=Î¸,åæ å°åï¼u0=ï¼0ï¼0ï¼0ï¼,u1=ï¼x0x1,0,0ï¼,u2=ï¼x0x2cosÎ¸,x0x2sinÎ¸,0ï¼

2. Local é¶æ®µï¼åºå®ð¢ï¼æ±ð¿ð¡ï¼

å¯¹Jtè¿è¡SVDåè§£ï¼æ*J* =UDV*T*ï¼det(UV*T*) å© *>* å°

   3.å¨ Global é¶æ®µï¼åºå®ð¿ð¡ï¼æ±ð¢

å¦æ­¤åå¤è¿­ä»£ï¼æç»æ¶æã

#### 4. å®ç°ææ

Mesh

| name     | Init                                                         | ASAP Mesh                                                    | ARAP Mesh (10)                                               |
| -------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| Beetle   | <img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Beetle_ABF/init.png" alt="face-par"/> | ![ASAPMesh]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Beetle_ABF/ASAPMesh.png"/> | ![ARAPMesh]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Beetle_ABF/ARAPMesh.png"/> |
| Cow      | ![init]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Cow_dABF/init.png"/> | ![ASAPMesh]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Cow_dABF/ASAPMesh.png"/> | ![ARAPMesh]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Cow_dABF/ARAPMesh.png"/> |
| Gargoyle | ![init]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Gargoyle_ABF/init.jpg"/> | ![ASAPMesh ]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Gargoyle_ABF/ASAPMesh .png"/> | ![ARAPMesh]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Gargoyle_ABF/ARAPMesh.png"/> |
| Isis     | ![init]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Isis_dABF/init.png"/> | ![ASAPMesh]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Isis_dABF/ASAPMesh.png"/> | ![ARAPMesh]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Isis_dABF/ARAPMesh.png"/> |



| name     | CC                                                           | SC                                                           | ASAP                                                         | ARAP(15)                                                     |
| :------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| Beetle   | ![CC]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Beetle_ABF/CC.png"/> | ![SC]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Beetle_ABF/SC.png"/> | ![ASAP]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Beetle_ABF/ASAP.png"/> | ![ARAP_15]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Beetle_ABF/ARAP_15.png"/> |
| Cow      | ![CC]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Cow_dABF/CC.png"/> | ![SC]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Cow_dABF/SC.png"/> | ![ASAP]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Cow_dABF/ASAP.png"/> | ![ARAP_15]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Cow_dABF/ARAP_15.png"/> |
| Gargoyle | ![CC]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Gargoyle_ABF/CC.png"/> | ![SC]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Gargoyle_ABF/SC.png"/> | ![ASAP]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Gargoyle_ABF/ASAP.jpg"/> | ![ARAP_15]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Gargoyle_ABF/ARAP_15.png"/> |
| Isis     | ![CC]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Isis_dABF/CC.png"/> | ![SC]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Isis_dABF/SC.png"/> | ![ASAP]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Isis_dABF/ASAP.png"/> | ![ARAP_15]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Isis_dABF/ARAP_15.png"/> |



| Iteration times | 5                                                            | 15                                                           | 30                                                           | 50                                                           |
| --------------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| Beetle          | ![ARAP_5]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Beetle_ABF/ARAP_5.png"/> | ![ARAP_15]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Beetle_ABF/ARAP_15.png"/> | ![ARAP_30]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Beetle_ABF/ARAP_30.png"/> | ![ARAP_50]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Beetle_ABF/ARAP_50.png"/> |
| Cow             | ![ARAP_5]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Cow_dABF/ARAP_5.png"/> | ![ARAP_15]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Cow_dABF/ARAP_15.png"/> | ![ARAP_30]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Cow_dABF/ARAP_30.png"/> | ![ARAP_50]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Cow_dABF/ARAP_50.png"/> |
| Gargoyle        | ![ARAP_5]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Gargoyle_ABF/ARAP_5.png"/> | ![ARAP_15]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Gargoyle_ABF/ARAP_15.png"/> | ![ARAP_30]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Gargoyle_ABF/ARAP_30.png"/> | ![ARAP_50]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Gargoyle_ABF/ARAP_50.png"/> |
| Isis            | ![ARAP_5]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Isis_dABF/ARAP_5.png"/> | ![ARAP_15]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Isis_dABF/ARAP_15.png"/> | ![ARAP_30]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Isis_dABF/ARAP_30.png"/> | ![ARAP_50]<img src="https://github.com/qjy-dhr/cg_learning/blob/main/5_ARAP/picture/Isis_dABF/ARAP_50.png"/> |

##### ä¸è¶³

â    ä»£ç æ¯è¾æ··ä¹±ï¼æ³å°çè½æ¹è¿çç¹æï¼1.å¯ä»¥æ½è±¡åºä¸ä¸ªåå§åçåºç±»ï¼ç¨äºææåæ°åæ¹æ³çåå§åã2.éç¨ä¸è§å½¢ç½æ ¼ç»æè¿è¡è®¡ç®.......æè®¸ä»¥åä¼æ¥å¡«åã

æ¶é´ 9.1-9.4

