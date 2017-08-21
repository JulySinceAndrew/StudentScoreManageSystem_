#include<QApplication>
#include"dialog_log.h"

int main(int argc,char*argv[])
{
    QApplication app(argc,argv);
    Dialog_Log w; //新建登录界面
    w.show(); //打开登录界面
    return app.exec(); //进入QT事件循环
}
