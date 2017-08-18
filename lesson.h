#ifndef LESSON_H
#define LESSON_H

//LessonID range 8位数 10000000-99999999
#include"object.h"
class score
{
public:
    long studentID;
    int _score;
public:
    score(long id,int s):studentID(id),_score(s)
    {
    }
    score(){}
};
class Lesson:public object
{
private:
    long _teacherID;
    int _credit;
public:
    Vector<score> score;
    Lesson(long id,QString _name,int c,long teacherid);
    Lesson();
    ~Lesson();
    long teacherID();
    int credit();
    bool set_ID(long id);
    bool set_teacherID(long teacherID);
    bool set_credit(int credit);
};

#endif // LESSON_H
