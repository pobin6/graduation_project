#include <QCoreApplication>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/ml.hpp"
#include <fstream>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //=================
    char classifierSavePath[256] = "D:\\pedestrianDetect-peopleFlow.xml";
    // step 1:
    //训练数据的分类标记，即4类
    int DescriptorDim;//HOG描述子的维数
    Ptr<ml::SVM>svm = ml::SVM::load<ml::SVM>(classifierSavePath);
    DescriptorDim = svm->getVarCount();//特征向量的维数，即HOG描述子的维数

    Mat supportVector = svm->getSupportVectors();//支持向量的个数
    int supportVectorNum = supportVector.rows;
    vector<float> svm_alpha;
    vector<float> svm_svidx;
    float  svm_rho;
    svm_rho = svm->getDecisionFunction(0, svm_alpha, svm_svidx);
    Mat alphaMat = Mat::zeros(1, supportVectorNum, CV_32FC1);//alpha向量，长度等于支持向量个数
    Mat supportVectorMat = Mat::zeros(supportVectorNum, DescriptorDim, CV_32FC1);//支持向量矩阵
    supportVectorMat = supportVector;
    Mat resultMat = Mat::zeros(1, DescriptorDim, CV_32FC1);//alpha向量乘以支持向量矩阵的结果

    //将支持向量的数据复制到supportVectorMat矩阵中
    for(int i=0; i<supportVectorNum; i++)
    {
        alphaMat.at<float>(0, i) = svm_alpha[i];
    }

    resultMat = -1 * alphaMat * supportVectorMat;

    //得到最终的setSVMDetector(const vector<float>& detector)参数中可用的检测子
    vector<float> myDetector;
    //将resultMat中的数据复制到数组myDetector中
    for(int i=0; i<DescriptorDim; i++)
    {
        myDetector.push_back(resultMat.at<float>(0,i));
        cout << resultMat.at<float>(0,i) << endl;
    }
    //最后添加偏移量rho，得到检测子
    myDetector.push_back(svm_rho);

    //保存检测子参数到文件
    ofstream fout("D:\\HOGDetectorForOpenCV.txt");
    for(int i=0; i<myDetector.size(); i++)
    {

        fout<<myDetector[i]<<endl;
    }
    return a.exec();
}
