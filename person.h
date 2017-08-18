#ifndef PERSON_H
#define PERSON_H

#define man true
#define woman false

#include"object.h"
#include"vector.h"

class Person:public object
{
private:
    bool _sex;

public:
    Person(long id,QString _n,bool s):object(_n,id),_sex(s) {}
    Vector<long> lessonID;
    Person(){}
    ~Person(){}
    void set_sex(bool s){_sex=s;}
    bool sex(){return _sex;}
};

#endif // PERSON_H
