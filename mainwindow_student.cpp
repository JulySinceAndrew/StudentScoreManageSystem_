#include "mainwindow_student.h"
#include "ui_mainwindow_student.h"
#include"fstream"
#include"sstream"
#include<QLineEdit>
#include<QComboBox>
#include<QDebug>
#include"dialog_setsort.h"

extern bool sort_modal;
extern long log_student_id;

#define row_height 30
#define top_height 50
#define heiht_max 380

using std::fstream;
using std::ios_base;
using std::ostringstream;
using std::istringstream;

MainWindow_Student::MainWindow_Student(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow_Student)
{
    ui->setupUi(this);
    setFixedSize(800,600);
    QLabel* label_takeupspace=new QLabel(this);
    label_takeupspace->setFixedSize(32,32);
    label_takeupspace->setPixmap(QPixmap(":/image/images/学生成绩管理系统.png"));
    label_takeupspace->setScaledContents(true);
    ui->toolBar_2->insertWidget(ui->action_score,label_takeupspace);
    ui->toolBar_2->insertSeparator(ui->action_score);
    ui->table_info1->setVisible(false);
    ui->table_info2->setVisible(false);
    now_page=0;
    stuid=log_student_id;
    ui->label_change->setText(long_to_qstr(stuid));
    open_all();
}

MainWindow_Student::~MainWindow_Student()
{
    delete ui;
}

void MainWindow_Student::open_all()
{
    open_a_student_file();
    open_lesson_file();
    open_teacher_file();
    open_a_student();
    int w=ui->table_student->size().width();
    int h=ui->table_student->rowCount()*row_height+top_height;
    if(h>352)
    h=352;
    ui->table_student->resize(w,h);
}

void MainWindow_Student::open_a_student_file()
{
    QString prefix="stu_";
    QString suffix=".bin";
    QString filename=prefix+long_to_qstr(stuid)+suffix;
    fstream finstu;
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
    a_student=new Student(id,name,sex);
    long lessonid;
    for(int j=0;j<n;j++)
    {
        finstu.read((char*)(&lessonid),sizeof(long));
        a_student->lessonID.add(lessonid);
    }
    finstu.close();
    delete []name;
}

void MainWindow_Student::open_lesson_file()
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

void MainWindow_Student::open_teacher_file()
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

void MainWindow_Student::open_a_student()
{
    QLineEdit* lineedit;
    QComboBox* combobox;
    lineedit=new QLineEdit(this);
    lineedit->setText(a_student->name());
    lineedit->setReadOnly(true);
    ui->table_student_total->setCellWidget(0,0,lineedit);
    lineedit=new QLineEdit(this);
    lineedit->setText(long_to_qstr(a_student->ID()));
    lineedit->setReadOnly(true);
    ui->table_student_total->setCellWidget(0,1,lineedit);
    combobox=new QComboBox(this);
    combobox->addItem("男");
    combobox->addItem("女");
    combobox->setEditable(false);
    if(a_student->sex()==man)
        combobox->setCurrentIndex(0);
    else
        combobox->setCurrentIndex(1);
    combobox->setEnabled(false);
    ui->table_student_total->setCellWidget(0,2,combobox);
    double nowcredit,nowscore;
    double totalcredit=0;
    double totalgpa=0;
    double totalscore=0;
    bool flag=false;
    for(int i=0;i<a_student->lessonID.count();i++)
    {
        Lesson& now=lesson(a_student->lessonID[i]);
        for(int j=0;j<now.stuscore.count();j++)
        {
            if(now.stuscore[j].studentID==a_student->ID())
            {
                nowscore=now.stuscore[j]._score;
                break;
            }
        }
        nowcredit=now.credit();
        addrow_table_student(now.name(),now.ID(),teacher(now.teacherID()).name(),nowcredit,nowscore);
        if(nowscore>=0)
        {
            totalcredit+=nowcredit;
            totalscore+=nowcredit*nowscore;
            totalgpa+=nowcredit*score_to_gpa(nowscore);
            flag=true;
        }
    }
    if(flag)
    {
        lineedit=new QLineEdit(this);
        lineedit->setText(avescore_to_qstr(totalscore/totalcredit));
        lineedit->setReadOnly(true);
        ui->table_student_total->setCellWidget(0,3,lineedit);
        lineedit=new QLineEdit(this);
        lineedit->setText(avegpa_to_qstr(totalgpa/totalcredit));
        lineedit->setReadOnly(true);
        ui->table_student_total->setCellWidget(0,4,lineedit);
    }
}

void MainWindow_Student::addrow_table_student(QString lessonname, long id, QString teachername, int credit, int score)
{
    int count=ui->table_student->rowCount();
    ui->table_student->insertRow(count);
    QLineEdit* lineedit;
    lineedit=new QLineEdit(this);
    lineedit->setText(lessonname);
    ui->table_student->setCellWidget(count,0,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit(this);
    lineedit->setText(long_to_qstr(id));
    ui->table_student->setCellWidget(count,1,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit(this);
    lineedit->setText(teachername);
    ui->table_student->setCellWidget(count,2,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit(this);
    lineedit->setText(int_to_qstr(credit));
    ui->table_student->setCellWidget(count,3,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit(this);
    lineedit->setText(int_to_qstr(score));
    ui->table_student->setCellWidget(count,4,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit(this);
    lineedit->setText(score_to_str_gpa(score));
    ui->table_student->setCellWidget(count,5,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit(this);
    lineedit->setText(score_to_level(score));
    ui->table_student->setCellWidget(count,6,lineedit);
    lineedit->setReadOnly(true);
}

QString MainWindow_Student::long_to_qstr(long n)
{
    ostringstream os;
    os<<n;
    return QString::fromStdString(os.str());
}

QString MainWindow_Student::int_to_qstr(int n)
{
    ostringstream os;
    os<<n;
    return QString::fromStdString(os.str());
}

long MainWindow_Student::qstr_to_long(QString qs)
{
    istringstream is(qs.toStdString());
    long id;
    is>>id;
    return id;
}

QString MainWindow_Student::avescore_to_qstr(double avescore)
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

QString MainWindow_Student::avegpa_to_qstr(double avegpa)
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

QString MainWindow_Student::percentage_to_qstr(double per)
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

void MainWindow_Student::sort_les_score()
{
    long tempid;
    int tempscore;
    int count=a_student->lessonID.count();
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

void MainWindow_Student::set_info(long id)
{
    ui->table_info1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_info2->setEditTriggers(QAbstractItemView::NoEditTriggers);qDebug()<<"debudao";
    Lesson now=lesson(id);
    ui->table_info1->setItem(0,0,new QTableWidgetItem(now.name()));
    ui->table_info1->setItem(0,1,new QTableWidgetItem(long_to_qstr(now.ID())));
    ui->table_info1->setItem(0,2,new QTableWidgetItem(teacher(now.teacherID()).name()));
    ui->table_info1->setItem(0,3,new QTableWidgetItem(int_to_qstr(now.credit())));
    ui->table_info1->setItem(0,4,new QTableWidgetItem(int_to_qstr(now.stuscore.count())));
    double count_pass=0;
    double count_havescore=0;
    double count_havelevel=0;
    double nowscore;
    double totalscore=0;
    double totalgpa=0;
    int youscore;
    Vector<int> stuscore;
    for(int i=0;i<now.stuscore.count();i++)
    {
        nowscore=now.stuscore[i]._score;
        if(nowscore!=-3)
        {
            count_havescore++;
            if(nowscore==-1||nowscore>=60)
                count_pass++;
        }
        if(nowscore>=0)
        {
            count_havelevel++;
            stuscore.add(nowscore);
            totalscore+=nowscore;
            totalgpa+=score_to_gpa(nowscore);
        }
        if(now.stuscore[i].studentID==a_student->ID())
            youscore=nowscore;
    }
    if(count_havescore==0)
    {
        ui->table_info1->setItem(0,5,new QTableWidgetItem(QString("****")));
        ui->table_info1->setItem(0,6,new QTableWidgetItem(QString("****")));
        ui->table_info2->setItem(0,0,new QTableWidgetItem(int_to_qstr(youscore)));
        ui->table_info2->setItem(0,1,new QTableWidgetItem(score_to_str_gpa(youscore)));
        ui->table_info2->setItem(0,2,new QTableWidgetItem(score_to_level(youscore)));
        ui->table_info2->setItem(1,0,new QTableWidgetItem(QString("****")));
        ui->table_info2->setItem(1,1,new QTableWidgetItem(QString("****")));
        ui->table_info2->setItem(2,0,new QTableWidgetItem(QString("****")));
        ui->table_info2->setItem(2,1,new QTableWidgetItem(QString("****")));
        return ;
    }
    if(count_havelevel==0)
    {
        ui->table_info1->setItem(0,5,new QTableWidgetItem(percentage_to_qstr(count_pass/count_havescore)));
        ui->table_info1->setItem(0,6,new QTableWidgetItem(percentage_to_qstr(1-count_pass/count_havescore)));
        ui->table_info2->setItem(0,0,new QTableWidgetItem(int_to_qstr(youscore)));
        ui->table_info2->setItem(0,1,new QTableWidgetItem(score_to_str_gpa(youscore)));
        ui->table_info2->setItem(0,2,new QTableWidgetItem(score_to_level(youscore)));
        ui->table_info2->setItem(1,0,new QTableWidgetItem(QString("N/A")));
        ui->table_info2->setItem(1,1,new QTableWidgetItem(QString("N/A")));
        ui->table_info2->setItem(2,0,new QTableWidgetItem(QString("N/A")));
        ui->table_info2->setItem(2,1,new QTableWidgetItem(QString("N/A")));
        return ;
    }
    int temp;
    for(int i=0;i<count_havelevel-1;i++) //从小到大排序
    {
        for(int j=0;j<count_havelevel-i-1;j++)
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
    if(int(count_havelevel)%2==1)
    {
        midscore=stuscore[int(count_havelevel)/2];
        midgpa=score_to_gpa(midscore);
    }
    else
    {
        midscore=double(stuscore[int(count_havelevel)/2]+stuscore[int(count_havelevel)/2-1])/2;
        midgpa=double(score_to_gpa(stuscore[int(count_havelevel)/2])+score_to_gpa(stuscore[int(count_havelevel)/2-1]))/2;
    }
    ui->table_info1->setItem(0,5,new QTableWidgetItem(percentage_to_qstr(count_pass/count_havescore)));
    ui->table_info1->setItem(0,6,new QTableWidgetItem(percentage_to_qstr(1-count_pass/count_havescore)));
    ui->table_info2->setItem(0,0,new QTableWidgetItem(int_to_qstr(youscore)));
    ui->table_info2->setItem(0,1,new QTableWidgetItem(score_to_str_gpa(youscore)));
    ui->table_info2->setItem(0,2,new QTableWidgetItem(score_to_level(youscore)));
    ui->table_info2->setItem(1,0,new QTableWidgetItem(avescore_to_qstr(totalscore/count_havelevel)));
    ui->table_info2->setItem(1,1,new QTableWidgetItem(avegpa_to_qstr(totalgpa/count_havelevel)));
    ui->table_info2->setItem(2,0,new QTableWidgetItem(avescore_to_qstr(midscore)));
    ui->table_info2->setItem(2,1,new QTableWidgetItem(avegpa_to_qstr(midgpa)));
}

QString MainWindow_Student::score_to_level(double score)
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

double MainWindow_Student::score_to_gpa(double score)
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

QString MainWindow_Student::score_to_str_gpa(double score)
{
    if(score==-1)
        return "N/A";
    else if(score==-2)
        return "N/A";
    else if(score==-3)
        return "*****";
    ostringstream os;
    double sco=score_to_gpa(score);
    os<<sco;
    if(sco==4||sco==3||sco==2||sco==1)
    os<<".0";
    return QString::fromStdString(os.str());

}

void MainWindow_Student::on_action_setsort_triggered()
{
    Dialog_setsort* dialog=new Dialog_setsort(this);
    dialog->show();
}

void MainWindow_Student::on_action_score_triggered()
{
    if(now_page==1)
        return ;
    int count=a_student->lessonID.count();
    if(count==0||count==1)
        return ;
    cp_lesscore=new score_of_lesson[count];
    for(int i=0;i<count;i++)
    {
        cp_lesscore[i].lessonID=a_student->lessonID[i];
        Lesson& nowles=lesson(cp_lesscore[i].lessonID);
        for(int j=0;j<nowles.stuscore.count();j++)
        {
            if(a_student->ID()==nowles.stuscore[j].studentID)
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
        addrow_table_student(nowles.name(),nowles.ID(),teacher(nowles.teacherID()).name(),nowles.credit(),cp_lesscore[i]._score);
    }
    delete cp_lesscore;
    return ;
}

void MainWindow_Student::on_action_look_triggered()
{
    if(now_page==1)
        return ;
    int count=ui->table_student->currentRow();
    if(count==-1)
        return ;
    now_page=1;
    ui->table_student->setVisible(false);
    ui->table_info1->setVisible(true);
    ui->table_info2->setVisible(true);
    QLineEdit* lineedit=(QLineEdit*)ui->table_student->cellWidget(count,1);
    set_info(qstr_to_long(lineedit->text()));
}

void MainWindow_Student::on_action_back_triggered()
{
    if(now_page==0)
        return ;
    now_page=0;
    ui->table_info1->setVisible(false);
    ui->table_info2->setVisible(false);
    ui->table_student->setVisible(true);
}
