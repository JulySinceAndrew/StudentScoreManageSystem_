#ifndef MANAGER_MAINWINDOW_H
#define MANAGER_MAINWINDOW_H

#define state_welcome 0
#define state_student 1
#define state_teacher 2
#define state_lesson 3
#include <QMainWindow>
#include<QLabel>
#include<QLineEdit>
#include<QComboBox>
#include<QWidget>
#include<QPixmap>
#include<QDebug>
#include<QMouseEvent>
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

private slots:

    void on_action_student_triggered();

    void on_action_teacher_triggered();

    void on_action_lesson_triggered();

    void on_action_edit_triggered();

    void on_action_New_triggered();

private:
    Ui::Manager_MainWindow *ui;
    QLabel* label_takeupspace;
    int now_page;
    int now_state;
    vector_of_student student;
    vector_of_Teacher teacher;
    vector_of_Lesson lesson;

private:
    void set_welcome_visible(bool arg);
    void set_serach_visible(bool arg);
    void set_studenttable_visible(bool arg);
    void set_table_visivle(bool arg);
    void open_file();
    void open_student_file();
    void open_teacher_file();
    QString long_to_qstr(long n);

};

#endif // MANAGER_MAINWINDOW_H
