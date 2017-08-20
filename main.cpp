#include<QApplication>
#include<QDebug>
#include"manager_mainwindow.h"
#include"mainwindow_student.h"
#include<vector.h>
#include<vector_of.h>
#include<specific_person.h>
#include<lesson.h>
#include<fstream>
#include<sstream>
using namespace std;

int main(int argc,char*argv[])
{
    QApplication app(argc,argv);
    MainWindow_Student w;
    //Manager_MainWindow w;
    w.show();
   /* fstream fout;
    fout.open("lessonlist.bin",ios_base::out|ios_base::trunc);
    if(!fout)
        qDebug()<<"yemeiyou";
    int a=3;
    fout.write((char*)(&a),4);
    long id;
    for(int i=0;i<3;i++)
    {
        id=20000000+i;qDebug()<<id;
        fout.write((char*)(&id),sizeof(long));
    }
    fout.close();
    fstream fin;
    fin.open("lessonlist.bin",ios_base::in);
            fin.read((char*)(&id),4);
    for(int i=0;i<3;i++)
    {
        fin.read((char*)(&id),sizeof(long));
        qDebug()<<"asdasd"<<id;
    }*/
   /* QString prefix="les_";
    QString houzhui=".bin";
    QString filename;
    fstream fin;
    fstream fout;
    fin.open("lessonlist.bin",ios_base::in);
    int count;
            long n;
    fin.read((char*)(&count),4);
    for(int i=0;i<count;i++)
    {
        fin.read((char*)(&n),sizeof(long));qDebug()<<n;
        ostringstream *os=new ostringstream;
        *os<<n;
        QString  s=QString::fromStdString(os->str());
        filename=prefix+s+houzhui;
        fout.open(filename.toStdString(),ios_base::in);
        char* name;
        int size;
        fout.read((char*)(&size),4);qDebug()<<size;
        name=new char[size+1];
        fout.read(name,size);
        name[size]='\0';
        long id;
        fout.read((char*)(&id),sizeof(long));
        int credit;
        fout.read((char*)(&credit),sizeof(int));
        long teaid;
        fout.read((char*)(&teaid),sizeof(long));
        int n;
        fout.read((char*)(&n),sizeof(int));
        qDebug()<<"!"<<QString(name)<<id<<credit<<teaid<<n;
        fout.close();
        delete []name;

        string name="微积分";
        int size=name.size();qDebug()<<size;
        fout.write((char*)(&size),4);qDebug()<<name.c_str();
        fout.write(name.c_str(),size);
        long id;
        id=20000000+i;
        fout.write((char*)(&id),sizeof(long));
        int credit=5;
        fout.write((char*)(&credit),sizeof(int));
        long teaid=100000+i;
        fout.write((char*)(&teaid),sizeof(long));
        int number=0;
        fout.write((char*)(&number),sizeof(int));
        fout.close();
        delete os;
    }*/
return app.exec();
}
