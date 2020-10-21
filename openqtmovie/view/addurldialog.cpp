#include "addurldialog.h"
#include "ui_addurldialog.h"

#include "application.h"

addUrlDialog::addUrlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addUrlDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("打开URL地址"));
}

addUrlDialog::~addUrlDialog()
{
    delete ui;
}

void addUrlDialog::on_addBtn_clicked()
{
    emit App->sigopenUrlStr(ui->lineEdit->text());
    this->close();
}

void addUrlDialog::on_cancelBtn_clicked()
{
    this->close();
}
