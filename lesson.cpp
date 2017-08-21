#include"lesson.h"

Lesson::Lesson(long id, QString _name, int c, long teacherid):object(_name,id),_teacherID(teacherid),_credit(c)
{

}

Lesson::Lesson()
{

}

Lesson::~Lesson()
{

}

long Lesson::teacherID()
{
    return _teacherID;
}

int Lesson::credit()
{
    return _credit;
}

bool Lesson::set_ID(long id)
{
    if(!(id>=10000000&&id<=99999999))
        return false;
    setID(id);
    return true;
}

bool Lesson::set_teacherID(long teacherID)
{
    if(!(teacherID>=100000&&teacherID<=999999))
        return false;
    _teacherID=teacherID;
    return true;
}

bool Lesson::set_credit(int credit)
{
    if(!(credit>0&&credit<=10))
        return false;
    _credit=credit;
    return true;
}
