#ifndef MAINWINDOW_TEACHER_H
#define MAINWINDOW_TEACHER_H

#include <QMainWindow>
#include"vector_of.h"
#include"vector"
#include"specific_person.h"
#include"lesson.h"
#include"fstream"
#include"sstream"
#include<QLineEdit>
#include<QCloseEvent>
#include"dialog_log.h"

using std::ios_base;
using std::fstream;
using std::ostringstream;
using std::istringstream;

namespace Ui {
class MainWindow_Teacher;
}

class MainWindow_Teacher : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_Teacher(QWidget *parent = 0);
    ~MainWindow_Teacher();
    struct score_of_lesson
    {
        long lessonID;
        int _score;
    };

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_action_back_triggered();

    void on_action_look_triggered();

    void on_action_edit_triggered();

    void on_action_save_triggered();

    void on_action_setsort_triggered();

    void on_action_score_triggered();

    void on_action_stuid_triggered();

    void on_action_leave_triggered();

    void on_action_relog_triggered();

private:
    Ui::MainWindow_Teacher *ui;
    long teaid;
    int now_page;
    Teacher* a_teacher;
    Lesson* lesson_object;
    score* cp_stuscore;
    vector_of_student student;
    vector_of_Lesson lesson;

private:
    void open_all();
    void open_a_teacher_file();
    void open_lesson_file();
    void open_student_file();
    void save_lesson_file();
    void open_a_teacher();
    void open_lesson();
    void clear_lesson();
    void addrow_table_totalles(QString lessonname,long id,QString teachername,long teacherid,int credit,int stunumber,double avescore,double avegpa,double midscore,double midgpa,double passpencentage);
    void addrow_table_lesson(QString stuname,long stuid,int score,bool editable);
    bool check_lesson();
    QLineEdit* lineedit_score();
    QString long_to_qstr(long n);
    QString int_to_qstr(int n);
    long qstr_to_long(QString qs);
    int qstr_to_int(QString qs);
    QString score_to_level(double score);
    double score_to_gpa(double score);
    QString score_to_str_gpa(double score);
    QString avescore_to_qstr(double avescore);
    QString avegpa_to_qstr(double avegpa);
    QString percentage_to_qstr(double per);
    void set_info(long id);
    void sort_stu_id();
    void sort_stu_score();

};

#endif // MAINWINDOW_TEACHER_H
