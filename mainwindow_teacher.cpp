#include "mainwindow_teacher.h"
#include "ui_mainwindow_teacher.h"
#include<QDebug>
#include<QLineEdit>
#include<QMessageBox>
#include"dialog_setsort.h"

#define row_height 30
#define top_height 50
#define heiht_max 380
#define up true
#define down false

extern bool sort_modal;
extern long log_teacher_id;

MainWindow_Teacher::MainWindow_Teacher(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow_Teacher)
{
    ui->setupUi(this);
    setFixedSize(800,600);
    QLabel* label_takeupspace=new QLabel(this);
    label_takeupspace->setFixedSize(32,32);
    label_takeupspace->setPixmap(QPixmap(":/image/images/学生成绩管理系统.png"));
    label_takeupspace->setScaledContents(true);
    ui->toolBar_2->insertWidget(ui->action_stuid,label_takeupspace);
    ui->toolBar_2->insertSeparator(ui->action_stuid);
    ui->table_lesson->setVisible(false);
    ui->table_lesson_total->setVisible(false);
    now_page=0;
    cp_stuscore=NULL;
    teaid=log_teacher_id;
    ui->label_3->setText(long_to_qstr(teaid));
    open_all();
}

MainWindow_Teacher::~MainWindow_Teacher()
{
    delete ui;
}

void MainWindow_Teacher::closeEvent(QCloseEvent *event)
{
    save_lesson_file();
}

void MainWindow_Teacher::open_all()
{
    open_a_teacher_file();
    open_lesson_file();
    open_student_file();
    open_a_teacher();
}

void MainWindow_Teacher::open_a_teacher_file()
{
    QString prefix="tea_";
    QString suffix=".bin";
    QString filename=prefix+long_to_qstr(teaid)+suffix;
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
    a_teacher=new Teacher(id,name,sex);
    long lessonid;
    for(int j=0;j<n;j++)
    {
        finstu.read((char*)(&lessonid),sizeof(long));
        a_teacher->lessonID.add(lessonid);
    }
    finstu.close();
    delete []name;
}

void MainWindow_Teacher::open_lesson_file()
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

void MainWindow_Teacher::open_student_file()
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

void MainWindow_Teacher::save_lesson_file()
{
    fstream fout;
    QString prefix="les_";
    QString suffix=".bin";
    QString filename;
    int count=lesson.count();
    long id;
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

void MainWindow_Teacher::open_a_teacher()
{
    ui->table_teacher_total->setItem(0,0,new QTableWidgetItem(a_teacher->name()));
    ui->table_teacher_total->setItem(0,1,new QTableWidgetItem(long_to_qstr(a_teacher->ID())));
    if(a_teacher->sex()==man)
        ui->table_teacher_total->setItem(0,2,new QTableWidgetItem("男"));
    else
        ui->table_teacher_total->setItem(0,2,new QTableWidgetItem("女"));
    ui->table_teacher_total->setItem(0,3,new QTableWidgetItem(int_to_qstr(a_teacher->lessonID.count())));
    for(int i=0;i<a_teacher->lessonID.count();i++)
    {
        Lesson& now=lesson(a_teacher->lessonID[i]);
        ui->table_teacher->insertRow(i);
        ui->table_teacher->setItem(i,0,new QTableWidgetItem(now.name()));
        ui->table_teacher->setItem(i,1,new QTableWidgetItem(long_to_qstr(now.ID())));
        ui->table_teacher->setItem(i,2,new QTableWidgetItem(int_to_qstr(now.credit())));
        ui->table_teacher->setItem(i,3,new QTableWidgetItem(int_to_qstr(now.stuscore.count())));
    }
    int w=ui->table_teacher->size().width();
    int h=ui->table_teacher->rowCount()*row_height+top_height;
    if(h>352)
    h=352;
    ui->table_teacher->resize(w,h);

}

void MainWindow_Teacher::open_lesson()
{
    double nowscore,nowgpa;
    double totalscore,totalgpa;
    totalgpa=totalscore=0;
    int count=lesson_object->stuscore.count();
    int count_havescore=0;
    int count_havelevel=0;
    int passcount=0;
    Vector<int> stuscore;
    Student now;
    if(count==0)
    {
       addrow_table_totalles(lesson_object->name(),lesson_object->ID(),a_teacher->name(),lesson_object->teacherID(),lesson_object->credit(),lesson_object->stuscore.count(),-5,-5,-5,-5,-5);//-5代表空
       return ;
    }
    for(int i=0;i<count;i++)
    {
        now=student(lesson_object->stuscore[i].studentID);
        nowscore=lesson_object->stuscore[i]._score;
        if(nowscore!=-3)
        {
            count_havescore++;
            if(nowscore==-1||nowscore>=60)
                passcount++;
        }
        if(nowscore>=0)
        {
            count_havelevel++;
            stuscore.add(nowscore);qDebug()<<nowscore<<lesson_object->stuscore[i].studentID;
        }
        addrow_table_lesson(now.name(),now.ID(),nowscore,false);
        nowgpa=score_to_gpa(nowscore);
        if(nowscore>=0)
        {
           totalgpa+=nowgpa;
           totalscore+=nowscore;
        }
    }
    int w=ui->table_lesson->size().width();
    int h=ui->table_lesson->rowCount()*row_height+top_height;
    if(h>352)
    h=352;
    ui->table_lesson->resize(w,h);
    if(count_havescore==0)
    {
        addrow_table_totalles(lesson_object->name(),lesson_object->ID(),a_teacher->name(),lesson_object->teacherID(),lesson_object->credit(),lesson_object->stuscore.count(),-5,-5,-5,-5,-5);//-5代表空
        return ;
    }
    double avescore,avegpa;
    avescore=totalscore/count_havelevel;
    avegpa=totalgpa/count_havelevel;
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
    if(count_havelevel%2==1)
    {
        midscore=stuscore[count_havelevel/2];
        midgpa=score_to_gpa(midscore);
    }
    else
    {
        midscore=double(stuscore[count_havelevel/2]+stuscore[count_havelevel/2-1])/2;
        midgpa=double(score_to_gpa(stuscore[count_havelevel/2])+score_to_gpa(stuscore[count_havelevel/2-1]))/2;
    }
    addrow_table_totalles(lesson_object->name(),lesson_object->ID(),a_teacher->name(),lesson_object->teacherID(),lesson_object->credit(),lesson_object->stuscore.count(),avescore,avegpa,midscore,midgpa,double(passcount)/double(count_havescore));
}

void MainWindow_Teacher::clear_lesson()
{
    int count;
    count=ui->table_lesson_total->columnCount();
    if(count==1)
        ui->table_lesson_total->removeColumn(0);
    count=ui->table_lesson->rowCount();
    for(int i=0;i<count;i++)
        ui->table_lesson->removeRow(0);
}

void MainWindow_Teacher::addrow_table_totalles(QString lessonname, long id, QString teachername, long teacherid, int credit, int stunumber, double avescore, double avegpa, double midscore, double midgpa, double passpencentage)
{
    ui->table_lesson_total->insertColumn(0);
    QLineEdit* lineedit;
    lineedit=new QLineEdit(this);
    lineedit->setText(lessonname);
    ui->table_lesson_total->setCellWidget(0,0,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit;
    lineedit->setText(long_to_qstr(id));
    ui->table_lesson_total->setCellWidget(1,0,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit(this);
    lineedit->setText(teachername);
    ui->table_lesson_total->setCellWidget(2,0,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit;
    lineedit->setText(long_to_qstr(teacherid));
    ui->table_lesson_total->setCellWidget(3,0,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit;
    lineedit->setText(int_to_qstr(credit));
    ui->table_lesson_total->setCellWidget(4,0,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit(this);
    lineedit->setText(int_to_qstr(stunumber));
    ui->table_lesson_total->setCellWidget(5,0,lineedit);
    lineedit->setReadOnly(true);
    if(avescore==-5)
        return ;
    lineedit=new QLineEdit;
    lineedit->setText(percentage_to_qstr(passpencentage));
    ui->table_lesson_total->setCellWidget(10,0,lineedit);
    lineedit->setReadOnly(true);
    lineedit=new QLineEdit(this);
    lineedit->setText(percentage_to_qstr(1-passpencentage));
    ui->table_lesson_total->setCellWidget(11,0,lineedit);
    lineedit->setReadOnly(true);
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

void MainWindow_Teacher::addrow_table_lesson(QString stuname, long stuid, int score, bool editable)
{
    int count=ui->table_lesson->rowCount();
    ui->table_lesson->insertRow(count);
    QLineEdit* lineedit;
    lineedit=new QLineEdit(this);
    lineedit->setText(stuname);
    ui->table_lesson->setCellWidget(count,0,lineedit);
    lineedit->setReadOnly(!editable);
    lineedit=new QLineEdit(this);
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

bool MainWindow_Teacher::check_lesson()
{
    QLineEdit* lineedit;
    for(int i=0;i<ui->table_lesson->rowCount();i++)
    {
        lineedit=(QLineEdit*)ui->table_lesson->cellWidget(i,2);
        if(lineedit->text()==QString())
        {
            QString text="第"+int_to_qstr(i+1)+"行成绩为空，请输入成绩";
            QMessageBox::critical(this,"成绩禁止为空！",text,QMessageBox::Yes);
            return false;
        }
    }
    return true;
}

QLineEdit *MainWindow_Teacher::lineedit_score()
{
    QLineEdit* lineedit=new QLineEdit(this);
    QRegExp rx("^[0-9]{1,2}$|^100$|^-1$|^-2$|^-3$");
    QValidator *val=new QRegExpValidator(rx,this);
    lineedit->setValidator(val);
    return lineedit;
}

QString MainWindow_Teacher::long_to_qstr(long n)
{
    ostringstream os;
    os<<n;
    return QString::fromStdString(os.str());
}

QString MainWindow_Teacher::int_to_qstr(int n)
{
    ostringstream os;
    os<<n;
    return QString::fromStdString(os.str());
}

long MainWindow_Teacher::qstr_to_long(QString qs)
{
    istringstream is(qs.toStdString());
    long id;
    is>>id;
    return id;
}

int MainWindow_Teacher::qstr_to_int(QString qs)
{
    istringstream is(qs.toStdString());
    long id;
    is>>id;
    return id;
}


QString MainWindow_Teacher::score_to_level(double score)
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

double MainWindow_Teacher::score_to_gpa(double score)
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

QString MainWindow_Teacher::score_to_str_gpa(double score)
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

QString MainWindow_Teacher::avescore_to_qstr(double avescore)
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

QString MainWindow_Teacher::avegpa_to_qstr(double avegpa)
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

QString MainWindow_Teacher::percentage_to_qstr(double per)
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

void MainWindow_Teacher::sort_stu_id()
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

void MainWindow_Teacher::sort_stu_score()
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

void MainWindow_Teacher::on_action_back_triggered()
{
    if(now_page==0||ui->action_edit->isChecked())
        return ;
    now_page=0;
    ui->table_teacher->setVisible(true);
    ui->table_teacher_total->setVisible(true);
    ui->table_lesson->setVisible(false);
    ui->table_lesson_total->setVisible(false);
    clear_lesson();
}

void MainWindow_Teacher::on_action_look_triggered()
{
    if(now_page==1)
        return ;
    int count=ui->table_teacher->currentRow();
    if(count==-1)
        return ;
    now_page=1;
    ui->table_teacher->setVisible(false);
    ui->table_teacher_total->setVisible(false);
    ui->table_lesson->setVisible(true);
    ui->table_lesson_total->setVisible(true);
    lesson_object=&lesson(qstr_to_long(ui->table_teacher->item(count,1)->text()));
    open_lesson();
}

void MainWindow_Teacher::on_action_edit_triggered()
{
    bool flag=ui->action_edit->isChecked();
    if(now_page==0||flag==false)
    {
        ui->action_edit->setChecked(!flag);
        return ;
    }
    QLineEdit* lineedit;
    for(int i=0;i<ui->table_lesson->rowCount();i++)
    {
        lineedit=(QLineEdit*)ui->table_lesson->cellWidget(i,2);
        lineedit->setReadOnly(false);
    }
}

void MainWindow_Teacher::on_action_save_triggered()
{
    if(now_page==0||!ui->action_edit->isChecked())
        return ;
    if(check_lesson())
    {
        ui->action_edit->setChecked(false);
        int count=lesson_object->stuscore.count();
        for(int i=0;i<count;i++)
            lesson_object->stuscore.remove_index(0);
        QLineEdit* stuid;
        QLineEdit* stuscore;
        for(int i=0;i<ui->table_lesson->rowCount();i++)
        {
            stuid=(QLineEdit*)ui->table_lesson->cellWidget(i,1);
            stuscore=(QLineEdit*)ui->table_lesson->cellWidget(i,2);
            lesson_object->stuscore.add(score(qstr_to_long(stuid->text()),qstr_to_int(stuscore->text())));
        }
        clear_lesson();
        open_lesson();
    }
}

void MainWindow_Teacher::on_action_setsort_triggered()
{
    Dialog_setsort* dialog=new Dialog_setsort(this);
    dialog->show();
}

void MainWindow_Teacher::on_action_score_triggered()
{
    if(now_page==0||ui->action_edit->isChecked())
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
    sort_stu_score();
    for(int i=0;i<count;i++)
        ui->table_lesson->removeRow(0);
    for(int i=0;i<count;i++)
    {
        addrow_table_lesson(student(cp_stuscore[i].studentID).name(),cp_stuscore[i].studentID,cp_stuscore[i]._score,false);
    }
    delete cp_stuscore;
}

void MainWindow_Teacher::on_action_stuid_triggered()
{
    if(now_page==0||ui->action_edit->isChecked())
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

void MainWindow_Teacher::on_action_leave_triggered()
{
    int answer=QMessageBox::question(this,"退出程序","请问您是否确定要退出当学生成绩管理系统？",QMessageBox::Yes,QMessageBox::No);
    if(answer==QMessageBox::Yes)
    {
        close();
    }
}

void MainWindow_Teacher::on_action_relog_triggered()
{
    if(ui->action_edit->isChecked())
    {
        QMessageBox::critical(this,"保存修改！","请先保存当前修改，再退出登录，否则当前修改会全部丢失！",QMessageBox::Ok);
        return ;
    }
    int answer=QMessageBox::question(this,"退出登录","请问您是否确定要退出当前登录？",QMessageBox::Yes,QMessageBox::No);
    if(answer==QMessageBox::Yes)
    {
        close();
        Dialog_Log* dialog=new Dialog_Log;
        dialog->show();
    }
}
