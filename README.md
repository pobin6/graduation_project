# 基于openCv进行行人检测
> 声明: 所有代码均是基于OpenCv2和Qt
## 文件夹说明：
*  Train文件夹: 训练用数据，来源于**INRIA Person Dataset**中的一部分，neg文件夹中为负样本，
pos文件夹中为正样本。
*  MyHOGSVM文件夹: 训练和检验程序，将Train文件夹中的数据随机分为十组，进行交叉检验。九组训练，一组检验。
* MyHOG文件夹：将所有数据进行训练得到SVM，并存储