#ifndef VECTOR_OF_STUDENT_H
#define VECTOR_OF_STUDENT_H

#include"vector.h"
#include"specific_person.h"
#include"lesson.h"

class vector_of_student:public Vector<Student>
{
public:
  bool set(long id,QString name,bool sex)
  {
      for(int i=0;i<Vector<Student>::count();i++)
      {
          if(Vector<Student>::operator [](i).ID()==id)
          {
              Vector<Student>::operator [](i).set_name(name);
              Vector<Student>::operator [](i).set_sex(sex);
              return true;
          }
      }
      return false;
  }
  bool remove_ID(long id)
  {
      for(int i=0;i<Vector<Student>::count();i++)
      {
          if(Vector<Student>::operator [](i).ID()==id)
          {
              Vector<Student>::remove_index(i);
              return true;
          }
      }
      return false;
  }
  Student& operator()(long id)
  {
      for(int i=0;i<Vector<Student>::count();i++)
      {
          if(Vector<Student>::operator [](i).ID()==id)
              return Vector<Student>::operator [](i);
      }
      Student *s=new Student;
      return *s;
  }
};

class vector_of_Teacher:public Vector<Teacher>
{
public:
    bool set(long id,QString name,bool sex)
    {
        for(int i=0;i<Vector<Teacher>::count();i++)
        {
            if(Vector<Teacher>::operator [](i).ID()==id)
            {
                Vector<Teacher>::operator [](i).set_name(name);
                Vector<Teacher>::operator [](i).set_sex(sex);
                return true;
            }
        }
        return false;
    }
    bool remove_ID(long id)
    {
        for(int i=0;i<Vector<Teacher>::count();i++)
        {
            if(Vector<Teacher>::operator [](i).ID()==id)
            {
                Vector<Teacher>::remove_index(i);
                return true;
            }
        }
        return false;
    }
    Teacher& operator()(long id)
    {
        for(int i=0;i<Vector<Teacher>::count();i++)
        {
            if(Vector<Teacher>::operator [](i).ID()==id)
                return Vector<Teacher>::operator [](i);
        }
        Teacher *s=new Teacher;
        return *s;
    }
};

class vector_of_Lesson:public Vector<Lesson>
{
public:
  bool set(long id,QString name,long teacherID,int credit)
  {
      for(int i=0;i<Vector<Lesson>::count();i++)
      {
          if(Vector<Lesson>::operator [](i).ID()==id)
          {
              Vector<Lesson>::operator [](i).set_name(name);
              Vector<Lesson>::operator [](i).set_teacherID(teacherID);
              Vector<Lesson>::operator [](i).set_credit(credit);
              return true;
          }
      }
      return false;
  }
  bool remove_ID(long id)
  {
      for(int i=0;i<Vector<Lesson>::count();i++)
      {
          if(Vector<Lesson>::operator [](i).ID()==id)
          {
              Vector<Lesson>::remove_index(i);
              return true;
          }
      }
      return false;
  }
  Lesson& operator()(long id)
  {
      for(int i=0;i<Vector<Lesson>::count();i++)
      {
          if(Vector<Lesson>::operator [](i).ID()==id)
              return Vector<Lesson>::operator [](i);
      }
      Lesson *s=new Lesson;
      return *s;
  }
};

#endif // VECTOR_OF_STUDENT_H
