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

private:
    Ui::Dialog_Log *ui;
};

#endif // DIALOG_LOG_H
