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


INCLUDEPATH+=D:\Qt\Qt5.10.0\openCv\include\opencv\
                    D:\Qt\Qt5.10.0\openCv\include\opencv2\
                    D:\Qt\Qt5.10.0\openCv\include

LIBS+=D:\Qt\Qt5.10.0\openCv\lib\libopencv_calib3d310.dll.a\
        D:\Qt\Qt5.10.0\openCv\lib\libopencv_core310.dll.a\
        D:\Qt\Qt5.10.0\openCv\lib\libopencv_features2d310.dll.a\
        D:\Qt\Qt5.10.0\openCv\lib\libopencv_flann310.dll.a\
        D:\Qt\Qt5.10.0\openCv\lib\libopencv_highgui310.dll.a\
        D:\Qt\Qt5.10.0\openCv\lib\libopencv_imgproc310.dll.a\
        D:\Qt\Qt5.10.0\openCv\lib\libopencv_imgcodecs310.dll.a\
        D:\Qt\Qt5.10.0\openCv\lib\libopencv_ml310.dll.a\
        D:\Qt\Qt5.10.0\openCv\lib\libopencv_objdetect310.dll.a\
        D:\Qt\Qt5.10.0\openCv\lib\libopencv_photo310.dll.a\
        D:\Qt\Qt5.10.0\openCv\lib\libopencv_shape310.dll.a\
        D:\Qt\Qt5.10.0\openCv\lib\libopencv_stitching310.dll.a\
        D:\Qt\Qt5.10.0\openCv\lib\libopencv_superres310.dll.a\
        D:\Qt\Qt5.10.0\openCv\lib\libopencv_ts310.a\
        D:\Qt\Qt5.10.0\openCv\lib\libopencv_video310.dll.a\
        D:\Qt\Qt5.10.0\openCv\lib\libopencv_videoio310.dll.a\
        D:\Qt\Qt5.10.0\openCv\lib\libopencv_videostab310.dll.a
