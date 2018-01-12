QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp



INCLUDEPATH+=D:\Qt\Qt5.10.0\openCv2\include\opencv\
                    D:\Qt\Qt5.10.0\openCv2\include\opencv2\
                    D:\Qt\Qt5.10.0\openCv2\include

LIBS+=D:\Qt\Qt5.10.0\openCv2\lib\libopencv_calib3d2413.dll.a\
        D:\Qt\Qt5.10.0\openCv2\lib\libopencv_contrib2413.dll.a\
        D:\Qt\Qt5.10.0\openCv2\lib\libopencv_core2413.dll.a\
        D:\Qt\Qt5.10.0\openCv2\lib\libopencv_features2d2413.dll.a\
        D:\Qt\Qt5.10.0\openCv2\lib\libopencv_flann2413.dll.a\
        D:\Qt\Qt5.10.0\openCv2\lib\libopencv_gpu2413.dll.a\
        D:\Qt\Qt5.10.0\openCv2\lib\libopencv_highgui2413.dll.a\
        D:\Qt\Qt5.10.0\openCv2\lib\libopencv_imgproc2413.dll.a\
        D:\Qt\Qt5.10.0\openCv2\lib\libopencv_legacy2413.dll.a\
        D:\Qt\Qt5.10.0\openCv2\lib\libopencv_ml2413.dll.a\
        D:\Qt\Qt5.10.0\openCv2\lib\libopencv_nonfree2413.dll.a\
        D:\Qt\Qt5.10.0\openCv2\lib\libopencv_objdetect2413.dll.a\
        D:\Qt\Qt5.10.0\openCv2\lib\libopencv_ocl2413.dll.a\
        D:\Qt\Qt5.10.0\openCv2\lib\libopencv_photo2413.dll.a\
        D:\Qt\Qt5.10.0\openCv2\lib\libopencv_stitching2413.dll.a\
        D:\Qt\Qt5.10.0\openCv2\lib\libopencv_ts2413.a\
        D:\Qt\Qt5.10.0\openCv2\lib\libopencv_video2413.dll.a
