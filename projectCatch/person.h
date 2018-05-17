#ifndef PERSON_H
#define PERSON_H


class Person
{
public:
    Person(int x, int y, int size);
    int size = 0;
    int x = 0;
    int y = 0;
    int direc = 0;
    int lasttimes = 9;
    bool isAppear = false;
};

#endif // PERSON_H
