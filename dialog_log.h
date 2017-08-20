#ifndef DIALOG_LOG_H
#define DIALOG_LOG_H

#include <QDialog>

namespace Ui {
class Dialog_Log;
}

class Dialog_Log : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Log(QWidget *parent = 0);
    ~Dialog_Log();

private slots:

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_leave_clicked();

    void on_pushButton_log_clicked();

    void on_lineedit_account_returnPressed();

private:
    Ui::Dialog_Log *ui;
    long qstr_to_long(QString qs);
};

#endif // DIALOG_LOG_H
