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
    open_file();
    QLabel* label_takeupspace=new QLabel(this);
    label_takeupspace->setFixedSize(32,32);
    label_takeupspace->setPixmap(QPixmap(":/image/images/背景.png"));
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

void Manager_MainWindow::addrow_table_stuortea(QString name, long id, bool sex,bool editable)
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

void Manager_MainWindow::addrow_table_lesson(QString lessonname, long id, QString teachername,bool editable)
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
    lineedit->setText(avescore_to_qstr(avescore));
    ui->table_student_total->setCellWidget(0,3,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit(this);
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

void Manager_MainWindow::open_studentlist()
{
    ui->table->horizontalHeaderItem(0)->setText("姓名");
    ui->table->horizontalHeaderItem(1)->setText("学号");
    ui->table->horizontalHeaderItem(2)->setText("性别");
    for(int i=0;i<student.count();i++)
        addrow_table_stuortea(student[i].name(),student[i].ID(),student[i].sex(),false);
    table_resize();
}

void Manager_MainWindow::open_teacherlist()
{
    ui->table->horizontalHeaderItem(0)->setText("姓名");
    ui->table->horizontalHeaderItem(1)->setText("教职工号");
    ui->table->horizontalHeaderItem(2)->setText("性别");
    for(int i=0;i<teacher.count();i++)
        addrow_table_stuortea(teacher[i].name(),teacher[i].ID(),teacher[i].sex(),false);
    table_resize();
}

void Manager_MainWindow::open_lessonlist()
{
    ui->table->horizontalHeaderItem(0)->setText("课程名");
    ui->table->horizontalHeaderItem(1)->setText("课程号");
    ui->table->horizontalHeaderItem(2)->setText("教师");
    for(int i=0;i<teacher.count();i++)
        addrow_table_lesson(lesson[i].name(),lesson[i].ID(),teacher(lesson[i].teacherID()).name(),false);
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
    }qDebug()<<totalscore<<totalgpa<<totalcredit;
    addrow_table_totalstu(student_object->name(),student_object->ID(),student_object->sex(),totalscore/totalcredit,totalgpa/totalcredit);
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
    QRegExp rx("2[01][0-9]{8}");
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

long Manager_MainWindow::qstr_to_long(QString qs)
{
    istringstream is(qs.toStdString());
    long id;
    is>>id;
    return id;
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
    set_serach_visible(flag);
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
    clear_alltable();
    set_welcome_visible(!flag);
    set_serach_visible(flag);
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
    clear_alltable();
    set_welcome_visible(!flag);
    set_serach_visible(flag);
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

void Manager_MainWindow::table_resize()
{
    int w=ui->table->size().width();
    int h=ui->table->rowCount()*row_height+top_height;
    if(h>352)
    h=352;
    ui->table->resize(w,h);
}

void Manager_MainWindow::tablestudent_resize()
{
    int w=ui->table_student->size().width();
    int h=ui->table_student->rowCount()*row_height+top_height;
    if(h>352)
    h=352;
    ui->table_student->resize(w,h);
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

void Manager_MainWindow::set_all_visible(bool arg)
{
    set_welcome_visible(arg);
    set_serach_visible(arg);
    set_studenttable_visible(arg);
    set_table_visivle(arg);
}

void Manager_MainWindow::on_action_Look_triggered()
{
    int count=ui->table->currentRow();
    if(count==-1)
        return ;
    QLineEdit *lineedit=(QLineEdit*)ui->table->cellWidget(count,1);
    long id=qstr_to_long(lineedit->text());
    for(int i=0;i<student.count();i++)
    {
        if(student[i].ID()==id)
        {
            student_object=&student[i];
            break;
        }
    }
    open_student();
    set_table_visivle(false);
    set_studenttable_visible(true);
}

void Manager_MainWindow::on_action_back_triggered()
{
    clear_alltable();
}
