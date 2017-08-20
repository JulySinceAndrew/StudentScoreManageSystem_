#include "dialog_setsort.h"
#include "ui_dialog_setsort.h"

extern bool sort_modal;

Dialog_setsort::Dialog_setsort(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Dialog_setsort)
{
    ui->setupUi(this);
    setModal(true);
    setWindowTitle("设置升序/降序");
    setFixedSize(200,168);
    if(sort_modal==up)
    {
        ui->radioButton->setChecked(true);
        ui->radioButton_2->setChecked(false);
    }
    else
    {
        ui->radioButton->setChecked(false);
        ui->radioButton_2->setChecked(true);
    }
}

Dialog_setsort::~Dialog_setsort()
{
    delete ui;
}

void Dialog_setsort::on_pushButton_clicked()
{
    if(ui->radioButton->isChecked())
        sort_modal=up;
    else
        sort_modal=down;
    close();
}
