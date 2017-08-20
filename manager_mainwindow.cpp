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

bool sort_modal=up;

Manager_MainWindow::Manager_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Manager_MainWindow)
{
    ui->setupUi(this);
    setFixedSize(767,523);
    now_page=0;
    now_state=state_welcome;
    set_all_visible(false);
    set_welcome_visible(true);
    open_file();
    QLabel* label_takeupspace=new QLabel(this);
    label_takeupspace->setFixedSize(32,32);
    label_takeupspace->setPixmap(QPixmap(":/image/images/学生成绩管理系统.png"));
    label_takeupspace->setScaledContents(true);
    ui->toolBar_3->insertWidget(ui->action_studentnumber,label_takeupspace);
    cp_lesscore=NULL;
    cp_stuscore=NULL;
    is_searching=false;
    /*QActionGroup *group=new QActionGroup(this);
    QMenu* sortmenu=new QMenu(this);
    QAction *up=group->addAction("升序");
    QAction *down=group->addAction("降序");
    up->setCheckable(true);
    down->setCheckable(true);
    up->setChecked(true);
    sortmenu->addAction(up);
    sortmenu->addAction(down);
    ui->action_setsort->setMenu(sortmenu);*/
  /*  student[0].lessonID.add(20000000);lesson(20000000).stuscore.add(score(2016011073,100)); //weijifen
    student[0].lessonID.add(20000001);lesson(20000001).stuscore.add(score(2016011073,95));  //dawu
    student[0].lessonID.add(20000002);lesson(20000002).stuscore.add(score(2016011073,92));  //xiandai
    student[1].lessonID.add(20000000);lesson(20000000).stuscore.add(score(2016000012,92)); //weijifen
    student[1].lessonID.add(20000001);lesson(20000001).stuscore.add(score(2016000012,89));  //dawu
    student[1].lessonID.add(20000002);lesson(20000002).stuscore.add(score(2016000012,87));  //xiandai
    student[1].set_ID(2016000012),student[1].set_name("张叶");
    student[2].lessonID.add(20000000);lesson(20000000).stuscore.add(score(2016000013,93)); //weijifen
    student[2].lessonID.add(20000001);lesson(20000001).stuscore.add(score(2016000013,97));  //dawu
    student[2].lessonID.add(20000002);lesson(20000002).stuscore.add(score(2016000013,96));  //xiandai
    student[2].set_ID(2016000013),student[2].set_name("王舒"),student[2].set_sex(woman);
    teacher[0].lessonID.add(20000000);
    teacher[1].lessonID.add(20000001),teacher[1].set_name("安宇");
    teacher[2].lessonID.add(20000002),teacher[2].set_name("瞿燕辉");
    lesson[0].set_teacherID(100000);
    lesson[1].set_teacherID(100001),lesson[1].set_name("大学物理"),lesson[1].set_credit(4);
    lesson[2].set_teacherID(100002),lesson[2].set_name("线性代数"),lesson[2].set_credit(2);*/
    //save_file();
    //open_student_file();
    //open_teacher_file();
    //open_lesson_file();
}

Manager_MainWindow::~Manager_MainWindow()
{
    delete ui;
}

void Manager_MainWindow::closeEvent(QCloseEvent *event)
{
    save_file();
}

void Manager_MainWindow::set_welcome_visible(bool arg)
{
    ui->label_welcome_fixed->setVisible(arg);
    ui->label_welcome_fixed2->setVisible(arg);
    ui->label_welcome_change->setVisible(arg);
}

void Manager_MainWindow::set_studenttable_visible(bool arg)
{
    ui->table_student->setVisible(arg);
    ui->table_student_total->setVisible(arg);
}

void Manager_MainWindow::set_teachertable_visible(bool arg)
{
    ui->table_teacher_total->setVisible(arg);
    ui->table_teacher->setVisible(arg);
}

void Manager_MainWindow::set_lessontable_visible(bool arg)
{
    ui->table_lesson->setVisible(arg);
    ui->table_lesson_total->setVisible(arg);
}

void Manager_MainWindow::set_table_visivle(bool arg)
{
    ui->table->setVisible(arg);
    /*if(arg==false)
    {
        int count=ui->table->rowCount();
        for(int i=0;i<count;i++)
            ui->table->removeRow(0);
    }*/
}

void Manager_MainWindow::set_student_editable(bool arg)
{
    QLineEdit* lineedit;
    lineedit=(QLineEdit*)ui->table_student_total->cellWidget(0,0);
    lineedit->setReadOnly(!arg);
    QComboBox* combobox;
    combobox=(QComboBox*)ui->table_student_total->cellWidget(0,2);
    combobox->setEnabled(arg);
    for(int i=0;i<ui->table_student->rowCount();i++)
    {
        lineedit=(QLineEdit*)ui->table_student->cellWidget(i,4);
        lineedit->setReadOnly(!arg);
    }
}

void Manager_MainWindow::set_teacher_editable(bool arg)
{
    QLineEdit* lineedit;
    lineedit=(QLineEdit*)ui->table_teacher_total->cellWidget(0,0);
    lineedit->setReadOnly(!arg);
    QComboBox* combobox;
    combobox=(QComboBox*)ui->table_teacher_total->cellWidget(0,2);
    combobox->setEnabled(arg);
}

void Manager_MainWindow::set_lesson_editable(bool arg)
{
    QLineEdit* lineedit;
    lineedit=(QLineEdit*)ui->table_lesson_total->cellWidget(0,0);
    lineedit->setReadOnly(!arg);
    lineedit=(QLineEdit*)ui->table_lesson_total->cellWidget(3,0);
    lineedit->setReadOnly(!arg);
    lineedit=(QLineEdit*)ui->table_lesson_total->cellWidget(4,0);
    lineedit->setReadOnly(!arg);
    for(int i=0;i<ui->table_lesson->rowCount();i++)
    {
        lineedit=(QLineEdit*)ui->table_lesson->cellWidget(i,2);
        lineedit->setReadOnly(!arg);
    }
}

void Manager_MainWindow::update_student()
{
    QLineEdit* lineedit;
    lineedit=(QLineEdit*)ui->table_student_total->cellWidget(0,0);
    student_object->set_name(lineedit->text());
    QComboBox* combobox;
    combobox=(QComboBox*)ui->table_student_total->cellWidget(0,2);
    if(combobox->currentIndex()==0)
        student_object->set_sex(man);
    else
        student_object->set_sex(woman);
    for(int i=0;i<ui->table_student->rowCount();i++)
    {
        lineedit=(QLineEdit*)ui->table_student->cellWidget(i,1);
        Lesson& lesnow=lesson(qstr_to_long(lineedit->text()));
        lineedit=(QLineEdit*)ui->table_student->cellWidget(i,4);
        for(int j=0;j<lesnow.stuscore.count();j++)
        {
            if(lesnow.stuscore[j].studentID==student_object->ID())
            {
                lesnow.stuscore[j]._score=qstr_to_int(lineedit->text());
                break;
            }
        }
    }
}

void Manager_MainWindow::update_teacher()
{
    QLineEdit* lineedit=(QLineEdit*)ui->table_teacher_total->cellWidget(0,0);
    teacher_object->set_name(lineedit->text());
    QComboBox* com=(QComboBox*)ui->table_teacher_total->cellWidget(0,2);
    if(com->currentIndex()==0)
        teacher_object->set_sex(man);
    else
        teacher_object->set_sex(woman);
}

void Manager_MainWindow::update_lesson()
{
    QLineEdit* lineedit;
    lineedit=(QLineEdit*)ui->table_lesson_total->cellWidget(0,0);
    lesson_object->set_name(lineedit->text());
    lineedit=(QLineEdit*)ui->table_lesson_total->cellWidget(3,0);
    long lasttea=lesson_object->teacherID();
    long nowtea=qstr_to_long(lineedit->text());
    if(lasttea!=nowtea)
    {
        lesson_object->set_teacherID(nowtea);
        teacher(lasttea).lessonID.remove_value(lesson_object->ID());
        teacher(nowtea).lessonID.add(lesson_object->ID());
    }
    lineedit=(QLineEdit*)ui->table_lesson_total->cellWidget(4,0);
    lesson_object->set_credit(qstr_to_int(lineedit->text()));
    long stuid;
    for(int i=0;i<ui->table_lesson->rowCount();i++)
    {
        lineedit=(QLineEdit*)ui->table_lesson->cellWidget(i,1);
        stuid=qstr_to_long(lineedit->text());
        lineedit=(QLineEdit*)ui->table_lesson->cellWidget(i,2);
        for(int j=0;j<lesson_object->stuscore.count();j++)
        {
            if(stuid==lesson_object->stuscore[j].studentID)
            {
                lesson_object->stuscore[j]._score=qstr_to_int(lineedit->text());
                break;
            }
        }
    }
}

void Manager_MainWindow::open_file()
{
    open_student_file();
    open_teacher_file();
    open_lesson_file();
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
        for(int j=0;j<n;j++)
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
        for(int j=0;j<n;j++)
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
        for(int j=0;j<n;j++)
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

void Manager_MainWindow::save_student_file()
{qDebug()<<"begin";
    fstream fout;
    fout.open("studentlist.bin",ios_base::out|ios_base::trunc);
    int count=student.count();
    long id;
    fout.write((char*)(&count),sizeof(int));
    for(int i=0;i<count;i++)
    {
        id=student[i].ID();qDebug()<<id;
        fout.write((char*)(&id),sizeof(long));
    }
    fout.close();
    QString prefix="stu_";
    QString suffix=".bin";
    QString filename;
    long stuid,lesid;
    bool sex;
    int lescount;
    for(int i=0;i<count;i++)
    {
        id=student[i].ID();
        filename=prefix+long_to_qstr(id)+suffix;
        fout.open(filename.toStdString(),ios_base::out|ios_base::trunc);
        QString name=student[i].name();
        int size=name.toStdString().size();
        fout.write((char*)(&size),sizeof(int));qDebug()<<size;
        fout.write(name.toStdString().c_str(),size);qDebug()<<name.toStdString().c_str();
        stuid=student[i].ID();
        fout.write((char*)(&stuid),sizeof(long));qDebug()<<stuid;
        sex=student[i].sex();
        fout.write((char*)(&sex),sizeof(bool));qDebug()<<sex;
        lescount=student[i].lessonID.count();
        fout.write((char*)(&lescount),sizeof(int));qDebug()<<lescount;
        for(int j=0;j<lescount;j++)
        {
            lesid=student[i].lessonID[j];
            fout.write((char*)(&lesid),sizeof(long));qDebug()<<stuid<<":"<<lesid;
        }
        fout.close();
    }qDebug()<<"end";
}

void Manager_MainWindow::save_teacher_file()
{
    fstream fout;
    fout.open("teacherlist.bin",ios_base::out|ios_base::trunc);
    int count=teacher.count();
    long id;
    fout.write((char*)(&count),sizeof(int));
    for(int i=0;i<count;i++)
    {
        id=teacher[i].ID();
        fout.write((char*)(&id),sizeof(long));
    }
    fout.close();
    QString prefix="tea_";
    QString suffix=".bin";
    QString filename;
    long teaid,lesid;
    bool sex;
    int lescount;
    for(int i=0;i<count;i++)
    {
        id=teacher[i].ID();
        filename=prefix+long_to_qstr(id)+suffix;
        fout.open(filename.toStdString(),ios_base::out|ios_base::trunc);
        QString name=teacher[i].name();
        int size=name.toStdString().size();
        fout.write((char*)(&size),sizeof(int));
        fout.write(name.toStdString().c_str(),size);
        teaid=teacher[i].ID();
        fout.write((char*)(&teaid),sizeof(long));
        sex=teacher[i].sex();
        fout.write((char*)(&sex),sizeof(bool));
        lescount=teacher[i].lessonID.count();
        fout.write((char*)(&lescount),sizeof(int));
        for(int j=0;j<lescount;j++)
        {
            lesid=teacher[i].lessonID[j];
            fout.write((char*)(&lesid),sizeof(long));
        }
        fout.close();
    }
}

void Manager_MainWindow::save_lesson_file()
{
    fstream fout;
    fout.open("lessonlist.bin",ios_base::out|ios_base::trunc);
    int count=lesson.count();
    long id;
    fout.write((char*)(&count),sizeof(int));
    for(int i=0;i<count;i++)
    {
        id=lesson[i].ID();
        fout.write((char*)(&id),sizeof(long));
    }
    fout.close();
    QString prefix="les_";
    QString suffix=".bin";
    QString filename;
    long lesid,stuid,teaid;
    int credit,score_;
    int stucount;
    for(int i=0;i<count;i++)
    {
        id=lesson[i].ID();
        filename=prefix+long_to_qstr(id)+suffix;
        fout.open(filename.toStdString(),ios_base::out|ios_base::trunc);
        QString name=lesson[i].name();
        int size=name.toStdString().size();
        fout.write((char*)(&size),sizeof(int));
        fout.write(name.toStdString().c_str(),size);
        lesid=lesson[i].ID();
        fout.write((char*)(&lesid),sizeof(long));
        credit=lesson[i].credit();
        fout.write((char*)(&credit),sizeof(int));
        teaid=lesson[i].teacherID();
        fout.write((char*)(&teaid),sizeof(long));
        stucount=lesson[i].stuscore.count();
        fout.write((char*)(&stucount),sizeof(int));
        for(int j=0;j<stucount;j++)
        {
            stuid=lesson[i].stuscore[j].studentID;
            score_=lesson[i].stuscore[j]._score;
            fout.write((char*)(&stuid),sizeof(long));
            fout.write((char*)(&score_),sizeof(int));

        }
        fout.close();
    }
}

void Manager_MainWindow::save_file()
{
    save_student_file();
    save_teacher_file();
    save_lesson_file();
}

void Manager_MainWindow::addrow_table_studentlist(QString name, long id, bool sex,bool editable)
{
    int count=ui->table->rowCount();
    ui->table->insertRow(count);
    QLineEdit* lineedit;
    lineedit=new QLineEdit(this);
    lineedit->setText(name);
    ui->table->setCellWidget(count,0,lineedit);
    lineedit->setReadOnly(!editable);
    lineedit=lineedit_studentID();
    lineedit->setText(long_to_qstr(id));
    ui->table->setCellWidget(count,1,lineedit);
    lineedit->setReadOnly(!editable);
    QComboBox *combobox=combobox_sex();
    if(sex==man)
        combobox->setCurrentIndex(0);
    else
        combobox->setCurrentIndex(1);
    ui->table->setCellWidget(count,2,combobox);
    combobox->setEnabled(editable);
}

void Manager_MainWindow::addrow_table_teacherlist(QString name, long id, bool sex, bool editable)
{
    int count=ui->table->rowCount();
    ui->table->insertRow(count);
    QLineEdit* lineedit;
    lineedit=new QLineEdit(this);
    lineedit->setText(name);
    ui->table->setCellWidget(count,0,lineedit);
    lineedit->setReadOnly(!editable);
    lineedit=lineedit_teacherID();
    lineedit->setText(long_to_qstr(id));
    ui->table->setCellWidget(count,1,lineedit);
    lineedit->setReadOnly(!editable);
    QComboBox *combobox=combobox_sex();
    if(sex==man)
        combobox->setCurrentIndex(0);
    else
        combobox->setCurrentIndex(1);
    ui->table->setCellWidget(count,2,combobox);
    combobox->setEnabled(editable);
}

void Manager_MainWindow::addrow_table_lessonlist(QString lessonname, long id, QString teachername,bool editable)
{
    int count=ui->table->rowCount();
    ui->table->insertRow(count);
    QLineEdit* lineedit;
    lineedit=new QLineEdit(this);
    lineedit->setText(lessonname);
    ui->table->setCellWidget(count,0,lineedit);
    lineedit->setReadOnly(!editable);
    lineedit=lineedit_studentID();
    lineedit->setText(long_to_qstr(id));
    ui->table->setCellWidget(count,1,lineedit);
    lineedit->setReadOnly(!editable);
    lineedit=new QLineEdit(this);
    lineedit->setText(teachername);
    ui->table->setCellWidget(count,2,lineedit);
    lineedit->setReadOnly(!editable);
}

void Manager_MainWindow::addrow_table_totalstu(QString name, long id, bool sex, double avescore, double avegpa)
{
    ui->table_student_total->insertRow(0);
    QLineEdit* lineedit;
    lineedit=new QLineEdit(this);
    lineedit->setText(name);
    ui->table_student_total->setCellWidget(0,0,lineedit);
    lineedit->setReadOnly(true);
    lineedit=lineedit_studentID();
    lineedit->setText(long_to_qstr(id));
    ui->table_student_total->setCellWidget(0,1,lineedit);
    lineedit->setReadOnly(true);
    QComboBox *combobox=combobox_sex();
    if(sex==man)
        combobox->setCurrentIndex(0);
    else
        combobox->setCurrentIndex(1);
    ui->table_student_total->setCellWidget(0,2,combobox);
    combobox->setEnabled(false);
    lineedit=new QLineEdit(this);
    if(avescore!=-1)
        lineedit->setText(avescore_to_qstr(avescore));
    ui->table_student_total->setCellWidget(0,3,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit(this);
    if(avegpa!=-1)
        lineedit->setText(avegpa_to_qstr(avegpa));
    ui->table_student_total->setCellWidget(0,4,lineedit);
    lineedit->setReadOnly(true);
}

void Manager_MainWindow::addrow_table_student(QString lessonname, long id, QString teachername, int credit, int score,bool editable)
{
    int count=ui->table_student->rowCount();
    ui->table_student->insertRow(count);
    QLineEdit* lineedit;
    lineedit=new QLineEdit(this);
    lineedit->setText(lessonname);
    ui->table_student->setCellWidget(count,0,lineedit);
    lineedit->setReadOnly(!editable);
    lineedit=lineedit_lessonID();
    lineedit->setText(long_to_qstr(id));
    ui->table_student->setCellWidget(count,1,lineedit);
    lineedit->setReadOnly(!editable);
    lineedit=new QLineEdit(this);
    lineedit->setText(teachername);
    ui->table_student->setCellWidget(count,2,lineedit);
    lineedit->setReadOnly(!editable);
    lineedit=lineedit_credit();
    lineedit->setText(int_to_qstr(credit));
    ui->table_student->setCellWidget(count,3,lineedit);
    lineedit->setReadOnly(!editable);
    lineedit=lineedit_score();
    lineedit->setText(int_to_qstr(score));
    ui->table_student->setCellWidget(count,4,lineedit);
    lineedit->setReadOnly(!editable);
    lineedit=new QLineEdit(this);
    lineedit->setText(score_to_str_gpa(score));
    ui->table_student->setCellWidget(count,5,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit(this);
    lineedit->setText(score_to_level(score));
    ui->table_student->setCellWidget(count,6,lineedit);
    lineedit->setReadOnly(true);
}

void Manager_MainWindow::addrow_table_totaltea(QString teachername, long id, bool sex, int lessoncount)
{
    ui->table_teacher_total->insertRow(0);
    QLineEdit* lineedit;
    lineedit=new QLineEdit(this);
    lineedit->setText(teachername);
    ui->table_teacher_total->setCellWidget(0,0,lineedit);
    lineedit->setReadOnly(true);
    lineedit=lineedit_teacherID();
    lineedit->setText(long_to_qstr(id));
    ui->table_teacher_total->setCellWidget(0,1,lineedit);
    lineedit->setReadOnly(true);
    QComboBox *combobox=combobox_sex();
    if(sex==man)
        combobox->setCurrentIndex(0);
    else
        combobox->setCurrentIndex(1);
    ui->table_teacher_total->setCellWidget(0,2,combobox);
    combobox->setEnabled(false);
    lineedit=new QLineEdit(this);
    lineedit->setText(int_to_qstr(lessoncount));
    ui->table_teacher_total->setCellWidget(0,3,lineedit);
    lineedit->setReadOnly(true);
}

void Manager_MainWindow::addrow_table_teacher(QString lessonname, long id, int credit, int stunumber,bool editable)
{
    int count=ui->table_teacher->rowCount();
    ui->table_teacher->insertRow(count);
    QLineEdit* lineedit;
    lineedit=new QLineEdit(this);
    lineedit->setText(lessonname);
    ui->table_teacher->setCellWidget(count,0,lineedit);
    lineedit->setReadOnly(!editable);
    lineedit=lineedit_lessonID();
    lineedit->setText(long_to_qstr(id));
    ui->table_teacher->setCellWidget(count,1,lineedit);
    lineedit->setReadOnly(!editable);
    lineedit=lineedit_credit();
    lineedit->setText(int_to_qstr(credit));
    ui->table_teacher->setCellWidget(count,2,lineedit);
    lineedit->setReadOnly(!editable);
    lineedit=new QLineEdit(this);
    lineedit->setText(int_to_qstr(stunumber));
    ui->table_teacher->setCellWidget(count,3,lineedit);
    lineedit->setReadOnly(!editable);
}

void Manager_MainWindow::addrow_table_totalles(QString lessonname, long id, QString teachername, long teacherid, int credit, int stunumber, double avescore, double avegpa,double midscore,double midgpa,double passpercentage)
{
    ui->table_lesson_total->insertColumn(0);
    QLineEdit* lineedit;
    lineedit=new QLineEdit(this);
    lineedit->setText(lessonname);
    ui->table_lesson_total->setCellWidget(0,0,lineedit);
    lineedit->setReadOnly(true);
    lineedit=lineedit_lessonID();
    lineedit->setText(long_to_qstr(id));
    ui->table_lesson_total->setCellWidget(1,0,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit(this);
    lineedit->setText(teachername);
    ui->table_lesson_total->setCellWidget(2,0,lineedit);
    lineedit->setReadOnly(true);
    lineedit=lineedit_teacherID();
    lineedit->setText(long_to_qstr(teacherid));
    ui->table_lesson_total->setCellWidget(3,0,lineedit);
    lineedit->setReadOnly(true);
    lineedit=lineedit_credit();
    lineedit->setText(int_to_qstr(credit));
    ui->table_lesson_total->setCellWidget(4,0,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit(this);
    lineedit->setText(int_to_qstr(stunumber));
    ui->table_lesson_total->setCellWidget(5,0,lineedit);
    lineedit->setReadOnly(true);
    if(avescore==-5)
        return ;
    if(avescore==-3)
        return ;
    lineedit=new QLineEdit(this);qDebug()<<passpercentage;
    lineedit->setText(percentage_to_qstr(passpercentage));qDebug()<<percentage_to_qstr(passpercentage);
    ui->table_lesson_total->setCellWidget(10,0,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit(this);
    lineedit->setText(percentage_to_qstr(1-passpercentage));
    ui->table_lesson_total->setCellWidget(11,0,lineedit);
    lineedit->setReadOnly(true);
    if(avescore==-1)
        return ;
    lineedit=new QLineEdit(this);
    lineedit->setText(avescore_to_qstr(avescore));
    ui->table_lesson_total->setCellWidget(6,0,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit(this);
    lineedit->setText(avegpa_to_qstr(avegpa));
    ui->table_lesson_total->setCellWidget(7,0,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit(this);
    lineedit->setText(avescore_to_qstr(midscore));
    ui->table_lesson_total->setCellWidget(8,0,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit(this);
    lineedit->setText(avegpa_to_qstr(midgpa));
    ui->table_lesson_total->setCellWidget(9,0,lineedit);
    lineedit->setReadOnly(true);
}

void Manager_MainWindow::addrow_table_lesson(QString stuname, long stuid, int score, bool editable)
{
    int count=ui->table_lesson->rowCount();
    ui->table_lesson->insertRow(count);
    QLineEdit* lineedit;
    lineedit=new QLineEdit(this);
    lineedit->setText(stuname);
    ui->table_lesson->setCellWidget(count,0,lineedit);
    lineedit->setReadOnly(!editable);
    lineedit=lineedit_studentID();
    lineedit->setText(long_to_qstr(stuid));
    ui->table_lesson->setCellWidget(count,1,lineedit);
    lineedit->setReadOnly(!editable);
    lineedit=lineedit_score();
    lineedit->setText(int_to_qstr(score));
    ui->table_lesson->setCellWidget(count,2,lineedit);
    lineedit->setReadOnly(!editable);
    lineedit=new QLineEdit(this);
    lineedit->setText(score_to_str_gpa(score));
    ui->table_lesson->setCellWidget(count,3,lineedit);
    lineedit->setReadOnly(!editable);
    lineedit=new QLineEdit(this);
    lineedit->setText(score_to_level(score));
    ui->table_lesson->setCellWidget(count,4,lineedit);
    lineedit->setReadOnly(!editable);
}

void Manager_MainWindow::newrow_table_student()
{
    int count=ui->table_student->rowCount();
    ui->table_student->insertRow(count);
    QLineEdit* lineedit;
    lineedit=new QLineEdit(this);
    ui->table_student->setCellWidget(count,0,lineedit);
    lineedit->setReadOnly(true);
    lineedit=lineedit_lessonID();
    ui->table_student->setCellWidget(count,1,lineedit);
    lineedit->setReadOnly(false);
    lineedit=new QLineEdit(this);
    ui->table_student->setCellWidget(count,2,lineedit);
    lineedit->setReadOnly(true);
    lineedit=lineedit_credit();
    ui->table_student->setCellWidget(count,3,lineedit);
    lineedit->setReadOnly(true);
    lineedit=lineedit_score();
    ui->table_student->setCellWidget(count,4,lineedit);
    lineedit->setReadOnly(false);
    lineedit=new QLineEdit(this);
    ui->table_student->setCellWidget(count,5,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit(this);
    ui->table_student->setCellWidget(count,6,lineedit);
    lineedit->setReadOnly(true);
}

bool Manager_MainWindow::saverow_table_student()
{
    if(check_student())
    {
        QLineEdit* lineedit;
        int count=ui->table_student->rowCount();
        count--;
        lineedit=(QLineEdit*)ui->table_student->cellWidget(count,1);
        Lesson &nowles=lesson(qstr_to_int(lineedit->text()));
        student_object->lessonID.add(nowles.ID());
        lineedit->setReadOnly(true);
        lineedit=(QLineEdit*)ui->table_student->cellWidget(count,0);
        lineedit->setText(nowles.name());
        lineedit=(QLineEdit*)ui->table_student->cellWidget(count,2);
        lineedit->setText(teacher(nowles.teacherID()).name());
        lineedit=(QLineEdit*)ui->table_student->cellWidget(count,3);
        lineedit->setText(int_to_qstr(nowles.credit()));
        lineedit=(QLineEdit*)ui->table_student->cellWidget(count,4);
        lineedit->setReadOnly(true);
        int score_=qstr_to_int(lineedit->text());
        nowles.stuscore.add(score(student_object->ID(),score_));
        lineedit=(QLineEdit*)ui->table_student->cellWidget(count,5);
        lineedit->setText(score_to_str_gpa(score_));
        lineedit=(QLineEdit*)ui->table_student->cellWidget(count,6);
        lineedit->setText(score_to_level(score_));
        return true;
    }
    return false;
}

void Manager_MainWindow::newrow_table_lesson()
{
    int count=ui->table_lesson->rowCount();
    QLineEdit* lineedit;
    ui->table_lesson->insertRow(count);
    lineedit=new QLineEdit(this);
    lineedit->setReadOnly(true);
    ui->table_lesson->setCellWidget(count,0,lineedit);
    lineedit=lineedit_studentID();
    lineedit->setReadOnly(false);
    ui->table_lesson->setCellWidget(count,1,lineedit);
    lineedit=lineedit_score();
    lineedit->setReadOnly(false);
    ui->table_lesson->setCellWidget(count,2,lineedit);
    lineedit=new QLineEdit(this);
    lineedit->setReadOnly(true);
    ui->table_lesson->setCellWidget(count,3,lineedit);
    lineedit=new QLineEdit(this);
    lineedit->setReadOnly(true);
    ui->table_lesson->setCellWidget(count,4,lineedit);
}

bool Manager_MainWindow::saverow_table_lesson()
{
    if(check_lesson())
    {
        int count=ui->table_lesson->rowCount()-1;
        QLineEdit* lineedit;
        lineedit=(QLineEdit*)ui->table_lesson->cellWidget(count,1);
        Student &nowstu=student(qstr_to_long(lineedit->text()));
        nowstu.lessonID.add(lesson_object->ID());
        lineedit->setReadOnly(true);
        lineedit=(QLineEdit*)ui->table_lesson->cellWidget(count,0);
        lineedit->setText(nowstu.name());
        lineedit=(QLineEdit*)ui->table_lesson->cellWidget(count,2);
        int score_=qstr_to_int(lineedit->text());
        lesson_object->stuscore.add(score(nowstu.ID(),score_));
        lineedit->setReadOnly(true);
        lineedit=(QLineEdit*)ui->table_lesson->cellWidget(count,3);
        lineedit->setText(score_to_str_gpa(score_));
        lineedit=(QLineEdit*)ui->table_lesson->cellWidget(count,4);
        lineedit->setText(score_to_level(score_));
        return true;
    }
    return false;
}

void Manager_MainWindow::open_studentlist()
{
    ui->table->horizontalHeaderItem(0)->setText("姓名");
    ui->table->horizontalHeaderItem(1)->setText("学号");
    ui->table->horizontalHeaderItem(2)->setText("性别");
    for(int i=0;i<student.count();i++)
        addrow_table_studentlist(student[i].name(),student[i].ID(),student[i].sex(),false);
    table_resize();
}

void Manager_MainWindow::open_teacherlist()
{
    ui->table->horizontalHeaderItem(0)->setText("姓名");
    ui->table->horizontalHeaderItem(1)->setText("教职工号");
    ui->table->horizontalHeaderItem(2)->setText("性别");
    for(int i=0;i<teacher.count();i++)
        addrow_table_teacherlist(teacher[i].name(),teacher[i].ID(),teacher[i].sex(),false);
    table_resize();
}

void Manager_MainWindow::open_lessonlist()
{
    ui->table->horizontalHeaderItem(0)->setText("课程名");
    ui->table->horizontalHeaderItem(1)->setText("课程号");
    ui->table->horizontalHeaderItem(2)->setText("教师");
    for(int i=0;i<lesson.count();i++)
        addrow_table_lessonlist(lesson[i].name(),lesson[i].ID(),teacher(lesson[i].teacherID()).name(),false);
    table_resize();
}

void Manager_MainWindow::open_student()
{
    double nowscore,nowcredit,nowgpa;
    double totalscore,totalcredit,totalgpa;
    totalcredit=totalgpa=totalscore=0;
    Lesson now;
    for(int i=0;i<student_object->lessonID.count();i++)
    {
        now=lesson(student_object->lessonID[i]);
        for(int j=0;j<now.stuscore.count();j++)
        {
            if(student_object->ID()==now.stuscore[j].studentID)
            {
                nowscore=now.stuscore[j]._score;
                break;
            }
        }
        nowcredit=now.credit();
        addrow_table_student(now.name(),now.ID(),teacher(now.teacherID()).name(),nowcredit,nowscore,false);
        nowgpa=score_to_gpa(nowscore);
        if(nowscore>=0)
        {
            totalcredit+=nowcredit;
            totalscore+=nowcredit*nowscore;
            totalgpa+=nowcredit*nowgpa;
        }
    }
    if(student_object->lessonID.count()==0)
    {
        addrow_table_totalstu(student_object->name(),student_object->ID(),student_object->sex(),-1,-1);
        return ;
    }
    addrow_table_totalstu(student_object->name(),student_object->ID(),student_object->sex(),totalscore/totalcredit,totalgpa/totalcredit);
}

void Manager_MainWindow::open_teacher()
{
    addrow_table_totaltea(teacher_object->name(),teacher_object->ID(),teacher_object->sex(),teacher_object->lessonID.count());
    Lesson now;
    for(int i=0;i<teacher_object->lessonID.count();i++)
    {
        now=lesson(teacher_object->lessonID[i]);
        addrow_table_teacher(now.name(),now.ID(),now.credit(),now.stuscore.count(),false);
    }
}

void Manager_MainWindow::open_lesson()
{
    double nowscore,nowgpa;
    double totalscore,totalgpa;
    totalgpa=totalscore=0;
    int count=lesson_object->stuscore.count();
    int *stuscore=new int[count];
    Student now;
    if(count==0)
    {
       addrow_table_totalles(lesson_object->name(),lesson_object->ID(),teacher(lesson_object->teacherID()).name(),lesson_object->teacherID(),lesson_object->credit(),lesson_object->stuscore.count(),-5,-5,-5,-5,-5);//-5代表空
       return ;
    }
    nowscore=stuscore[0];
    if(nowscore==-3)
    {
        addrow_table_totalles(lesson_object->name(),lesson_object->ID(),teacher(lesson_object->teacherID()).name(),lesson_object->teacherID(),lesson_object->credit(),lesson_object->stuscore.count(),-3,-3,-3,-3,-3);//-3代表没出成绩
        return ;
    }qDebug()<<count<<lesson_object;
    for(int i=0;i<count;i++)
    {
        now=student(lesson_object->stuscore[i].studentID);
        nowscore=lesson_object->stuscore[i]._score;
        addrow_table_lesson(now.name(),now.ID(),nowscore,false);
        nowgpa=score_to_gpa(nowscore);
        stuscore[i]=nowscore;
        if(nowscore>=0)
        {
           totalgpa+=nowgpa;
           totalscore+=nowscore;
        }
    }
    if(nowscore==-1||nowscore==-2)
    {
        int passcount=0;
        for(int i=0;i<count;i++)
        {
            if(stuscore[i]==-1)
                passcount++;
        }
            addrow_table_totalles(lesson_object->name(),lesson_object->ID(),teacher(lesson_object->teacherID()).name(),lesson_object->teacherID(),lesson_object->credit(),lesson_object->stuscore.count(),-1,-1,-1,-1,double(passcount)/double(count));//-1代表记PF
    }
    double avescore,avegpa;
    avescore=totalscore/count;
    avegpa=totalgpa/count;
    int temp;
    for(int i=0;i<count-1;i++) //从小到大排序
    {
        for(int j=0;j<count-i-1;j++)
        {
            if(stuscore[j]>stuscore[j+1])
            {
                temp=stuscore[j];
                stuscore[j]=stuscore[j+1];
                stuscore[j+1]=temp;
            }
        }
    }
    double midscore,midgpa;
    if(count%2==1)
    {
        midscore=stuscore[count/2];
        midgpa=score_to_gpa(midscore);
    }
    else
    {
        midscore=(stuscore[count/2]+stuscore[count/2-1])/2;
        midgpa=(score_to_gpa(stuscore[count/2])+score_to_gpa(stuscore[count/2-1]))/2;
    }
    int passcount=0;
    for(int i=0;i<count;i++)
    {
        if(stuscore[i]>=60)
            passcount++;
    }
    addrow_table_totalles(lesson_object->name(),lesson_object->ID(),teacher(lesson_object->teacherID()).name(),lesson_object->teacherID(),lesson_object->credit(),lesson_object->stuscore.count(),avescore,avegpa,midscore,midgpa,double(passcount)/double(count));
}

QString Manager_MainWindow::score_to_level(double score)
{
    if(score>=95)
            return "A";
        else if(score>=90&&score<=94)
            return "A-";
        else if(score>=85&&score<=89)
            return "B+";
        else if(score>=80&&score<=84)
            return "B";
        else if(score>=77&&score<=79)
            return "B-";
        else if(score>=73&&score<=76)
            return "C+";
        else if(score>=70&&score<=72)
            return "C";
        else if(score>=67&&score<=69)
            return "C-";
        else if(score>=63&&score<=66)
            return "D+";
        else if(score>=60&&score<=62)
            return "D";
        else if(score>=0&&score<60)
            return "F";
        else if(score==-1)  //记PF
            return "P";
        else if(score==-2)  //记PF
            return "F";
        else if(score==-3)
            return "*****";
}

double Manager_MainWindow::score_to_gpa(double score)
{
    if(score>=95)
            return 4;
        else if(score>=90&&score<=94)
            return 3.7;
        else if(score>=85&&score<=89)
            return 3.3;
        else if(score>=80&&score<=84)
            return 3.0;
        else if(score>=77&&score<=79)
            return 2.7;
        else if(score>=73&&score<=76)
            return 2.3;
        else if(score>=70&&score<=72)
            return 2.0;
        else if(score>=67&&score<=69)
            return 1.7;
        else if(score>=63&&score<=66)
            return 1.3;
        else if(score>=60&&score<=62)
            return 1.0;
        else if(score>=0&&score<=59)
            return 0;
        else
            return score;
}

QString Manager_MainWindow::score_to_str_gpa(double score)
{
    if(score==-1)
        return "N/A";
    else if(score==-2)
        return "F";
    else if(score==-3)
        return "*****";
    ostringstream os;
    double sco=score_to_gpa(score);
    os<<sco;
    if(sco==4||sco==3||sco==2||sco==1)
    os<<".0";
    return QString::fromStdString(os.str());

}

QString Manager_MainWindow::avescore_to_qstr(double avescore)
{
    double temp=avescore*100;
    int temp2=temp;
    if(temp-temp2>=0.5)
        temp2++;
    double now=double(temp2)/100;
    ostringstream os;
    os<<now;
    return QString::fromStdString(os.str());
}

QString Manager_MainWindow::avegpa_to_qstr(double avegpa)
{
    double temp=avegpa*1000;
    int temp2=temp;
    if(temp-temp2>=0.5)
        temp2++;
    double now=double(temp2)/1000;
    ostringstream os;
    os<<now;
    return QString::fromStdString(os.str());
}

QLineEdit *Manager_MainWindow::lineedit_studentID()
{
    QLineEdit* lineedit=new QLineEdit(this);
    QRegExp rx("2[0][0-9]{8}");
    QValidator *val=new QRegExpValidator(rx,this);
    lineedit->setValidator(val);
    return lineedit;
}

QLineEdit *Manager_MainWindow::lineedit_teacherID()
{
    QLineEdit* lineedit=new QLineEdit(this);
    QRegExp rx("[1-9][0-9]{5}");
    QValidator *val=new QRegExpValidator(rx,this);
    lineedit->setValidator(val);
    return lineedit;
}

QLineEdit *Manager_MainWindow::lineedit_lessonID()
{
    QLineEdit* lineedit=new QLineEdit(this);
    QRegExp rx("[1-9][0-9]{7}");
    QValidator *val=new QRegExpValidator(rx,this);
    lineedit->setValidator(val);
    return lineedit;
}

QLineEdit *Manager_MainWindow::lineedit_credit()
{
    QLineEdit* lineedit=new QLineEdit(this);
    QRegExp rx("^[1-9]{1}$|^10$");
    QValidator *val=new QRegExpValidator(rx,this);
    lineedit->setValidator(val);
    return lineedit;
}

QLineEdit *Manager_MainWindow::lineedit_score()
{
    QLineEdit* lineedit=new QLineEdit(this);
    QRegExp rx("^[0-9]{1,2}$|^100$|^-1$|^-2$|^-3$");
    QValidator *val=new QRegExpValidator(rx,this);
    lineedit->setValidator(val);
    return lineedit;
}

QComboBox *Manager_MainWindow::combobox_sex()
{
    QComboBox* com=new QComboBox(this);
    com->addItem("男");
    com->addItem("女");
    com->setEditable(false);
    return com;
}

QString Manager_MainWindow::long_to_qstr(long n)
{
    ostringstream os;
    os<<n;
    return QString::fromStdString(os.str());
}

QString Manager_MainWindow::int_to_qstr(int n)
{
    ostringstream os;
    os<<n;
    return QString::fromStdString(os.str());
}

QString Manager_MainWindow::percentage_to_qstr(double per)
{
    double temp=per*10000;
    int temp2=temp;
    if(temp-temp2>=0.5)
        temp2++;
    double re=double(temp2)/100;
    ostringstream os;
    os<<re<<"%";
    return QString::fromStdString(os.str());
}

long Manager_MainWindow::qstr_to_long(QString qs)
{
    istringstream is(qs.toStdString());
    long id;
    is>>id;
    return id;
}

int Manager_MainWindow::qstr_to_int(QString qs)
{
    istringstream is(qs.toStdString());
    int id;
    is>>id;
    return id;
}

bool Manager_MainWindow::check_student_list()
{
    QLineEdit* lineedit;
    int count=ui->table->rowCount();
    count--;
    lineedit=(QLineEdit*)ui->table->cellWidget(count,0);
    if(lineedit->text()==QString())
    {
        critical_nopersonname(count+1);
        return false;
    }
    lineedit=(QLineEdit*)ui->table->cellWidget(count,1);
    long id=-1;
    id=qstr_to_long(lineedit->text());
    if(id==-1)
    {
        critical_nostuid(count+1);
        return false;
    }
    if(id<2000000000)
    {
        critical_wrongstuid(count+1);
        return false;
    }
    for(int i=0;i<student.count();i++)
    {
        if(student[i].ID()==id)
        {
            critical_repeatstu(count+1);
            return false;
        }
    }
    return true;
}

bool Manager_MainWindow::check_teacher_list()
{
    QLineEdit* lineedit;
    int count=ui->table->rowCount();
    count--;
    lineedit=(QLineEdit*)ui->table->cellWidget(count,0);
    if(lineedit->text()==QString())
    {
        critical_nopersonname(count+1);
        return false;
    }
    lineedit=(QLineEdit*)ui->table->cellWidget(count,1);
    long id=-1;
    id=qstr_to_long(lineedit->text());qDebug()<<id;
    if(id==-1)
    {
        critical_noteaid(count+1);
        return false;
    }
    if(id<100000)
    {
        critical_wrongteaid(count+1);
        return false;
    }
    for(int i=0;i<teacher.count();i++)
    {
        if(teacher[i].ID()==id)
        {
            critical_repeattea(count+1);
            return false;
        }
    }
    return true;
}

bool Manager_MainWindow::check_lesson_list()
{
    QLineEdit* lineedit;
    lineedit=(QLineEdit*)ui->table_newlesson->cellWidget(0,0);
    long useid;
    if(lineedit->text()==QString())
    {
        critical_nolesname(0);
        return false;
    }
    lineedit=(QLineEdit*)ui->table_newlesson->cellWidget(0,1);
    long id=-1;
    id=qstr_to_long(lineedit->text());
    if(id==-1)
    {
        critical_nolesid(0);
        return false;
    }
    if(id<10000000)
    {
        critical_wronglesid(0);
        return false;
    }
    useid=id;
    id=-1;
    lineedit=(QLineEdit*)ui->table_newlesson->cellWidget(0,2);
    id=qstr_to_long(lineedit->text());
    if(id==-1)
    {
        critical_noteaid(0);
        return false;
    }
    if(id<100000)
    {
        critical_wrongteaid(0);
        return false;
    }
    int credit=-1;
    lineedit=(QLineEdit*)ui->table_newlesson->cellWidget(0,3);
    credit=qstr_to_int(lineedit->text());
    if(credit<=0)
    {
        critical_nocredit(0);
        return false;
    }
    for(int i=0;i<lesson.count();i++)
    {
        if(lesson[i].ID()==useid)
        {
            critical_repeatles(0);
            return false;
        }
    }
    bool flag=true;
    for(int i=0;i<teacher.count();i++)
    {
        if(teacher[i].ID()==id)
        {
            flag=false;
            break;
        }
    }
    if(flag)
    {
        critical_notea(0);
        return false;
    }
    return true;
}

bool Manager_MainWindow::check_student()
{
    int count=ui->table_student->rowCount();
    count--;
    QLineEdit* lineedit;
    lineedit=(QLineEdit*)ui->table_student->cellWidget(count,1);
    long id=-1;
    id=qstr_to_long(lineedit->text());
    if(id<10000000)
    {
        critical_wronglesid(count+1);
        return false;
    }
    int score_=-10;
    lineedit=(QLineEdit*)ui->table_student->cellWidget(count,4);
    if(lineedit->text()==QString())
    {
        critical_noscore(count+1);
        return false;
    }
    score_=qstr_to_int(lineedit->text());
    bool flag=true;
    for(int i=0;i<lesson.count();i++)
    {
        if(lesson[i].ID()==id)
        {
            flag=false;
            break;
        }
    }
    if(flag)
    {
        critical_noles(count+1);
        return false;
    }
    flag=false;
    for(int i=0;i<student_object->lessonID.count();i++)
    {
        if(id==student_object->lessonID[i])
        {
            flag=true;
            break;
        }
    }
    if(flag)
    {
        critical_stu_repeatles(count+1);
        return false;
    }
    return true;
}

bool Manager_MainWindow::check_lesson()
{
    int count=ui->table_lesson->rowCount()-1;
    QLineEdit* lineedit;
    lineedit=(QLineEdit*)(ui->table_lesson->cellWidget(count,1));
    long id=qstr_to_long(lineedit->text());
    if(id<2000000000)
    {
        critical_wrongstuid(count+1);
        return false;
    }
    lineedit=(QLineEdit*)(ui->table_lesson->cellWidget(count,2));
    if(lineedit->text()==QString())
    {
        critical_noscore(count+1);
        return false;
    }
    bool flag=true;qDebug()<<id;
    for(int i=0;i<student.count();i++)
    {
        if(student[i].ID()==id)
        {
            flag=false;
            break;
        }
    }
    if(flag)
    {
        critical_nostu(count+1);
        return false;
    }
    for(int i=0;i<lesson_object->stuscore.count();i++)
    {
        if(lesson_object->stuscore[i].studentID==id)
        {
            critical_repeatstu(count+1);
            return false;
        }
    }
    return true;
}

bool Manager_MainWindow::check_teacher_all()
{
    QLineEdit*lineedit=(QLineEdit*)ui->table_teacher_total->cellWidget(0,0);
    if(lineedit->text()==QString())
    {
        critical_nopersonname(1);
        return false;
    }
    return true;
}

bool Manager_MainWindow::check_student_all()
{
    QLineEdit*lineedit;
    lineedit=(QLineEdit*)ui->table_student_total->cellWidget(0,0);
    if(lineedit->text()==QString())
    {
        critical_nopersonname(1);
        return false;
    }
    for(int i=0;i<ui->table_student->rowCount();i++)
    {
        lineedit=(QLineEdit*)ui->table_student->cellWidget(i,4);
        if(lineedit->text()==QString())
        {
            critical_noscore(i+1);
            return false;
        }
    }
    return true;
}

bool Manager_MainWindow::check_lesson_all()
{
    QLineEdit*lineedit;
    lineedit=(QLineEdit*)ui->table_lesson_total->cellWidget(0,0);
    if(lineedit->text()==QString())
    {
        critical_nopersonname(1);
        return false;
    }
    lineedit=(QLineEdit*)ui->table_lesson_total->cellWidget(3,0);
    if(lineedit->text()==QString())
    {
        critical_noteaid(3);
        return false;
    }
    long id=qstr_to_long(lineedit->text());
    if(id<100000)
    {
        critical_wrongteaid(3);
        return false;
    }
    bool flag=true;
    for(int i=0;i<teacher.count();i++)
    {
        if(teacher[i].ID()==id)
        {
            flag=false;
            break;
        }
    }
    if(flag)
    {
        critical_notea(3);
        return false;
    }
    lineedit=(QLineEdit*)ui->table_lesson_total->cellWidget(4,0);
    if(lineedit->text()==QString())
    {
        critical_nocredit(4);
        return false;
    }
    for(int i=0;i<ui->table_lesson->rowCount();i++)
    {
        lineedit=(QLineEdit*)ui->table_lesson->cellWidget(i,2);
        if(lineedit->text()==QString())
        {
            critical_noscore(i+1);
            return false;
        }
    }
    return true;
}

void Manager_MainWindow::critical_nopersonname(int row)
{
    ostringstream os;
    os<<"第"<<row<<"行姓名为空，请输入姓名！";
    QMessageBox::critical(this,"姓名为空！",QString::fromStdString(os.str()),QMessageBox::Yes);
}

void Manager_MainWindow::critical_nolesname(int row)
{
    ostringstream os;
    os<<"第"<<row<<"行课程名为空，请输入课程名！";
    QMessageBox::critical(this,"课程名为空！",QString::fromStdString(os.str()),QMessageBox::Yes);
}

void Manager_MainWindow::critical_nostuid(int row)
{
    ostringstream os;
    os<<"第"<<row<<"行学号为空，请输入学号！";
    QMessageBox::critical(this,"学号为空！",QString::fromStdString(os.str()),QMessageBox::Yes);
}

void Manager_MainWindow::critical_noteaid(int row)
{
    ostringstream os;
    os<<"第"<<row<<"行教职工号为空，请输入教职工号！";
    QMessageBox::critical(this,"教职工号为空！",QString::fromStdString(os.str()),QMessageBox::Yes);
}

void Manager_MainWindow::critical_nolesid(int row)
{
    ostringstream os;
    os<<"第"<<row<<"行课程号为空，请输入课程号！";
    QMessageBox::critical(this,"课程号为空！",QString::fromStdString(os.str()),QMessageBox::Yes);
}

void Manager_MainWindow::critical_nocredit(int row)
{
    ostringstream os;
    os<<"第"<<row<<"行学分为空，请输入学分！";
    QMessageBox::critical(this,"学分为空！",QString::fromStdString(os.str()),QMessageBox::Yes);
}

void Manager_MainWindow::critical_wrongstuid(int row)
{
    ostringstream os;
    os<<"第"<<row<<"行学号范围错误，学号范围介于[2000000000,2100000000)！";
    QMessageBox::critical(this,"学号范围错误！",QString::fromStdString(os.str()),QMessageBox::Yes);
}

void Manager_MainWindow::critical_wrongteaid(int row)
{
    ostringstream os;
    os<<"第"<<row<<"行教职工号范围错误，教职工号范围介于[100000,1000000)！";
    QMessageBox::critical(this,"教职工号范围错误！",QString::fromStdString(os.str()),QMessageBox::Yes);
}

void Manager_MainWindow::critical_wronglesid(int row)
{
    ostringstream os;
    os<<"第"<<row<<"行课程号范围错误，课程号范围介于[10000000,100000000)！";
    QMessageBox::critical(this,"课程号范围错误！",QString::fromStdString(os.str()),QMessageBox::Yes);
}

void Manager_MainWindow::critical_wrongcredit(int row)
{
    ostringstream os;
    os<<"第"<<row<<"行学分范围错误，学分范围介于(0,10]！";
    QMessageBox::critical(this,"学分范围错误！",QString::fromStdString(os.str()),QMessageBox::Yes);
}

void Manager_MainWindow::critical_repeatles(int row)
{
    ostringstream os;
    os<<"第"<<row<<"行课程号已经存在，请勿添加重复课程！";
    QMessageBox::critical(this,"课程重复！",QString::fromStdString(os.str()),QMessageBox::Yes);
}

void Manager_MainWindow::critical_repeatstu(int row)
{
    ostringstream os;
    os<<"第"<<row<<"行学号号已经存在，请勿添加重复学生！";
    QMessageBox::critical(this,"学生重复！",QString::fromStdString(os.str()),QMessageBox::Yes);
}

void Manager_MainWindow::critical_repeattea(int row)
{
    ostringstream os;
    os<<"第"<<row<<"行教职工号已经存在，请勿添加重复教师！";
    QMessageBox::critical(this,"教师重复！",QString::fromStdString(os.str()),QMessageBox::Yes);
}

void Manager_MainWindow::critical_notea(int row)
{
    ostringstream os;
    os<<"第"<<row<<"行教职工号不存在，请核实后再添加！";
    QMessageBox::critical(this,"教师不存在！",QString::fromStdString(os.str()),QMessageBox::Yes);
}

void Manager_MainWindow::critical_nostu(int row)
{
    ostringstream os;
    os<<"第"<<row<<"行学号不存在，请核实后再添加！";
    QMessageBox::critical(this,"学生不存在！",QString::fromStdString(os.str()),QMessageBox::Yes);
}

void Manager_MainWindow::critical_noscore(int row)
{
    ostringstream os;
    os<<"第"<<row<<"行成绩为空，请填写成绩！";
    QMessageBox::critical(this,"成绩为空！",QString::fromStdString(os.str()),QMessageBox::Yes);
}

void Manager_MainWindow::critical_noles(int row)
{
    ostringstream os;
    os<<"第"<<row<<"行课程号号不存在，请核实后再添加！";
    QMessageBox::critical(this,"课程不存在！",QString::fromStdString(os.str()),QMessageBox::Yes);
}

void Manager_MainWindow::critical_stu_repeatles(int row)
{
    ostringstream os;
    os<<"第"<<row<<"行课程已经存在，请核实后再添加！";
    QMessageBox::critical(this,"课程重复！",QString::fromStdString(os.str()),QMessageBox::Yes);
}

void Manager_MainWindow::new_lesson_list()
{
    ui->table_newlesson->insertRow(0);
    QLineEdit* lineedit;
    lineedit=new QLineEdit(this);
    ui->table_newlesson->setCellWidget(0,0,lineedit);
    lineedit=lineedit_lessonID();
    ui->table_newlesson->setCellWidget(0,1,lineedit);
    lineedit=lineedit_teacherID();
    ui->table_newlesson->setCellWidget(0,2,lineedit);
    lineedit=lineedit_credit();
    ui->table_newlesson->setCellWidget(0,3,lineedit);
}

void Manager_MainWindow::sort_stu_id()
{
    long tempid;
    int tempscore;
    int count=lesson_object->stuscore.count();
    if(sort_modal==up) //从小到大排序
    {
        for(int i=0;i<count-1;i++)
        {
            for(int j=0;j<count-i-1;j++)
            {
                if(cp_stuscore[j].studentID>cp_stuscore[j+1].studentID)
                {
                    tempid=cp_stuscore[j].studentID;
                    cp_stuscore[j].studentID=cp_stuscore[j+1].studentID;
                    cp_stuscore[j+1].studentID=tempid;
                    tempscore=cp_stuscore[j]._score;
                    cp_stuscore[j]._score=cp_stuscore[j+1]._score;
                    cp_stuscore[j+1]._score=tempscore;
                }
            }
        }
    }
    else if(sort_modal==false)  //降序排序
    {
        for(int i=0;i<count-1;i++)
        {
            for(int j=0;j<count-i-1;j++)
            {
                if(cp_stuscore[j].studentID<cp_stuscore[j+1].studentID)
                {
                    tempid=cp_stuscore[j].studentID;
                    cp_stuscore[j].studentID=cp_stuscore[j+1].studentID;
                    cp_stuscore[j+1].studentID=tempid;
                    tempscore=cp_stuscore[j]._score;
                    cp_stuscore[j]._score=cp_stuscore[j+1]._score;
                    cp_stuscore[j+1]._score=tempscore;
                }
            }
        }
    }
}

void Manager_MainWindow::sort_stu_score()
{
    long tempid;
    int tempscore;
    int count=lesson_object->stuscore.count();
    if(sort_modal==up) //从小到大排序
    {
        for(int i=0;i<count-1;i++)
        {
            for(int j=0;j<count-i-1;j++)
            {
                if(cp_stuscore[j]._score>cp_stuscore[j+1]._score)
                {
                    tempid=cp_stuscore[j].studentID;
                    cp_stuscore[j].studentID=cp_stuscore[j+1].studentID;
                    cp_stuscore[j+1].studentID=tempid;
                    tempscore=cp_stuscore[j]._score;
                    cp_stuscore[j]._score=cp_stuscore[j+1]._score;
                    cp_stuscore[j+1]._score=tempscore;
                }
            }
        }
    }
    else if(sort_modal==false)  //降序排序
    {
        for(int i=0;i<count-1;i++)
        {
            for(int j=0;j<count-i-1;j++)
            {
                if(cp_stuscore[j]._score<cp_stuscore[j+1]._score)
                {
                    tempid=cp_stuscore[j].studentID;
                    cp_stuscore[j].studentID=cp_stuscore[j+1].studentID;
                    cp_stuscore[j+1].studentID=tempid;
                    tempscore=cp_stuscore[j]._score;
                    cp_stuscore[j]._score=cp_stuscore[j+1]._score;
                    cp_stuscore[j+1]._score=tempscore;
                }
            }
        }
    }
}

void Manager_MainWindow::sort_les_score()
{
    long tempid;
    int tempscore;
    int count=student_object->lessonID.count();
    if(sort_modal==up) //从小到大排序
    {
        for(int i=0;i<count-1;i++)
        {
            for(int j=0;j<count-i-1;j++)
            {
                if(cp_lesscore[j]._score>cp_lesscore[j+1]._score)
                {
                    tempid=cp_lesscore[j].lessonID;
                    cp_lesscore[j].lessonID=cp_lesscore[j+1].lessonID;
                    cp_lesscore[j+1].lessonID=tempid;
                    tempscore=cp_lesscore[j]._score;
                    cp_lesscore[j]._score=cp_lesscore[j+1]._score;
                    cp_lesscore[j+1]._score=tempscore;
                }
            }
        }
    }
    else if(sort_modal==false)  //降序排序
    {
        for(int i=0;i<count-1;i++)
        {
            for(int j=0;j<count-i-1;j++)
            {
                if(cp_lesscore[j]._score<cp_lesscore[j+1]._score)
                {
                    tempid=cp_lesscore[j].lessonID;
                    cp_lesscore[j].lessonID=cp_lesscore[j+1].lessonID;
                    cp_lesscore[j+1].lessonID=tempid;
                    tempscore=cp_lesscore[j]._score;
                    cp_lesscore[j]._score=cp_lesscore[j+1]._score;
                    cp_lesscore[j+1]._score=tempscore;
                }
            }
        }
    }
}

void Manager_MainWindow::on_action_student_triggered()
{
    bool flag=ui->action_student->isChecked();
    if(ui->action_edit->isChecked()||ui->action_New->isChecked())
    {
        ui->action_student->setChecked(!flag);
        return ;
    }
    close_all();
    set_welcome_visible(!flag);
    set_table_visivle(flag);
    if(!flag)
    {
        now_page=0;
        now_state=state_welcome;
        return ;
    }
    now_page=1;
    now_state=state_student;
    ui->action_teacher->setChecked(false);
    ui->action_lesson->setChecked(false);
    //打开studentlist
    open_studentlist();
}

void Manager_MainWindow::on_action_teacher_triggered()
{
    bool flag=ui->action_teacher->isChecked();
    if(ui->action_edit->isChecked()||ui->action_New->isChecked())
    {
        ui->action_teacher->setChecked(!flag);
        return ;
    }
    close_all();
    set_welcome_visible(!flag);
    set_table_visivle(flag);
    if(!flag)
    {
        now_page=0;
        now_state=state_welcome;
        return ;
    }
    now_page=1;
    now_state=state_teacher;
    ui->action_student->setChecked(false);
    ui->action_lesson->setChecked(false);
    //打开tealist
    open_teacherlist();
}

void Manager_MainWindow::on_action_lesson_triggered()
{
    bool flag=ui->action_lesson->isChecked();
    if(ui->action_edit->isChecked()||ui->action_New->isChecked())
    {
        ui->action_lesson->setChecked(!flag);
        return ;
    }
    close_all();
    set_welcome_visible(!flag);
    set_table_visivle(flag);
    if(!flag)
    {
        now_page=0;
        now_state=state_welcome;
        return ;
    }
    now_page=1;
    now_state=state_lesson;
    ui->action_teacher->setChecked(false);
    ui->action_student->setChecked(false);
    //打开lesslist
    open_lessonlist();
}

void Manager_MainWindow::on_action_edit_triggered()
{
    bool flag=ui->action_edit->isChecked();
    if(now_page!=2||ui->action_New->isChecked()||!ui->action_edit->isChecked())
    {
        ui->action_edit->setChecked(!flag);
        return ;
    }
    if(now_state==state_teacher)
    {
        set_teacher_editable(true);
    }
    else if(now_state==state_student)
    {
        set_student_editable(true);
    }
    else if(now_state==state_lesson)
    {
        set_lesson_editable(true);
    }


}

void Manager_MainWindow::on_action_New_triggered()
{
    bool flag=ui->action_New->isChecked();
    if(now_page==0||ui->action_edit->isChecked())
    {
        ui->action_New->setChecked(!flag);
        return ;
    }
    if(!flag)
    {
        int answer=QMessageBox::question(this,"您是否放弃这次新增？","您是否放弃这次新增？放弃新增后编辑的内容将会丢失",QMessageBox::Yes,QMessageBox::No);
        if(answer!=QMessageBox::Yes)
        {
            ui->action_New->setChecked(true);
            return ;
        }
        if(now_page==1)
        {
            if(now_state==state_student||now_state==state_teacher)
            {
                ui->table->removeRow(ui->table->rowCount()-1);
                table_resize();
                return ;
            }
            if(now_state==state_lesson)
            {
                ui->table_newlesson->removeRow(0);
                ui->table_newlesson->setVisible(false);
                ui->table->setVisible(true);
                return ;
            }
        }
        if(now_page==2)
        {
            if(now_state==state_student)
            {
                ui->table_student->removeRow(ui->table_student->rowCount()-1);
                student_resize();
                return ;
            }
            if(now_state==state_lesson)
            {
                ui->table_lesson->removeRow(ui->table_lesson->rowCount()-1);
                lesson_resize();
                return ;
            }
        }
    }
    if(now_page==1)
    {
        if(now_state==state_student||now_state==state_teacher)
        {
            if(now_state==state_student)
                addrow_table_studentlist(QString(),0,man,true);
            else
                addrow_table_teacherlist(QString(),0,man,true);
            table_resize();
            return ;
        }
        if(now_state==state_lesson)
        {
           ui->table_newlesson->setVisible(true);
           ui->table->setVisible(false);
           new_lesson_list();
           return ;
        }
    }
    if(now_page==2)
    {
        if(now_state==state_student)
        {
            newrow_table_student();
            student_resize();
            return ;
        }
        if(now_state==state_lesson)
        {
            newrow_table_lesson();
            lesson_resize();
            return ;
        }
        if(now_state==state_teacher)
        {
            QMessageBox::warning(this,"禁止在教师栏中新增课程","禁止在教师栏中新增课程，清前往课程栏中将相应的课程的任课教师改为当前教师",QMessageBox::Yes);
            ui->action_New->setChecked(false);
            return ;
        }
    }

}

void Manager_MainWindow::table_resize()
{
    int w=ui->table->size().width();
    int h=ui->table->rowCount()*row_height+top_height;
    if(h>352)
    h=352;
    ui->table->resize(w,h);
}

void Manager_MainWindow::student_resize()
{
    int w=ui->table_student->size().width();
    int h=ui->table_student->rowCount()*row_height+top_height;
    if(h>352)
    h=352;
    ui->table_student->resize(w,h);
}

void Manager_MainWindow::teacher_resize()
{
    int w=ui->table_teacher->size().width();
    int h=ui->table_teacher->rowCount()*row_height+top_height;
    if(h>352)
    h=352;
    ui->table_teacher->resize(w,h);
}

void Manager_MainWindow::lesson_resize()
{
    int w=ui->table_lesson->size().width();
    int h=ui->table_lesson->rowCount()*row_height+top_height;
    if(h>352)
    h=352;
    ui->table_lesson->resize(w,h);
}

void Manager_MainWindow::close_all()
{
    clear_alltable();
    set_all_visible(false);
}

void Manager_MainWindow::clear_alltable()
{
    clear_table();
    clear_student();
    clear_teacher();
    clear_lesson();
}

void Manager_MainWindow::clear_table()
{
    int count=ui->table->rowCount();
    for(int i=0;i<count;i++)
        ui->table->removeRow(0);
    table_resize();
}

void Manager_MainWindow::clear_student()
{
    int count;
    count=ui->table_student_total->rowCount();
    if(count==1)
        ui->table_student_total->removeRow(0);
    count=ui->table_student->rowCount();
    for(int i=0;i<count;i++)
        ui->table_student->removeRow(0);
}

void Manager_MainWindow::clear_teacher()
{
    int count;
    count=ui->table_teacher_total->rowCount();
    if(count==1)
        ui->table_teacher_total->removeRow(0);
    count=ui->table_teacher->rowCount();
    for(int i=0;i<count;i++)
        ui->table_teacher->removeRow(0);
}

void Manager_MainWindow::clear_lesson()
{
    int count;
    count=ui->table_lesson_total->columnCount();
    if(count==1)
        ui->table_lesson_total->removeColumn(0);
    count=ui->table_lesson->rowCount();
    for(int i=0;i<count;i++)
        ui->table_lesson->removeRow(0);
}

void Manager_MainWindow::set_all_visible(bool arg)
{
    set_welcome_visible(arg);
    set_studenttable_visible(arg);
    set_table_visivle(arg);
    set_teachertable_visible(arg);
    set_lessontable_visible(arg);
    ui->table_newlesson->setVisible(arg);
}

void Manager_MainWindow::on_action_Look_triggered()
{
    if(now_page==0||now_page==2)
        return ;
    int count=ui->table->currentRow();
    if(count==-1)
        return ;
    if(now_state==state_student)
    {
        now_page=2;
        QLineEdit *lineedit=(QLineEdit*)(ui->table->cellWidget(count,1));
        long id=qstr_to_long(lineedit->text());
        for(int i=0;i<student.count();i++)
        {
            if(student[i].ID()==id)
            {
                student_object=&student[i];
                break;
            }
        }
        close_all();
        open_student();
        set_studenttable_visible(true);
        student_resize();
        return ;
    }
    if(now_state==state_teacher)
    {
        now_page=2;
        QLineEdit *lineedit=(QLineEdit*)(ui->table->cellWidget(count,1));
        long id=qstr_to_long(lineedit->text());
        for(int i=0;i<teacher.count();i++)
        {
            if(teacher[i].ID()==id)
            {
                teacher_object=&teacher[i];
                break;
            }
        }
        close_all();
        open_teacher();
        set_teachertable_visible(true);
        teacher_resize();
        return ;
    }
    if(now_state==state_lesson)
    {
        now_page=2;
        QLineEdit *lineedit=(QLineEdit*)(ui->table->cellWidget(count,1));
        long id=qstr_to_long(lineedit->text());
        for(int i=0;i<lesson.count();i++)
        {
            if(lesson[i].ID()==id)
            {
                lesson_object=&lesson[i];
                break;
            }
        }
        close_all();
        open_lesson();
        set_lessontable_visible(true);
        lesson_resize();
        return ;
    }
}

void Manager_MainWindow::on_action_back_triggered()
{
    if(ui->action_edit->isChecked()||ui->action_New->isChecked())
        return ;
    if(now_page==0)
        return ;
    close_all();
    if(now_page==1)
    {
        now_page--;
        set_welcome_visible(true);
        now_state=state_welcome;
        ui->action_student->setChecked(false);
        ui->action_teacher->setChecked(false);
        ui->action_lesson->setChecked(false);
        return ;
    }
    if(now_page==2)
    {
        now_page--;
        if(now_state==state_student)
            open_studentlist();
        else if(now_state==state_teacher)
            open_teacherlist();
        else
            open_lessonlist();
        set_table_visivle(true);
    }
}

void Manager_MainWindow::on_action_deleteobject_triggered()
{
    if(ui->action_edit->isChecked()||ui->action_New->isChecked())
        return ;
    if(now_page==0)
        return ;
    if(now_page==1)
    {
        int count=ui->table->currentRow();
        if(count==-1)
            return ;
        int answer=QMessageBox::question(this,"你是否确定要删除该项？","删除后将无法撤销，您是否要删除该项？",QMessageBox::Yes,QMessageBox::No);
        if(answer!=QMessageBox::Yes)
            return ;
        QLineEdit* lineedit;
        long id;
        lineedit=(QLineEdit*)ui->table->cellWidget(count,1);
        id=qstr_to_long(lineedit->text());
        if(now_state==state_student)
        {
            Student &now=student(id);
            long lessonid;
            for(int i=0;i<now.lessonID.count();i++)
            {
                lessonid=now.lessonID[i];
                for(int j=0;j<lesson(lessonid).stuscore.count();j++)
                {
                    if(lesson(lessonid).stuscore[j].studentID==id)
                    {
                        lesson(lessonid).stuscore.remove_index(j);
                        break;
                    }
                }
            }
            student.remove_ID(id);
            ui->table->removeRow(count);
            table_resize();
        }
        else if(now_state==state_teacher)
        {
            Teacher &now=teacher(id);
            if(now.lessonID.count()>0)
            {
                QMessageBox::critical(this,"无法删除！","该教师仍有任课，请先将该教师的任课清空，才能删除该教师信息！",QMessageBox::Yes);
                return ;
            }
            teacher.remove_ID(id);
            ui->table->removeRow(count);
            table_resize();
        }
        else if(now_state==state_lesson)
        {
            Lesson &now=lesson(id);
            long stuid;
            for(int i=0;i<now.stuscore.count();i++)
            {
                stuid=now.stuscore[i].studentID;
                student(stuid).lessonID.remove_value(id);
            }
            teacher(now.teacherID()).lessonID.remove_value(id);
            lesson.remove_ID(id);
            ui->table->removeRow(count);
            table_resize();
        }
    }
    else if(now_page==2)
    {
        int count;
        QLineEdit* lineedit;
        if(now_state==state_teacher)
        {
            if(ui->table_teacher->currentRow()==-1)
                return ;
            QMessageBox::critical(this,"禁止从教师信息中删除任课信息！","禁止从教师信息中心删除任课信息，应前往课程信息中为该课程更换一名任课教师，更换后即达到删除该条任课信息的效果",QMessageBox::Yes);
            return ;
        }
        else if(now_state==state_student)
        {
            count=ui->table_student->currentRow();
            if(count==-1)
                return ;
            int answer=QMessageBox::question(this,"你是否确定要删除该项？","删除后将无法撤销，您是否要删除该项？",QMessageBox::Yes,QMessageBox::No);
            if(answer!=QMessageBox::Yes)
                return ;
            lineedit=(QLineEdit*)ui->table_student->cellWidget(count,1);
            long lesid=qstr_to_long(lineedit->text());
            Lesson &lesnow=lesson(lesid);
            for(int i=0;i<lesnow.stuscore.count();i++)
            {
                if(lesnow.stuscore[i].studentID==student_object->ID())
                {
                    lesnow.stuscore.remove_index(i);
                    break;
                }
            }
            for(int i=0;i<lesnow.stuscore.count();i++)
                qDebug()<<lesnow.stuscore[i].studentID<<lesnow.stuscore[i]._score;
            student_object->lessonID.remove_value(lesid);
            close_all();
            open_student();
            set_studenttable_visible(true);
            student_resize();
        }
        else if(now_state==state_lesson)
        {
            count=ui->table_lesson->currentRow();
            if(count==-1)
                return ;
            int answer=QMessageBox::question(this,"你是否确定要删除该项？","删除后将无法撤销，您是否要删除该项？",QMessageBox::Yes,QMessageBox::No);
            if(answer!=QMessageBox::Yes)
                return ;
            lineedit=(QLineEdit*)ui->table_lesson->cellWidget(count,1);
            long stuid=qstr_to_long(lineedit->text());
            student(stuid).lessonID.remove_value(lesson_object->ID());
            for(int i=0;i<lesson_object->stuscore.count();i++)
            {
                if(lesson_object->stuscore[i].studentID==stuid)
                {
                    lesson_object->stuscore.remove_index(i);
                    break;
                }
            }
            close_all();
            open_lesson();
            set_lessontable_visible(true);
            lesson_resize();
        }
    }
}

void Manager_MainWindow::on_action_Save_triggered()
{
    if(ui->action_New->isChecked())
    {
        if(now_page==1)
        {
            if((now_state==state_student&&check_student_list())||(now_state==state_teacher&&check_teacher_list()))
            {
                QLineEdit* lineedit;
                int count=ui->table->rowCount();
                count--;
                lineedit=(QLineEdit*)ui->table->cellWidget(count,0);
                lineedit->setReadOnly(true);
                QString name=lineedit->text();
                lineedit=(QLineEdit*)ui->table->cellWidget(count,1);
                lineedit->setReadOnly(true);
                long id=qstr_to_long(lineedit->text());
                bool sex;
                QComboBox* combobox=(QComboBox*)ui->table->cellWidget(count,2);
                if(combobox->currentIndex()==0)
                    sex=man;
                else
                    sex=woman;
                combobox->setEnabled(false);
                if(now_state==state_student)
                    student.add(Student(id,name,sex));
                else
                    teacher.add(Teacher(id,name,sex));
                ui->action_New->setChecked(false);
                return ;
            }
            if(now_state==state_lesson&&check_lesson_list())
            {
                QLineEdit* lineedit;
                lineedit=(QLineEdit*)ui->table_newlesson->cellWidget(0,0);
                QString name=lineedit->text();
                lineedit=(QLineEdit*)ui->table_newlesson->cellWidget(0,1);
                long id=qstr_to_long(lineedit->text());
                lineedit=(QLineEdit*)ui->table_newlesson->cellWidget(0,2);
                long teaid=qstr_to_long(lineedit->text());
                lineedit=(QLineEdit*)ui->table_newlesson->cellWidget(0,3);
                int credit=qstr_to_int(lineedit->text());
                lesson.add(Lesson(id,name,credit,teaid));
                ui->table_newlesson->removeRow(0);
                ui->table_newlesson->setVisible(false);
                set_table_visivle(true);
                int count=lesson.count()-1;
                teacher(lesson[count].teacherID()).lessonID.add(lesson[count].ID());
                addrow_table_lessonlist(lesson[count].name(),lesson[count].ID(),teacher(lesson[count].teacherID()).name(),false);
                table_resize();
                ui->action_New->setChecked(false);
                return ;
            }
        }
        if(now_page==2)
        {
            if(now_state==state_student)
            {
                if(saverow_table_student())
                {
                    clear_student();
                    open_student();
                    ui->action_New->setChecked(false);
                }
                return ;
            }
            if(now_state==state_lesson)
            {
                if(saverow_table_lesson())
                {
                    clear_lesson();
                    open_lesson();
                    ui->action_New->setChecked(false);
                }
            }
        }

    }
    else if(ui->action_edit->isChecked())
    {
        if(now_state==state_student&&check_student_all())
        {
            ui->action_edit->setChecked(false);
            set_student_editable(false);
            update_student();
            close_all();
            open_student();
            set_studenttable_visible(true);

        }
        else if(now_state==state_teacher&&check_teacher_all())
        {
            ui->action_edit->setChecked(false);
            set_teacher_editable(false);
            update_teacher();
            close_all();
            open_teacher();
            set_teachertable_visible(true);
        }
        else if(now_state==state_lesson&&check_lesson_all())
        {
            ui->action_edit->setChecked(false);
            set_lesson_editable(false);
            update_lesson();
            close_all();
            open_lesson();
            set_lessontable_visible(true);
        }
    }
}

void Manager_MainWindow::on_action_setsort_triggered()
{
    Dialog_setsort* dialog=new Dialog_setsort(this);
    dialog->show();
}

void Manager_MainWindow::on_action_studentnumber_triggered()
{
    if(!(now_page==2&&now_state==state_lesson))
        return ;
    if(ui->action_New->isChecked()||ui->action_edit->isChecked())
        return ;
    int count=lesson_object->stuscore.count();
    if(count==0||count==1)
        return ;
    cp_stuscore=new score[count];
    for(int i=0;i<count;i++)
    {
        cp_stuscore[i].studentID=lesson_object->stuscore[i].studentID;
        cp_stuscore[i]._score=lesson_object->stuscore[i]._score;
    }
    sort_stu_id();
    for(int i=0;i<count;i++)
        ui->table_lesson->removeRow(0);
    for(int i=0;i<count;i++)
    {
        addrow_table_lesson(student(cp_stuscore[i].studentID).name(),cp_stuscore[i].studentID,cp_stuscore[i]._score,false);
    }
    delete cp_stuscore;
}

void Manager_MainWindow::on_action_score_triggered()
{
    if(now_page!=2||now_state==state_teacher)
        return ;
    if(ui->action_New->isChecked()||ui->action_edit->isChecked())
        return ;
    if(now_state==state_lesson)
    {
        int count=ui->table_lesson->rowCount();
        if(count==0||count==1)
            return ;
        cp_stuscore=new score[count];
        for(int i=0;i<count;i++)
        {
            cp_stuscore[i].studentID=lesson_object->stuscore[i].studentID;
            cp_stuscore[i]._score=lesson_object->stuscore[i]._score;
        }
        sort_stu_score();
        for(int i=0;i<count;i++)
            ui->table_lesson->removeRow(0);
        for(int i=0;i<count;i++)
        {
            addrow_table_lesson(student(cp_stuscore[i].studentID).name(),cp_stuscore[i].studentID,cp_stuscore[i]._score,false);
        }
        delete cp_stuscore;
        return ;
    }
    if(now_state==state_student)
    {
        int count=student_object->lessonID.count();
        if(count==0||count==1)
            return ;
        cp_lesscore=new score_of_lesson[count];
        for(int i=0;i<count;i++)
        {
            cp_lesscore[i].lessonID=student_object->lessonID[i];
            Lesson& nowles=lesson(cp_lesscore[i].lessonID);
            for(int j=0;j<nowles.stuscore.count();j++)
            {
                if(student_object->ID()==nowles.stuscore[j].studentID)
                {
                    cp_lesscore[i]._score=nowles.stuscore[j]._score;
                    break;
                }
            }
        }
        sort_les_score();
        for(int i=0;i<count;i++)
            ui->table_student->removeRow(0);
        for(int i=0;i<count;i++)
        {
            Lesson& nowles=lesson(cp_lesscore[i].lessonID);
            addrow_table_student(nowles.name(),nowles.ID(),teacher(nowles.teacherID()).name(),nowles.credit(),cp_lesscore[i]._score,false);
        }
        delete cp_lesscore;
        return ;
    }
}

