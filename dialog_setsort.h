#ifndef DIALOG_SETSORT_H
#define DIALOG_SETSORT_H

#define up true
#define down false

#include <QDialog>

namespace Ui {
class Dialog_setsort;
}

class Dialog_setsort : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_setsort(QWidget *parent = 0);
    ~Dialog_setsort();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog_setsort *ui;

};

#endif // DIALOG_SETSORT_H
