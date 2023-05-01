#include "dialoginput.h"
#include "ui_dialoginput.h"


#include "dialogedit.h"
#include "keyboard.h"
#include "mymessage.h"


#include <QMessageBox>




DialogInput::DialogInput(const QString &str, const QString &mess, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInput)
{
    ui->setupUi(this);

    ui->label_input->setText(mess);

   // this->setModal(true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::Widget);

    //в окно сообщений ?????????????????????????????????????????????????????????
    connect(ui->widget, &Keyboard::keyPressed, this, &DialogInput::slot_keyPressed);
    connect(ui->widget, SIGNAL(backspacePressed()), this, SLOT(slot_backspacePressed()));
    connect(ui->widget, SIGNAL(returnPressed()), this, SLOT(slot_returnPressed()));
    connect(ui->widget, &Keyboard::leftPressed, this, &DialogInput::slot_leftPressed);
    connect(ui->widget, &Keyboard::rightPressed, this, &DialogInput::slot_rightPressed);


    if(!str.isEmpty())
    {
        ui->lineEdit->setText(str);
        last_cursor_pos = str.length();
        flag_edit = true;
    }

     ui->lineEdit->setFocus();
}

DialogInput::~DialogInput()
{
    delete ui;
}


void DialogInput::on_pushButton_OK_clicked()
{

    if(!ui->lineEdit->text().isEmpty())
    {

  emit signal_OK_input(ui->lineEdit->text(), flag_edit);


  //  DialogEdit *win_edit = new DialogEdit(ui->lineEdit->text(), true, false, this);
  //  win_edit->setFixedSize(this->width(), this->height());
  //  win_edit->move(0,0);
  //  win_edit->show();

    this->close();
    }
    else
        //QMessageBox::warning(this, "Внимание","Пустая строка");
    MyMessage::warning(this, "Внимание", "Пустая строка!");

}

void DialogInput::on_pushButton_cancel_clicked()
{
    this->close();
}

void DialogInput::slot_leftPressed()
{
  //курсор влево
    last_cursor_pos = ui->lineEdit->cursorPosition();

    if(last_cursor_pos>0)
    ui->lineEdit->setCursorPosition(--last_cursor_pos);
}

void DialogInput::slot_rightPressed()
{
  //курсор вправо
    last_cursor_pos = ui->lineEdit->cursorPosition();

    if(last_cursor_pos<ui->lineEdit->text().length())
    ui->lineEdit->setCursorPosition(++last_cursor_pos);
}


void DialogInput::slot_keyPressed(const QChar &t)
{
    QString text = ui->lineEdit->text();
    text.insert(last_cursor_pos++,t);
    ui->lineEdit->setText(text);
    ui->lineEdit->setCursorPosition(last_cursor_pos);

    ui->lineEdit->setFocus();
}

void DialogInput::slot_returnPressed()
{
//???????????????

}

void DialogInput::slot_backspacePressed()
{
//удалить последний символ
    QString text = ui->lineEdit->text();
    if(!text.isEmpty())
    {
    text.remove(--last_cursor_pos,1);


    ui->lineEdit->setText(text);
    ui->lineEdit->setCursorPosition(last_cursor_pos);
    ui->lineEdit->setFocus();
    }
}

