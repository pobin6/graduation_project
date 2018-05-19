#include "detectcv.h"

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
/** @函数 detectBody */
int DetectCV::detectBody(Mat frame,vector<Rect> boundRect)
{

//        QString path = "full-" + QString::number(past,10) + ".jpg";
//        Mat frame = imread(path.toStdString());
        std::vector<Rect> bodys;
        HOGDescriptor defaultHog;
        defaultHog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
        //进行检测
        defaultHog.detectMultiScale(frame, bodys);
        //画长方形，框出行人
        for(int i = 0; i < bodys.size(); i++)
        {
                Rect r = bodys[i];
                Point center = heightPoint(r);
                for(int j = 0; j < boundRect.size(); j++)
                {
                    if(center.x > boundRect[j].x
                            && center.x < boundRect[j].x+boundRect[j].width
                            && center.y > boundRect[j].y
                            && center.y < boundRect[j].y+boundRect[j].height
                            && boundRect[i].width * boundRect[i].height > 5000)
                    {
                        detectDirec2(boundRect[j]);
                        rectangle(frame, r, Scalar(0, 0, 255), 2);
                    }
                }
        }
        //-- 显示结果图像
        imshow( window_name, frame );
        detecNum();
        return 0;
}
/** @function detectMove*/
int DetectCV::detectMove(String background)
{
        QString path = "full-" + QString::number(past,10) + ".jpg";
        Mat back = imread(path.toStdString());
//        Mat back = imread(background);
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
//        imshow("diff", diff);
        //3.对差值图diff_thresh进行阈值化处理
        Mat diff_thresh;
        threshold(diff, diff_thresh, 50, 255, CV_THRESH_BINARY);
//        imshow("diff_thresh", diff_thresh);
        //4.腐蚀
        Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(3, 3));
        Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(15, 15));
        erode(diff_thresh, diff_thresh, kernel_erode);
//        imshow("erode", diff_thresh);
        //5.膨胀
        dilate(diff_thresh, diff_thresh, kernel_dilate);
//        imshow("dilate", diff_thresh);
        //6.查找轮廓并绘制轮廓
        vector<vector<Point>> contours;
        findContours(diff_thresh, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//        drawContours(result, contours, -1, Scalar(0, 0, 255), 2);//在result上绘制轮廓
        //7.查找正外接矩形
        vector<Rect> boundRect(contours.size());
        for (int i = 0; i < contours.size(); i++)
        {
                boundRect[i] = boundingRect(contours[i]);
        }
        detectBody(result, boundRect);
//        imshow(window_name, result);
//        detecNum();
        return contours.size();//返回result
}

//get height point
Point DetectCV::heightPoint(Rect r)
{
    Point hp;
    hp.x = r.x+r.width/2;
    hp.y = r.y+r.height/2;
    return hp;
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
        if(!screen->grabWindow(0, x, y, width, height).save(filePathName, "jpg"))
        {
              cout<<"save full screen failed"<<endl;
        }
}

//get person nd
MatND DetectCV::detectND(Mat img)
{
        Mat hsv_base;
        // 转换到 HSV
        cvtColor( img, hsv_base, CV_BGR2HSV );
        /// 对hue通道使用30个bin,对saturatoin通道使用32个bin
        int h_bins = 50; int s_bins = 60;
        int histSize[] = { h_bins, s_bins };
        // hue的取值范围从0到256, saturation取值范围从0到180
        float h_ranges[] = { 0, 256 };
        float s_ranges[] = { 0, 180 };
        const float* ranges[] = { h_ranges, s_ranges };
        // 使用第0和第1通道
        int channels[] = { 0, 1 };
        /// 直方图
        MatND hist_base;
        /// 计算HSV图像的直方图
        calcHist( &hsv_base, 1, channels, Mat(), hist_base, 2, histSize, ranges, true, false );
        normalize( hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat() );
        return hist_base;
}

//get direction by move
void DetectCV::detectDirec2(Rect bound)
{
    int j = 0;
    for(j = 0; j < per_direc.size(); j++)
    {
        Rect a,b,c;
        a = bound;
        b = per_direc[j].bound;
        //get repeat size
        if((a.x+a.width>b.x) && (a.y+a.height>b.y)&&(b.x+b.width>a.x) && (b.y+b.height>a.y))
        {
            if(a.x > b.x) c.x = a.x;
            else c.x = b.x;
            if(a.y > b.y) c.y = a.y;
            else c.y = b.y;
            if((a.x+a.width) > (b.x+b.width)) c.width = b.x+b.width - c.x;
            else c.width = a.x+a.width - c.x;
            if((a.y+a.height) > (b.y+b.height)) c.height = b.y+b.height - c.y;
            else c.height = a.y+a.height - c.y;

            int S = c.width * c.height;
            int as = a.width * a.height;
            if(as < S * 3)
            {
                per_direc[j].x2 = a.x;
                per_direc[j].isAppear = true;
                per_direc[j].preNum++;
                break;
            }
        }
    }
    if(j == per_direc.size())
    {
        Person p;
        p.x1 = bound.x;
        p.x2 = bound.x;
        p.bound = bound;
        per_direc.append(p);
    }
}

//get person number
void DetectCV::detecNum()
{
        int n = 0;
        for(int i=0; i<per_direc.length(); i++)
        {
            if(per_direc[i].isAppear) per_direc[i].times = 9;
            else per_direc[i].times -=1;
            per_direc[i].isAppear = false;
            if(per_direc[i].num < per_direc[i].preNum) per_direc[i].num = per_direc[i].preNum;
            per_direc[i].preNum = 0;
            if(per_direc[i].times < 0)
            {
                    if(per_direc[i].x1 > per_direc[i].x2) per_num += per_direc[i].num;
                    else if(per_direc[i].x1 < per_direc[i].x2) per_num -= per_direc[i].num;
                    per_direc.removeAt(i-n);
                    n++;
            }
        }
}
