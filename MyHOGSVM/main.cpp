#include <QCoreApplication>
#include "cv.h"
#include"cxcore.h"
#include"highgui.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <ml.h>

using namespace std;
using namespace cv;

int test(int posSam[], int negSam[]);
int train(int posSam[], int negSam[]);

#define Posnum   240  //正样本个数
#define Negnum 120    //负样本个数

#define PosTrainnum   2160  //正样本个数
#define NegTrainnum 1080    //负样本个数

char classifierSavePath[256] = "pedestrianDetect-peopleFlow.txt";
int main(int argc, char *argv[])
{
    //posTestSam 测试正样本
    //negTestSam 测试负样本
    //posTrainSam 训练正样本
    //negTrainSam 训练负样本
    int posTestSam[Posnum],negTestSam[Negnum], posTrainSam[PosTrainnum], negTrainSam[NegTrainnum];
    //posSam 所有正样本 negSam 所有负样本
    int posSam[Posnum+PosTrainnum], negSam[Negnum+NegTrainnum];
    //将所有正样本随机分为十份
    int i;
    for(i=0; i<Posnum+PosTrainnum; i++)
    {
        posSam[i]=i;
    }
    for(i=0; i<Negnum+NegTrainnum; i++)
    {
        negSam[i]=i;
    }
    srand((unsigned)time(NULL));
    for(i=0; i<Posnum+PosTrainnum; i++)
    {
        int r = rand() % (Posnum+PosTrainnum);
        int t = posSam[i];
        posSam[i] = posSam[r];
        posSam[r]=t;
    }
    //将所有负样本随机分为十份
    for(i=0; i<Negnum+NegTrainnum; i++)
    {
        negSam[i]=i;
    }
    srand((unsigned)time(NULL));
    for(i=0; i<Negnum+NegTrainnum; i++)
    {
        int r = rand() % (Negnum+NegTrainnum);
        int t = negSam[i];
        negSam[i] = negSam[r];
        negSam[r]=t;
    }
    //轮流十组数据，一组检验，九组训练
    int n=0, m=0, x=0, y=0,j;
    for(i=0; i<10; i++)
    {
        for(j=0; j<Posnum+PosTrainnum; j++)
        {
            if(j>=Posnum*i&&j<Posnum*(i+1))
            {
                posTestSam[n] = j+1;
                n++;
            }
            else
            {
                posTrainSam[m] = j+1;
                m++;
            }
        }
        for(j=0; j<Negnum+NegTrainnum; j++)
        {
            if( j>=Negnum*i && j<Negnum*(i+1) )
            {
                negTestSam[x] = j+1;
                x++;
            }
            else
            {
                negTrainSam[y] = j+1;
                y++;
            }
        }
        int resTrain = train(posTrainSam, negTrainSam);
        if( resTrain==1 )
        {
            int resTest = test(posTestSam, negTestSam);
            if( resTest==0 ) {cout<< "Test error!!!!" <<endl;break;}
            else {cout<< "the " << i << " group success" <<endl;}
        }
        else
        {
            cout<<"Train error!!!!"<<endl;
            break;
        }
        m=0, n=0, x=0, y=0;
    }
    system("pause");
    return 0;
}

int test(int posSam[],int negSam[])
{
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
    //依次读取正样本图片，生成HOG描述子
    float P = 0;
    for (int i = 1;i <= Posnum ;i++)
    {
        sprintf(adpos, "../Train/pos/1 (%d).png", posSam[i-1]);
        Mat src = imread(adpos);//读取图片
        if(src.data == NULL)
        {
            cout<<adpos<<endl;
            return 0;
        }
        vector<float> descriptors;//HOG描述子向量
        hog.compute(src,descriptors,Size(8,8));
        DescriptorDim = descriptors.size();
        for(int j=0; j<DescriptorDim; j++)
        {
            samFeatureMat.at<float>(0,j) = descriptors[j];
        }
        CvMat feature = samFeatureMat;
        float res = svm.predict(&feature);
        if(res>0) P+=1;
    }
    P=P/Posnum;
    cout << "Presult: " << P << endl;
    P=0;
    for (int i = 1;i <= Negnum ;i++)
    {
        sprintf(adneg, "../Train/neg/1 (%d).png", negSam[i-1]);
        Mat src = imread(adneg);//读取图片
        if(src.data == NULL)
        {
            cout<<adneg<<endl;
            return 0;
        }
        vector<float> descriptors;//HOG描述子向量
        hog.compute(src,descriptors,Size(8,8));
        DescriptorDim = descriptors.size();
        for(int j=0; j<DescriptorDim; j++)
        {
            samFeatureMat.at<float>(0,j) = descriptors[j];
        }
        CvMat feature = samFeatureMat;
        float res = svm.predict(&feature);
        if(res<0) P+=1;
    }
    P/=Negnum;
    cout << "Nresult: " << P << endl;
    return 1;
}

int train(int posSam[],int negSam[])
{
    // step 1:
    //训练数据的分类标记，即4类
    char adpos[128],adneg[128];
    HOGDescriptor hog(Size(64,64),Size(16,16),Size(8,8),Size(8,8),9);//利用构造函数，给对象赋值。
    int DescriptorDim=1764;//HOG描述子的维数
    Mat samFeatureMat, samLabelMat;
    samFeatureMat = Mat::zeros(PosTrainnum + NegTrainnum , DescriptorDim, CV_32FC1);
    samLabelMat = Mat::zeros(PosTrainnum + NegTrainnum , 1, CV_32FC1);
//    //依次读取正样本图片，生成HOG描述子

    //依次读取负样本图片，生成HOG描述子
    for (int k=1; k<=NegTrainnum; k++)
    {
        sprintf(adneg, "../Train/neg/1 (%d).png", negSam[k-1]);
        Mat src = imread(adneg);//读取图片
        cout<<adneg<<endl;
        if(src.data == NULL)
        {
            cout<<adneg<<endl;
            return 0;
        }
        vector<float> descriptors;//HOG描述子向量
        hog.compute(src,descriptors,Size(16,16));
        DescriptorDim = descriptors.size();
        for(int l=0; l<DescriptorDim; l++)
        {
            samFeatureMat.at<float>(k+PosTrainnum-1,l) = descriptors[l];
            samLabelMat.at<float>(k+PosTrainnum-1,0) = -1;
        }
    }
    cout<<"==============1"<<endl;
    for (int i = 1;i <= PosTrainnum ;i++)
    {
        sprintf(adpos, "../Train/pos/1 (%d).png", posSam[i-1]);
        Mat src = imread(adpos);//读取图片
        if(src.data == NULL)
        {
            cout<<adpos<<endl;
            return 0;
        }
        vector<float> descriptors;//HOG描述子向量
        hog.compute(src,descriptors,Size(16,16));
        DescriptorDim = descriptors.size();

        for(int j=0; j<DescriptorDim; j++)
        {
            samFeatureMat.at<float>(i-1,j) = descriptors[j];
            samLabelMat.at<float>(i-1,0) = 1;
        }
    }
    cout<<"==============2"<<endl;
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

    return 1;
}


