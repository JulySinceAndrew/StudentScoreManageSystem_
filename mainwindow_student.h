#ifndef MAINWINDOW_STUDENT_H
#define MAINWINDOW_STUDENT_H

#include <QMainWindow>
#include"specific_person.h"
#include"lesson.h"
#include"vector_of.h"
#include"dialog_log.h"
#include"fstream"
#include"sstream"
#include<QLineEdit>
#include<QComboBox>
#include<QDebug>
#include"dialog_setsort.h"
#include<QMessageBox>

namespace Ui {
class MainWindow_Student;
}

class MainWindow_Student : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_Student(QWidget *parent = 0);
    ~MainWindow_Student();
    struct score_of_lesson
    {
        long lessonID;
        int _score;
    };

private slots:
    void on_action_setsort_triggered();

    void on_action_score_triggered();

    void on_action_look_triggered();

    void on_action_back_triggered();

    void on_action_relog_triggered();

    void on_action_leave_triggered();

private:
    Ui::MainWindow_Student *ui;
    long stuid;
    Student* a_student;
    vector_of_Lesson lesson;
    vector_of_Teacher teacher;
    score_of_lesson* cp_lesscore;
    int now_page;

    void open_all();
    void open_a_student_file();
    void open_lesson_file();
    void open_teacher_file();
    void open_a_student();
    void addrow_table_student(QString lessonname,long id,QString teachername,int credit,int score);
    QString long_to_qstr(long n);
    QString int_to_qstr(int n);
    long qstr_to_long(QString qs);
    QString score_to_level(double score);
    double score_to_gpa(double score);
    QString score_to_str_gpa(double score);
    QString avescore_to_qstr(double avescore);
    QString avegpa_to_qstr(double avegpa);
    QString percentage_to_qstr(double per);
    void sort_les_score();
    void set_info(long id);
};

#endif // MAINWINDOW_STUDENT_H
