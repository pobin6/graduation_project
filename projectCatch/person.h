#ifndef PERSON_H
#define PERSON_H

#include "iostream"
#include <QMap>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;
class Person
{
public:
    Person();
    int x1= 0;
    int x2= 0;
    int num = 1;
    int preNum = 0;
    int times = 9;
    int appearTimes = 1;
    bool isAppear = true;
    Rect bound;
};

#endif // PERSON_H
