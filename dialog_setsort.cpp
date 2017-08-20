#include "dialog_setsort.h"
#include "ui_dialog_setsort.h"

Dialog_setsort::Dialog_setsort(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Dialog_setsort)
{
    ui->setupUi(this);
    setModal(true);
    setWindowTitle("设置升序/降序");
    setFixedSize(200,168);
}

Dialog_setsort::~Dialog_setsort()
{
    delete ui;
}

void Dialog_setsort::on_pushButton_clicked()
{
    emit set_sort(ui->radioButton->isChecked());
    close();
}
