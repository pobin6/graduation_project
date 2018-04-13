#include "detectcv.h"
#include "iostream"

using namespace std;
DetectCV::DetectCV()
{
        //-- 1. 加载级联分类器文件
        if( !face_cascade.load( face_cascade_name ) )
        {
                printf("--(!)Error loading\n");
                cout << face_cascade_name << endl;
        }
}

/** @函数 detectFace */
int DetectCV::detectFace( )
{
        std::vector<Rect> faces;
        Mat frame_gray;
        QString path = "full-" + QString::number(past,10) + ".jpg";
        Mat frame = imread(path.toStdString());
        cvtColor( frame, frame_gray, CV_BGR2GRAY );
        equalizeHist( frame_gray, frame_gray );

        //-- 多尺寸检测人脸
        face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

        for( int i = 0; i < faces.size(); i++ )
        {
                Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
                ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
        }
        //-- 显示结果图像
        imshow( window_name, frame );
        return faces.size();
}
/** @函数 detectFace */
int DetectCV::detectBody()
{

        QString path = "full-" + QString::number(past,10) + ".jpg";
        Mat frame = imread(path.toStdString());
        std::vector<Rect> bodys;
        HOGDescriptor defaultHog;
        defaultHog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
        //进行检测
        defaultHog.detectMultiScale(frame, bodys);
        //画长方形，框出行人
        for(int i = 0; i < bodys.size(); i++)
        {
                Rect r = bodys[i];
                rectangle(frame, r.tl(), r.br(), Scalar(0, 0, 255), 3);
        }
        //-- 显示结果图像
        imshow( window_name, frame );
        return bodys.size();
}
/** @function detectMove*/
int DetectCV::detectMove(String background)
{
        QString path = "full-" + QString::number(past,10) + ".jpg";
        Mat back = imread(path.toStdString());
        path = "full-" + QString::number(now,10) + ".jpg";
        Mat frame = imread(path.toStdString());
        Mat result = frame.clone();
        //1.将background和frame转为灰度图
        Mat gray1, gray2;
        cvtColor(back, gray1, CV_BGR2GRAY);
        cvtColor(frame, gray2, CV_BGR2GRAY);
        //2.将background和frame做差
        Mat diff;
        absdiff(gray1, gray2, diff);
        imshow("diff", diff);
        //3.对差值图diff_thresh进行阈值化处理
        Mat diff_thresh;
        threshold(diff, diff_thresh, 50, 255, CV_THRESH_BINARY);
        imshow("diff_thresh", diff_thresh);
        //4.腐蚀
        Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(3, 3));
        Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(15, 15));
        erode(diff_thresh, diff_thresh, kernel_erode);
        imshow("erode", diff_thresh);
        //5.膨胀
        dilate(diff_thresh, diff_thresh, kernel_dilate);
        imshow("dilate", diff_thresh);
        //6.查找轮廓并绘制轮廓
        vector<vector<Point>> contours;
        findContours(diff_thresh, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
        drawContours(result, contours, -1, Scalar(0, 0, 255), 2);//在result上绘制轮廓
        //7.查找正外接矩形
        vector<Rect> boundRect(contours.size());
        for (int i = 0; i < contours.size(); i++)
        {
                boundRect[i] = boundingRect(contours[i]);
                rectangle(result, boundRect[i], Scalar(0, 255, 0), 2);//在result上绘制正外接矩形
        }
        imshow(window_name, result);
        return contours.size();//返回result
}

//get fullscreen picture
void DetectCV::FullScreen(int x, int y, int width, int height)
{
        past = now;
        now = ++now%3;
        QScreen *screen = QGuiApplication::primaryScreen();
        QString filePathName = "full-";
        filePathName += QString::number(now,10);
        filePathName += ".jpg";
        cout << x << endl;
        if(!screen->grabWindow(0, x, y, width, height).save(filePathName, "jpg"))
        {
              cout<<"save full screen failed"<<endl;
        }
}

