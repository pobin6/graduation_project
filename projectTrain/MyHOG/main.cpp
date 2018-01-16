#include <QCoreApplication>
#include "cv.h"
#include"cxcore.h"
#include"highgui.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ml.h>
#include <fstream>

using namespace std;
using namespace cv;

#define Posnum   2400  //正样本个数
#define Negnum 1200    //负样本个数


class MySVM : public CvSVM
{
public:
    double* getAlpha()
    {
        return this->decision_func->alpha;
    }
    double getRho()
    {
        return this->decision_func->rho;
    }
};
char *classifierSavePath = "../HOGDetectorForOpenCV.txt";
char *imagePath = "../Train";
int main(int argc, char *argv[])
{
    switch (argc) {
    case 1:
        break;
    case 2:
        classifierSavePath = argv[1];
        break;
    case 3:
        classifierSavePath = argv[1];
        imagePath = argv[2];
        break;
    default:
        cout<< "arg is too much, more than two";
        return 0;
        break;
    }
    cout<< "classifierSavePath: " << classifierSavePath <<endl;
    cout<< "imagePath: " << imagePath <<endl;
    // step 1:
    //训练数据的分类标记
    char adpos[128],adneg[128];
    HOGDescriptor hog(Size(64,64),Size(16,16),Size(8,8),Size(8,8),9);//利用构造函数，给对象赋值。
    int DescriptorDim=1764;//HOG描述子的维数
    Mat samFeatureMat, samLabelMat;
    samFeatureMat = Mat::zeros(Posnum + Negnum , DescriptorDim, CV_32FC1);
    samLabelMat = Mat::zeros(Posnum + Negnum , 1, CV_32FC1);
    cout<< "start read data to get HOG" <<endl;
    //依次读取正样本图片，生成HOG描述子
    for (int i = 1;i <= Posnum ;i++)
    {
        sprintf(adpos, "%s/pos/1 (%d).png", imagePath,i);
        Mat src = imread(adpos);//读取图片
        if(src.data == NULL)
        {
            cout<<"image not exist: "<<adpos<<endl;
            return 0;
        }
        vector<float> descriptors;//HOG描述子向量
        hog.compute(src,descriptors,Size(16,16));
        for(int j=0; j<DescriptorDim; j++)
        {
            samFeatureMat.at<float>(i-1,j) = descriptors[j];
            samLabelMat.at<float>(i-1,0) = 1;
        }
    }
    //依次读取负样本图片，生成HOG描述子
    for (int k = 1;k <= Negnum ;k++)
    {
        sprintf(adneg, "%s/neg/1 (%d).png", imagePath,k);
        Mat src = imread(adneg);//读取图片
        if(src.data == NULL)
        {
            cout<<"image not exist: "<<adneg<<endl;
            return 0;
        }
        vector<float> descriptors;//HOG描述子向量
        hog.compute(src,descriptors,Size(16,16));
        for(int l=0; l<DescriptorDim; l++)
        {
            samFeatureMat.at<float>(k+Posnum-1,l) = descriptors[l];
            samLabelMat.at<float>(k+Posnum-1,0) = -1;
        }
    }
    cout<< "finish get HOG" <<endl;
    // step 2:
    //训练参数设定
    CvSVMParams params;
    params.svm_type = CvSVM::C_SVC;                 //SVM类型
    params.kernel_type = CvSVM::LINEAR;             //核函数的类型

    //SVM训练过程的终止条件, max_iter:最大迭代次数  epsilon:结果的精确性
    params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, FLT_EPSILON );


    // step 3:
    //启动训练过程
    MySVM svm;
    CvMat label = samLabelMat;
    CvMat feature = samFeatureMat;
    cout<< "start SVM train" <<endl;
    if(!svm.train(&feature, &label, NULL,NULL, params))
    {
        cout<<"svm train failed,reason not clearly"<<endl;
        return 0;
    }
    cout<< "finish SVM train, strat save classifier" <<endl;
    // step 4:
    // 将SVM根据wx+b公式存储为分类器
    int supportVectorNum = svm.get_support_vector_count();
    double *svm_alpha;
    double  svm_rho;
    const float *svm_support;

    svm_rho = svm.getRho();
    svm_alpha = svm.getAlpha();

    Mat alphaMat = Mat::zeros(1, supportVectorNum, CV_32FC1);//alpha向量，长度等于支持向量个数
    Mat supportVectorMat = Mat::zeros(supportVectorNum, DescriptorDim, CV_32FC1);//支持向量矩阵
//    supportVectorMat = supportVector;
    Mat resultMat = Mat::zeros(1, DescriptorDim, CV_32FC1);//alpha向量乘以支持向量矩阵的结果

    //将支持向量的数据复制到supportVectorMat矩阵中
    for(int i=0; i<supportVectorNum; i++)
    {
        svm_support = svm.get_support_vector(i);
        for(int j=0; j<DescriptorDim; j++)
        {
            supportVectorMat.at<float>(i,j) = svm_support[j];
        }
    }
    for(int i=0; i<supportVectorNum; i++)
    {
        alphaMat.at<float>(0, i) = svm_alpha[i];
    }
    resultMat = (-1) * (alphaMat * supportVectorMat);

    //得到最终的setSVMDetector(const vector<float>& detector)参数中可用的检测子
    vector<float> myDetector;
    //将resultMat中的数据复制到数组myDetector中
    for(int i=0; i<DescriptorDim; i++)
    {
        myDetector.push_back(resultMat.at<float>(0,i));
    }
    //最后添加偏移量rho，得到检测子
    myDetector.push_back(svm_rho);

    //保存检测子参数到文件
    ofstream fout(classifierSavePath);
    for(int i=0; i<myDetector.size(); i++)
    {

        fout<<myDetector[i]<<endl;
    }
    cout << "complete finish" <<endl;
    system("pause");
    return 0;
}

