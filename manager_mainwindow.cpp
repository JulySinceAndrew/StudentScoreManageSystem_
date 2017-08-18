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
    //open_student_file();
    //open_teacher_file();
    //open_lesson_file();

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
    int nowscore,nowcredit,nowgpa;
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
    clear_alltable();
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
        return ;
    }
    now_page=1;
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
        return ;
    }
    now_page=1;
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
    }student_object->lessonID.add(20000000);lesson(20000000).stuscore.add(score(2016011073,97));
    open_student();
    set_table_visivle(false);
    set_studenttable_visible(true);
}

void Manager_MainWindow::on_action_back_triggered()
{
    clear_alltable();
}
