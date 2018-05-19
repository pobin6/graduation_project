#ifndef DETECTCV_H
#define DETECTCV_H

#include "QScreen"
#include "QGuiApplication"
#include "QPixmap"
#include "QMap"
#include "iostream"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "person.h"

using namespace cv;
using namespace std;
class DetectCV
{
public:
    DetectCV();
    int detectFace();
    int detectBody(Mat frame, vector<Rect> boundRect);
    int detectMove(String  background);
    void FullScreen(int x, int y, int width, int height);
    MatND detectND(Mat img);
    Point heightPoint(Rect r);
    void detectDirec(Mat frame, Rect r);
    void detectDirec2(Rect bound);
    void detecNum();
    /** 全局变量 */
    String PATH = "../../../应用/opencv/opencv-3.3.0/data/haarcascades/";
    String face_cascade_name = PATH + "haarcascade_frontalface_alt.xml";
    CascadeClassifier face_cascade;
    String window_name = "Capture - Face detection";
    int now = 0;
    int past = 0;
    int per_num = 0;
    QList <MatND> per_nd;
    QList <Person> per_direc;
};

#endif // DETECTCV_H
