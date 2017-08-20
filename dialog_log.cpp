#include "dialog_log.h"
#include "ui_dialog_log.h"

Dialog_Log::Dialog_Log(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Log)
{
    ui->setupUi(this);
}

Dialog_Log::~Dialog_Log()
{
    delete ui;
}
