#ifndef DETECTCV_H
#define DETECTCV_H

#include "QScreen"
#include "QGuiApplication"
#include "QPixmap"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/objdetect/objdetect.hpp"

using namespace cv;
class DetectCV
{
public:
    DetectCV();
    int detectFace();
    int detectBody();
    int detectMove(String  background);
    void FullScreen(int x, int y, int width, int height);
    /** 全局变量 */
    String PATH = "../../../应用/opencv/opencv-3.3.0/data/haarcascades/";
    String face_cascade_name = PATH + "haarcascade_frontalface_alt.xml";
    CascadeClassifier face_cascade;
    String window_name = "Capture - Face detection";
    int now = 0;
    int past = 0;
};

#endif // DETECTCV_H
