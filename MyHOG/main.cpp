#include <QCoreApplication>
#include "cv.h"
#include"cxcore.h"
#include"highgui.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ml.h>

using namespace std;
using namespace cv;

#define Posnum   2316  //正样本个数
#define Negnum 1168    //负样本个数

int main(int argc, char *argv[])
{
    char classifierSavePath[256] = "d:/pedestrianDetect-peopleFlow.txt";
    // step 1:
    //训练数据的分类标记，即4类
    char adpos[128],adneg[128];
    HOGDescriptor hog(Size(64,64),Size(16,16),Size(8,8),Size(8,8),9);//利用构造函数，给对象赋值。
    int DescriptorDim=1764;//HOG描述子的维数
    Mat samFeatureMat, samLabelMat;
    samFeatureMat = Mat::zeros(Posnum + Negnum , DescriptorDim, CV_32FC1);
    samLabelMat = Mat::zeros(Posnum + Negnum , 1, CV_32FC1);
//    //依次读取正样本图片，生成HOG描述子

    //依次读取负样本图片，生成HOG描述子
    for (int k = 1;k <= Negnum ;k++)
    {
        sprintf(adneg, "D:\\Train\\neg\\1 (%d).png", k);
        Mat src = imread(adneg);//读取图片
        cout<<adneg<<endl;
        if(src.data == NULL)
        {
            return 0;
        }
        vector<float> descriptors;//HOG描述子向量
        hog.compute(src,descriptors,Size(16,16));
        DescriptorDim = descriptors.size();
        if(DescriptorDim >= 200000)
        {
            cout<<DescriptorDim<<endl;
            return 0;
        }
        cout<<DescriptorDim<<endl;
        for(int l=0; l<DescriptorDim; l++)
        {
            samFeatureMat.at<float>(k+Posnum-1,l) = descriptors[l];
            samLabelMat.at<float>(k+Posnum-1,0) = -1;
        }
    }

    for (int i = 1;i <= Posnum ;i++)
    {
        sprintf(adpos, "D:\\Train\\pos\\1 (%d).png", i);
        Mat src = imread(adpos);//读取图片
        cout<<adpos<<endl;
        if(src.data == NULL) return 0;
        vector<float> descriptors;//HOG描述子向量
        hog.compute(src,descriptors,Size(16,16));
        DescriptorDim = descriptors.size();

        for(int j=0; j<DescriptorDim; j++)
        {
            samFeatureMat.at<float>(i-1,j) = descriptors[j];
            samLabelMat.at<float>(i-1,0) = 1;
        }
    }


    // step 2:
    //训练参数设定
    CvSVMParams params;
    params.svm_type = CvSVM::C_SVC;                 //SVM类型
    params.kernel_type = CvSVM::LINEAR;             //核函数的类型

    //SVM训练过程的终止条件, max_iter:最大迭代次数  epsilon:结果的精确性
    params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, FLT_EPSILON );


    // step 3:
    //启动训练过程
    CvSVM svm;
    CvMat label = samLabelMat;
    CvMat feature = samFeatureMat;
    svm.train(&feature, &label, NULL,NULL, params);
    svm.save(classifierSavePath);

    // step 4
    // 格式化分类器
    DescriptorDim = svm.get_var_count();//特征向量的维数，即HOG描述子的维数
    int supportVectorNum = svm.get_support_vector_count();//支持向量的个数
    cout<<"支持向量个数："<<supportVectorNum<<endl;

    Mat alphaMat = Mat::zeros(1, supportVectorNum, CV_32FC1);//alpha向量，长度等于支持向量个数
    Mat supportVectorMat = Mat::zeros(supportVectorNum, DescriptorDim, CV_32FC1);//支持向量矩阵
    Mat resultMat = Mat::zeros(1, DescriptorDim, CV_32FC1);//alpha向量乘以支持向量矩阵的结果

    //将支持向量的数据复制到supportVectorMat矩阵中
    for(int i=0; i<supportVectorNum; i++)
    {
        const float * pSVData = svm.get_support_vector(i);//返回第i个支持向量的数据指针
        for(int j=0; j<DescriptorDim; j++)
        {
            //cout<<pData[j]<<" ";
            supportVectorMat.at<float>(i,j) = pSVData[j];
        }
    }

    //将alpha向量的数据复制到alphaMat中
    double alpha = 1;
    double * pAlphaData = alpha;//返回SVM的决策函数中的alpha向量
    for(int i=0; i<supportVectorNum; i++)
    {
        alphaMat.at<float>(0,i) = pAlphaData[i];
    }

    //计算-(alphaMat * supportVectorMat),结果放到resultMat中
    //gemm(alphaMat, supportVectorMat, -1, 0, 1, resultMat);//不知道为什么加负号？
    resultMat = -1 * alphaMat * supportVectorMat;

    //得到最终的setSVMDetector(const vector<float>& detector)参数中可用的检测子
    vector<float> myDetector;
    //将resultMat中的数据复制到数组myDetector中
    for(int i=0; i<DescriptorDim; i++)
    {
        myDetector.push_back(resultMat.at<float>(0,i));
    }
    //最后添加偏移量rho，得到检测子
    myDetector.push_back(-4.2335200835967246);
    cout<<"检测子维数："<<myDetector.size()<<endl;
    //设置HOGDescriptor的检测子
    HOGDescriptor myHOG;
    myHOG.setSVMDetector(myDetector);
    //myHOG.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

    //保存检测子参数到文件
    ofstream fout("HOGDetectorForOpenCV.txt");
    for(int i=0; i<myDetector.size(); i++)
    {
        fout<<myDetector[i]<<endl;
    }
}
