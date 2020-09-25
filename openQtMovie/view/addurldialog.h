#ifndef ADDURLDIALOG_H
#define ADDURLDIALOG_H

#include <QDialog>

namespace Ui {
class addUrlDialog;
}

class addUrlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addUrlDialog(QWidget *parent = nullptr);
    ~addUrlDialog();

private slots:
    void on_addBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::addUrlDialog *ui;
};

#endif // ADDURLDIALOG_H
