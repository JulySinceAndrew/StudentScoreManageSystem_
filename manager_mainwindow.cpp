#include "manager_mainwindow.h"
#include "ui_manager_mainwindow.h"
#include<fstream>
#include<QFileDialog>
#include<string>
#include<QMessageBox>
#include<sstream>
#include<QValidator>
#include<QRegExp>

#define row_height 30
#define top_height 50
#define heiht_max 380

using std::ostringstream;
using std::istringstream;
using std::string;
using std::fstream;
using std::ios_base;

Manager_MainWindow::Manager_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Manager_MainWindow)
{
    ui->setupUi(this);
    setFixedSize(767,523);
    now_page=0;
    now_state=state_welcome;
    set_welcome_visible(true);
    set_serach_visible(false);
    set_studenttable_visible(false);
    ui->table->setVisible(false);
    //open_student_file();
    //open_teacher_file();
    open_lesson_file();

}

Manager_MainWindow::~Manager_MainWindow()
{
    delete ui;
}

void Manager_MainWindow::set_welcome_visible(bool arg)
{
    ui->label_welcome_fixed->setVisible(arg);
    ui->label_welcome_fixed2->setVisible(arg);
    ui->label_welcome_change->setVisible(arg);
}

void Manager_MainWindow::set_serach_visible(bool arg)
{
    ui->label_search->setVisible(arg);
    ui->comboBox_search->setVisible(arg);
    ui->lineEdit_search->setVisible(arg);
    ui->pushButton_search->setVisible(arg);
    ui->pushButton_lookall->setVisible(arg);
}

void Manager_MainWindow::set_studenttable_visible(bool arg)
{
    ui->table_student->setVisible(arg);
    ui->table_student_total->setVisible(arg);
    if(arg==false)
    {
        int count=ui->table_student->rowCount();
        for(int i=0;i<count;i++)
            ui->table_student->removeRow(0);
    }
    else
    {
        //打开学生
    }

}

void Manager_MainWindow::set_table_visivle(bool arg)
{
    ui->table->setVisible(arg);
    if(arg==false)
    {
        int count=ui->table->rowCount();
        for(int i=0;i<count;i++)
            ui->table->removeRow(0);
    }
}

void Manager_MainWindow::open_file()
{

}

void Manager_MainWindow::open_student_file()
{
    fstream fin;
    fstream finstu;
    fin.open("studentlist.bin",ios_base::in);
    int count;
    fin.read((char*)(&count),sizeof(int));
    QString prefix="stu_";
    QString suffix=".bin";
    for(int i=0;i<count;i++)
    {
        long fileid;
        fin.read((char*)(&fileid),sizeof(long));
        ostringstream *os=new ostringstream;
        *os<<fileid;
        QString filename=prefix+QString::fromStdString(os->str())+suffix;
        delete os;
        finstu.open(filename.toStdString(),ios_base::in);
        char* name;
        int size;
        finstu.read((char*)(&size),4);
        name=new char[size+1];
        finstu.read(name,size);
        name[size]='\0';
        long id;
        finstu.read((char*)(&id),sizeof(long));
        bool sex;
        finstu.read((char*)(&sex),sizeof(bool));
        int n;
        finstu.read((char*)(&n),sizeof(int));
        student.add(Student(id,name,sex));
        long lessonid;
        for(int i=0;i<n;i++)
        {
            finstu.read((char*)(&lessonid),sizeof(long));
            student[i].lessonID.add(lessonid);
        }
        //qDebug()<<student[i].ID()<<student[i].name()<<student[i].sex();
        finstu.close();
        delete []name;
    }

}

void Manager_MainWindow::open_teacher_file()
{
    fstream fin;
    fstream finstu;
    fin.open("teacherlist.bin",ios_base::in);
    int count;
    fin.read((char*)(&count),sizeof(int));
    QString prefix="tea_";
    QString suffix=".bin";
    for(int i=0;i<count;i++)
    {
        long fileid;
        fin.read((char*)(&fileid),sizeof(long));
        ostringstream *os=new ostringstream;
        *os<<fileid;
        QString filename=prefix+QString::fromStdString(os->str())+suffix;
        delete os;
        finstu.open(filename.toStdString(),ios_base::in);
        char* name;
        int size;
        finstu.read((char*)(&size),4);
        name=new char[size+1];
        finstu.read(name,size);
        name[size]='\0';
        long id;
        finstu.read((char*)(&id),sizeof(long));
        bool sex;
        finstu.read((char*)(&sex),sizeof(bool));
        int n;
        finstu.read((char*)(&n),sizeof(int));
        teacher.add(Teacher(id,name,sex));
        long lessonid;
        for(int i=0;i<n;i++)
        {
            finstu.read((char*)(&lessonid),sizeof(long));
            teacher[i].lessonID.add(lessonid);
        }
        qDebug()<<teacher[i].ID()<<teacher[i].name()<<teacher[i].sex();
        finstu.close();
        delete []name;
    }

}

void Manager_MainWindow::open_lesson_file()
{
    QString prefix="les_";
    QString houzhui=".bin";
    QString filename;
    fstream fin;
    fstream finles;
    fin.open("lessonlist.bin",ios_base::in);
    int count;
    long fileid;
    fin.read((char*)(&count),4);
    for(int i=0;i<count;i++)
    {
        fin.read((char*)(&fileid),sizeof(long));qDebug()<<fileid;
        ostringstream *os=new ostringstream;
        *os<<fileid;
        QString  s=QString::fromStdString(os->str());
        filename=prefix+s+houzhui;
        finles.open(filename.toStdString(),ios_base::in);
        char* name;
        int size;
        finles.read((char*)(&size),4);qDebug()<<size;
        name=new char[size+1];
        finles.read(name,size);
        name[size]='\0';
        long id;
        finles.read((char*)(&id),sizeof(long));
        int credit;
        finles.read((char*)(&credit),sizeof(int));
        long teaid;
        finles.read((char*)(&teaid),sizeof(long));
        lesson.add(Lesson(id,name,credit,teaid));
        int n;
        finles.read((char*)(&n),sizeof(int));
        long stuid;
        int scores;
        for(int i=0;i<n;i++)
        {
            finles.read((char*)(&stuid),sizeof(long));
            finles.read((char*)(&scores),sizeof(int));
            lesson[i].stuscore.add(score(stuid,scores));
        }
        qDebug()<<"asdasd"<<lesson[i].ID()<<lesson[i].name()<<lesson[i].credit()<<lesson[i].teacherID();
        finles.close();
        delete []name;
    }
}

QString Manager_MainWindow::long_to_qstr(long n)
{
    ostringstream os;
    os<<n;
    return QString::fromStdString(os.str());
}


void Manager_MainWindow::on_action_student_triggered()
{
    bool flag=ui->action_student->isChecked();
    if(ui->action_edit->isChecked()||ui->action_New->isChecked())
    {
        ui->action_student->setChecked(!flag);
        return ;
    }
    set_welcome_visible(!flag);
    set_serach_visible(flag);
    set_table_visivle(flag);
    if(!flag)
    {
        now_page=0;
        return ;
    }
    now_page=1;
    ui->action_teacher->setChecked(false);
    ui->action_lesson->setChecked(false);
    //打开studentlist
}

void Manager_MainWindow::on_action_teacher_triggered()
{
    bool flag=ui->action_teacher->isChecked();
    if(ui->action_edit->isChecked()||ui->action_New->isChecked())
    {
        ui->action_teacher->setChecked(!flag);
        return ;
    }
    set_welcome_visible(!flag);
    set_serach_visible(flag);
    set_table_visivle(flag);
    if(!flag)
    {
        now_page=0;
        return ;
    }
    now_page=1;
    ui->action_student->setChecked(false);
    ui->action_lesson->setChecked(false);
    //打开tealist
}

void Manager_MainWindow::on_action_lesson_triggered()
{
    bool flag=ui->action_lesson->isChecked();
    if(ui->action_edit->isChecked()||ui->action_New->isChecked())
    {
        ui->action_lesson->setChecked(!flag);
        return ;
    }
    set_welcome_visible(!flag);
    set_serach_visible(flag);
    set_table_visivle(flag);
    if(!flag)
    {
        now_page=0;
        return ;
    }
    now_page=1;
    ui->action_teacher->setChecked(false);
    ui->action_student->setChecked(false);
    //打开lesslist
}

void Manager_MainWindow::on_action_edit_triggered()
{
    bool flag=ui->action_edit->isChecked();
    if(now_page!=2||ui->action_New->isChecked())
    {
        ui->action_edit->setChecked(!flag);
        return ;
    }
    //开始编辑

}

void Manager_MainWindow::on_action_New_triggered()
{
    bool flag=ui->action_New->isChecked();
    if(now_page==0||ui->action_edit->isChecked())
    {
        ui->action_New->setChecked(!flag);
        return ;
    }
    //开始新增

}
