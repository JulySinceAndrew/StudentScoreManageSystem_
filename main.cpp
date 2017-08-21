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
    Dialog_Log w; //新建登录界面
    w.show(); //打开登录界面
    return app.exec(); //进入QT时间循环
}
