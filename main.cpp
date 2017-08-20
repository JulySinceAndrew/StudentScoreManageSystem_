#include<QApplication>
#include<QDebug>
#include"manager_mainwindow.h"
#include"mainwindow_student.h"
#include"mainwindow_teacher.h"
#include<vector.h>
#include<vector_of.h>
#include"dialog_log.h"
#include<specific_person.h>
#include<lesson.h>
#include<fstream>
#include<sstream>
using namespace std;

int main(int argc,char*argv[])
{
    QApplication app(argc,argv);
    Dialog_Log w;
    //MainWindow_Student w;
    //MainWindow_Teacher w;
    //Manager_MainWindow w;
    w.show();
  /*  fstream fout;
    fout.open("account_manager.bin",ios_base::out|ios_base::trunc);
    int count=1;
    fout.write((char*)(&count),sizeof(int));
    long id;
    id=9540802;
    fout.write((char*)(&id),sizeof(long));
    fout.close();
    fout.open("password_manager.bin",ios_base::out|ios_base::trunc);
    fout.write((char*)(&count),sizeof(int));
    char* p="123456";
    int c=6;
    for(int i=0;i<1;i++)
    {
        fout.write((char*)(&c),sizeof(int));
        fout.write(p,c);
    }
    fout.close();*/
return app.exec();
}
