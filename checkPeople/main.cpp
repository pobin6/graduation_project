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

int main(int argc, char *argv[])
{
    Mat img;
    vector<Rect> people;
    img = imread("C:\\Users\\linch\\Desktop\\1356921437_3981.jpg");
    if(img.data == NULL) return 0;

    //读入分类器
    vector<float> x;
    ifstream fileIn("D:\\HOGDetectorForOpenCV.txt", ios::in);
    float val = 0.0f;
    while(!fileIn.eof())
    {
        fileIn>>val;
        x.push_back(val);
    }
    fileIn.close();
    //定义HOG对象，采用默认参数，或者按照下面的格式自己设置
    HOGDescriptor myHog;//(cv::Size(64,64), cv::Size(16,16), cv::Size(8,8), cv::Size(8,8), 9);
    //(cv::Size(64, 128), cv::Size(16, 16), cv::Size(8, 8),
    //cv::Size(8, 8),9, 1, -1,
    //cv::HOGDescriptor::L2Hys, 0.2, true,
    //cv::HOGDescriptor::DEFAULT_NLEVELS);

    //设置SVM分类器，用默认分类器

//    myHog.setSVMDetector(x);
    myHog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    //对图像进行多尺度行人检测，返回结果为矩形框
    myHog.detectMultiScale(img, people,0,Size(8,8),Size(0,0),1.03,2);

    //画长方形，框出行人
    for (int i = 0; i < people.size(); i++)
    {
        Rect r = people[i];
        rectangle(img, r.tl(), r.br(), Scalar(0, 0, 255), 3);
    }

    namedWindow("检测行人", CV_WINDOW_AUTOSIZE);
    imshow("检测行人", img);

    waitKey(0);

    return 0;
}
