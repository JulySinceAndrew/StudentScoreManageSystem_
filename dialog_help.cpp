#include "dialog_help.h"
#include "ui_dialog_help.h"

Dialog_Help::Dialog_Help(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Help)
{
    ui->setupUi(this);
    setFixedSize(400,300);
}

Dialog_Help::~Dialog_Help()
{
    delete ui;
}

void Dialog_Help::on_pushButton_clicked()
{
    close();
}
