#include "dialogmotion.h"
#include "ui_dialogmotion.h"

DialogMotion::DialogMotion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMotion)
{
    ui->setupUi(this);

  //  this->setModal(true);
    this->setAttribute(Qt::WA_DeleteOnClose);
}

DialogMotion::~DialogMotion()
{
    delete ui;
}

void DialogMotion::on_pushButton_5_clicked()
{
    emit signal_send(QByteArray("$H"));
}

void DialogMotion::on_pushButton_left_clicked()
{

    X_pos -= ui->doubleSpinBox_mm->value();

    emit signal_send(QByteArray(QString("X%0").arg(X_pos).toLocal8Bit()));
}

void DialogMotion::on_pushButton_right_clicked()
{

    X_pos += ui->doubleSpinBox_mm->value();
     emit signal_send(QByteArray(QString("X%0").arg(X_pos).toLocal8Bit()));
}

void DialogMotion::on_pushButton_up_clicked()
{
    //if(Y_pos<170.f)
    {
    Y_pos += ui->doubleSpinBox_mm->value();
    emit signal_send(QByteArray(QString("Y%0").arg(Y_pos).toLocal8Bit()));
    }
}

void DialogMotion::on_pushButton_down_clicked()
{
     Y_pos -= ui->doubleSpinBox_mm->value();
     emit signal_send(QByteArray(QString("Y%0").arg(Y_pos).toLocal8Bit()));
}

void DialogMotion::on_pushButton_cancel_clicked()
{
    this->close();
}

void DialogMotion::on_pushButton_stop_clicked()
{
    QByteArray arr;
    arr.append(char(24));//reset


     emit signal_send(arr);
}

void DialogMotion::on_pushButton_pos_clicked()
{
    emit signal_send(QByteArray("?"));
}

void DialogMotion::on_pushButton_unlook_clicked()
{
    emit signal_send(QByteArray("$X"));
}

void DialogMotion::set_text(const QString &str)
{
    ui->textEdit->insertPlainText(str);

    QTextCursor c = ui->textEdit->textCursor();
    c.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(c);

    float x,y;

    if(str.contains("MPos:"))
    {
        QString str2 = str;

    QStringList list =   str2.split(":");
QStringList list2 = list[1].split(",");
    if(list2.size()>2)
    {
    x=list2[0].toFloat();
    y=list2[1].toFloat();

X_pos=x;
Y_pos=y;


ui->label_x->setText(QString::number(X_pos));
ui->label_y->setText(QString::number(Y_pos));


    }


    }
}

void DialogMotion::on_pushButton_OK_clicked()
{
    QByteArray arr = QByteArray(ui->lineEdit->text().toLocal8Bit());

    emit signal_send(arr);
}
