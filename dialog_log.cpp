#include "dialog_log.h"
#include "ui_dialog_log.h"
#include<QMessageBox>
#include<QValidator>
#include<QRegExp>
#include<sstream>
#include<QDebug>
#include<fstream>
#include"mainwindow_student.h"
#include"mainwindow_teacher.h"
#include"manager_mainwindow.h"
#include"dialog_help.h"

using std::fstream;
using std::ios_base;
using std::istringstream;
using std::ostringstream;

long log_student_id;
long log_teacher_id;
long log_manager_id;

Dialog_Log::Dialog_Log(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Log)
{
    ui->setupUi(this);
    ui->comboBox->setEditable(false);
    QRegExp reg("^[0-9a-zA-Z]{6,12}$");
    QValidator *val=new QRegExpValidator(reg,this);
    ui->lineedit_password->setValidator(val);
    QRegExp reg2("20[0-9]{8}");//学号
    QValidator *val2=new QRegExpValidator(reg2,this);
    ui->lineedit_account->setValidator(val2);
    setFixedSize(460,460);
}

Dialog_Log::~Dialog_Log()
{
    delete ui;
}

void Dialog_Log::on_comboBox_currentIndexChanged(int index)
{
    if(index==0)
    {
        QRegExp reg("20[0-9]{8}");//学号
        QValidator *val=new QRegExpValidator(reg,this);
        ui->lineedit_account->setValidator(val);
    }
    else if(index==1)
    {
        QRegExp reg("[1-9][0-9]{5}");//教职工号
        QValidator *val=new QRegExpValidator(reg,this);
        ui->lineedit_account->setValidator(val);
    }
    else if(index==2)
    {
        QRegExp reg("[1-9][0-9]{6}");//管理员账号 设置为7位
        QValidator *val=new QRegExpValidator(reg,this);
        ui->lineedit_account->setValidator(val);
    }
}

void Dialog_Log::on_pushButton_leave_clicked()
{
    int answer=QMessageBox::question(this,"您是否要退出？","请问您是否确认要退出学生成绩管理系用？",QMessageBox::Yes,QMessageBox::No);
    if(answer==QMessageBox::Yes)
        close();
}

void Dialog_Log::on_pushButton_log_clicked()
{
    int now=ui->comboBox->currentIndex();
    if(ui->lineedit_account->text()==QString())
    {
        QMessageBox::warning(this,"账号为空！","账号不能为空，请输入账号!",QMessageBox::Ok);
        return ;
    }
    if(ui->lineedit_password->text()==QString())
    {
        QMessageBox::warning(this,"密码为空！","密码不能为空，请输入密码!",QMessageBox::Ok);
        return ;
    }
    if(ui->lineedit_password->text().toStdString().size()<6)
    {
        QMessageBox::warning(this,"密码位数错误！","密码位数错误，密码位数为6-12位，请重新输入密码!",QMessageBox::Ok);
        return ;
    }
    long id=qstr_to_long(ui->lineedit_account->text());
    QString filename;
    QString filename2;
    if(now==0) //学生
    {
        if(id<2000000000)
        {
            QMessageBox::warning(this,"学生账号范围错误！","学生账号位数错误，学生账号范围为[2000000000,2100000000),请重新输入学生账号",QMessageBox::Ok);
            return ;
        }
        filename="account_student.bin";
        filename2="password_student.bin";
    }
    else if(now==1) //老师
    {
        if(id<100000)
        {
            QMessageBox::warning(this,"教职工账号范围错误！","教职工账号位数错误，教职工账号范围为[100000,1000000),请重新输入教职工账号",QMessageBox::Ok);
            return ;
        }
        filename="account_teacher.bin";
        filename2="password_teacher.bin";
    }
    else if(now==2)//管理员
    {
        if(id<1000000)
        {
            QMessageBox::warning(this,"管理员账号范围错误！","管理员账号位数错误，管理员账号范围为[1000000,10000000),请重新输入管理员账号",QMessageBox::Ok);
            return ;
        }
        filename="account_manager.bin";
        filename2="password_manager.bin";
    }
    long inputid=qstr_to_long(ui->lineedit_account->text());
    fstream fin;
    fin.open(filename.toStdString(),ios_base::in);
    int count;
    fin.read((char*)(&count),sizeof(int));
    int i=0;
    long nowid;
    for(;i<count;i++)
    {
        fin.read((char*)(&nowid),sizeof(long));
        if(nowid==inputid)
            break;
    }
    if(i==count)
    {
        QMessageBox::warning(this,"账号不存在！","您输入的账号不存在，请核实后重新输入！",QMessageBox::Ok);
        return ;
    }
    fin.close();
    char* password;
    int size;
    fin.open(filename2.toStdString(),ios_base::in);
    fin.read((char*)(&size),sizeof(int));
    for(int j=0;j<i;j++)
    {
        fin.read((char*)(&size),sizeof(int));
        password=new char[size+1];
        fin.read(password,size);
        password[size]='\0';
        delete password;
    }
    fin.read((char*)(&size),sizeof(int));
    password=new char[size+1];
    fin.read(password,size);
    password[size]='\0';
    int inputsize=ui->lineedit_password->text().toStdString().size();qDebug()<<size<<inputsize;
    if(size!=inputsize)
    {
        QMessageBox::warning(this,"密码错误！","您输入的账号或密码错误，请重新输入！",QMessageBox::Ok);
        return ;
    }
    const char* inputpassword=ui->lineedit_password->text().toStdString().c_str();
    for(int k=0;k<size+1;k++)
    {qDebug()<<inputpassword[k]<<password[k];
        if(inputpassword[k]!=password[k])
        {
            QMessageBox::warning(this,"密码错误！","您输入的账号或密码错误，请重新输入！",QMessageBox::Ok);
            return ;
        }
    }
    if(now==0)
    {
        log_student_id=id;
        MainWindow_Student* w=new MainWindow_Student;
        w->show();
        close();
        return ;
    }
    else if(now==1)
    {
        log_teacher_id=id;
        MainWindow_Teacher* w=new MainWindow_Teacher;
        w->show();
        close();
        return ;
    }
    else if(now==2)
    {
        log_manager_id=id;
        Manager_MainWindow* w=new Manager_MainWindow;
        w->show();
        close();
        return ;
    }
}

long Dialog_Log::qstr_to_long(QString qs)
{
    istringstream is(qs.toStdString());
    long id;
    is>>id;
    return id;
}


void Dialog_Log::on_pushButton_help_clicked()
{
    Dialog_Help w(this);
    w.exec();
}
