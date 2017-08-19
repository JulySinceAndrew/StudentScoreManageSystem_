#ifndef MANAGER_MAINWINDOW_H
#define MANAGER_MAINWINDOW_H

#define state_welcome 0
#define state_student 1
#define state_teacher 2
#define state_lesson 3
#define isscore true
#define isgpa false
#include <QMainWindow>
#include<QLabel>
#include<QLineEdit>
#include<QComboBox>
#include<QWidget>
#include<QPixmap>
#include<QDebug>
#include<QMouseEvent>
#include<QCloseEvent>
#include"specific_person.h"
#include"lesson.h"
#include"vector.h"
#include"vector_of.h"

namespace Ui {
class Manager_MainWindow;
}

class Manager_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Manager_MainWindow(QWidget *parent = 0);
    ~Manager_MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:

    void on_action_student_triggered();

    void on_action_teacher_triggered();

    void on_action_lesson_triggered();

    void on_action_edit_triggered();

    void on_action_New_triggered();

    void on_action_Look_triggered();

    void on_action_back_triggered();

    void on_action_deleteobject_triggered();

private:
    Ui::Manager_MainWindow *ui;
    QLabel* label_takeupspace;
    int now_page;
    int now_state;
    vector_of_student student;
    vector_of_Teacher teacher;
    vector_of_Lesson lesson;
    Student* student_object;
    Teacher* teacher_object;
    Lesson* lesson_object;

private:
    void table_resize();
    void student_resize();
    void teacher_resize();
    void lesson_resize();
    void close_all();
    void clear_alltable();
    void clear_table();
    void clear_student();
    void clear_teacher();
    void clear_lesson();
    void set_all_visible(bool arg);
    void set_welcome_visible(bool arg);
    void set_serach_visible(bool arg);
    void set_studenttable_visible(bool arg);
    void set_teachertable_visible(bool arg);
    void set_lessontable_visible(bool arg);
    void set_table_visivle(bool arg);
    void open_file();
    void open_student_file();
    void open_teacher_file();
    void open_lesson_file();
    void save_student_file();
    void save_teacher_file();
    void save_lesson_file();
    void save_file();
    void addrow_table_stuortea(QString name,long id,bool sex,bool editable);
    void addrow_table_lessonlist(QString lessonname,long id,QString teachername,bool editable);
    void addrow_table_totalstu(QString name,long id,bool sex,double avescore,double gpa);
    void addrow_table_student(QString lessonname,long id,QString teachername,int credit,int score,bool editable);
    void addrow_table_totaltea(QString teachername,long id,bool sex,int lessoncount);
    void addrow_table_teacher(QString lessonname,long id,int credit,int stunumber,bool editable);
    void addrow_table_totalles(QString lessonname,long id,QString teachername,long teacherid,int credit,int stunumber,double avescore,double avegpa,double midscore,double midgpa,double passpencentage);
    void addrow_table_lesson(QString stuname,long stuid,int score,bool editable);
    void open_studentlist();
    void open_teacherlist();
    void open_lessonlist();
    void open_student();
    void open_teacher();
    void open_lesson();
    QString score_to_level(double score);
    double score_to_gpa(double score);
    QString score_to_str_gpa(double score);
    QString avescore_to_qstr(double avescore);
    QString avegpa_to_qstr(double avegpa);
    QLineEdit* lineedit_studentID();
    QLineEdit* lineedit_teacherID();
    QLineEdit* lineedit_lessonID();
    QLineEdit* lineedit_credit();
    QLineEdit* lineedit_score();
    QComboBox* combobox_sex();
    QString long_to_qstr(long n);
    QString int_to_qstr(int n);
    QString percentage_to_qstr(double per);
    long qstr_to_long(QString qs);
    int qstr_to_int(QString qs);

};

#endif // MANAGER_MAINWINDOW_H
