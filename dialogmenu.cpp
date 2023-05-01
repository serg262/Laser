#include "dialogmenu.h"
#include "ui_dialogmenu.h"
#include "mymessage.h"

#include <QProcess>

DialogMenu::DialogMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMenu)
{
    ui->setupUi(this);

    this->setModal(true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::Widget);

}

DialogMenu::~DialogMenu()
{
    delete ui;
}



void DialogMenu::on_pushButton_cancel_clicked()
{
    this->close();
}

void DialogMenu::on_pushButton_exit_clicked()
{
    MyMessage *message = new MyMessage(this);

    message->set_text("ВЫКЛЮЧИТЬ КОМПЬЮТЕР?");//yes no

    int dialogCode = message->exec();



    if(dialogCode == QDialog::Accepted)
    {
        QApplication::quit();
        QProcess::startDetached("shutdown /s /t 0");


    }
    else
        QApplication::quit();



}

void DialogMenu::on_pushButton_reboot_clicked()
{
    QApplication::quit();
    QProcess::startDetached("shutdown  /r /t 0");
}
