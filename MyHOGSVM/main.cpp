#include <QCoreApplication>
#include "cv.h"
#include"cxcore.h"
#include"highgui.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ml.h>

using namespace std;
using namespace cv;

#define Posnum   100  //正样本个数
#define Negnum 50    //负样本个数

int main(int argc, char *argv[])
{
    char classifierSavePath[256] = "d:/pedestrianDetect-peopleFlow.txt";
    // step 1:
    //训练数据的分类标记，即4类
    char adpos[128],adneg[128];
    HOGDescriptor hog(Size(64,64),Size(16,16),Size(8,8),Size(8,8),9);//利用构造函数，给对象赋值。
    int DescriptorDim=1764;//HOG描述子的维数
    Mat samFeatureMat, samLabelMat;
    samFeatureMat = Mat::zeros(1 , DescriptorDim, CV_32FC1);
    samLabelMat = Mat::zeros(1 , 1, CV_32FC1);
    CvSVM svm;
    svm.load(classifierSavePath);
//    //依次读取正样本图片，生成HOG描述子
    for (int i = 1;i <= Posnum ;i++)
    {
        sprintf(adpos, "D:\\Train\\testPos\\1 (%d).png", i);
        Mat src = imread(adpos);//读取图片
        vector<float> descriptors;//HOG描述子向量
        hog.compute(src,descriptors,Size(8,8));
        DescriptorDim = descriptors.size();
        for(int j=0; j<DescriptorDim; j++)
        {
            samFeatureMat.at<float>(0,j) = descriptors[j];
        }
        CvMat feature = samFeatureMat;
        float res = svm.predict(&feature);
        cout << "Presult" << i << ": " << res << endl;
    }
    for (int i = 1;i <= Negnum ;i++)
    {
        sprintf(adpos, "D:\\Train\\testNeg\\1 (%d).png", i);
        Mat src = imread(adpos);//读取图片
        vector<float> descriptors;//HOG描述子向量
        hog.compute(src,descriptors,Size(8,8));
        DescriptorDim = descriptors.size();
        for(int j=0; j<DescriptorDim; j++)
        {
            samFeatureMat.at<float>(0,j) = descriptors[j];
        }
        CvMat feature = samFeatureMat;
        float res = svm.predict(&feature);
        cout << "Nresult" << i << ": " << res << endl;
    }
    system("pause");
    return 0;
}



