#include "addurldialog.h"
#include "ui_addurldialog.h"

addUrlDialog::addUrlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addUrlDialog)
{
    ui->setupUi(this);
}

addUrlDialog::~addUrlDialog()
{
    delete ui;
}

void addUrlDialog::on_addBtn_clicked()
{

}

void addUrlDialog::on_cancelBtn_clicked()
{

}
