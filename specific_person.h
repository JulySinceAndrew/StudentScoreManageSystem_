#ifndef STUDENT_H
#define STUDENT_H

#include<person.h>

// studentID range 2000000000-2100000000
// teacherID range 100000-999999

class Student:public Person
{
public:
    Student(long id,QString _n,bool s):Person(id,_n,s){}
    Student(){}
    ~Student(){}
    bool set_ID(long ID)
    {
        if(!(ID>=2000000000&&ID<2100000000))
            return false;
        setID(ID);
        return true;
    }
};

class Teacher:public Person
{
public:
    Teacher(long id,QString _n,bool s):Person(id,_n,s){}
    Teacher(){}
    ~Teacher(){}
    bool set_ID(long ID)
    {
        if(!(ID>=100000&&ID<=999999))
            return false;
        setID(ID);
        return true;
    }
};

#endif // STUDENT_H
