

#include <QtWidgets>
#include "mymessage.h"
//#include "mainwindow.h"
//#include "workwindow.h"
//#include "global.h"





MyMessage::MyMessage(QWidget *parent)
    : QDialog(parent)
{
   // this->setModal(true);
    this->setAttribute(Qt::WA_DeleteOnClose);

    this->setWindowFlags(Qt::FramelessWindowHint |
                      Qt::Tool |
                       Qt::WindowStaysOnTopHint);

    this->setContentsMargins(6,6,6,6);
      this->setStyleSheet("QDialog {border: 6px solid gray; background-color: rgb(50,50,50);}");



    yesButton = new QPushButton(this);
    yesButton->setText("ДА");
    yesButton->installEventFilter(this);
    connect(yesButton, &QPushButton::clicked, [=](){this->accept();});

    noButton = new QPushButton(this);
    noButton->setText("НЕТ");
    noButton->installEventFilter(this);
    connect(noButton, &QPushButton::clicked, [=](){this->reject();});


    label = new QLabel();
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("QLabel{ color: white; font: 42pt;}");


    QString str = "QPushButton {font: 42pt; \
            color: white; \
            background-color: qlineargradient(spread:pad, x1:0.534, y1:0.289773, x2:0.534, y2:0, \
            stop:0 rgba(0, 0, 0, 255), \
            stop:1 rgba(255, 255, 255, 255));}";



    yesButton->setStyleSheet(str);

    noButton->setStyleSheet(str);





    yesButton->setFixedHeight(80);
    noButton->setFixedHeight(80);

    this->setFixedHeight(280);





}


MyMessage::~MyMessage()
{

}


//static
void MyMessage::warning(QWidget *parent, const QString &title, const QString &str)
{
   MyMessage *msg = new MyMessage(parent);

   msg->yesButton->setText("OK");
   connect(msg->yesButton, &QPushButton::clicked, [=](){msg->close();});
   msg->noButton->hide();
   msg->set_text(str);

   msg->exec();
}

void MyMessage::set_text(const QString &str)
{
    label->setText(str);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(label);
    this->setStyleSheet("color: white;");



        QHBoxLayout *mainLayout2 = new QHBoxLayout;
        mainLayout2->addWidget(yesButton);
        mainLayout2->addWidget(noButton);
        mainLayout->addLayout(mainLayout2);
        setLayout(mainLayout);
        setLayout(mainLayout2);
        //nextButton->hide();

}


/*
bool MyMessage::eventFilter(QObject *obj, QEvent *event)
{
QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

if(mouseEvent->button() == Qt::LeftButton && mouseEvent->type() == QMouseEvent::MouseButtonPress)
       {




        if((QPushButton*)obj == yesButton)
        {
            if(yesButton->text() == QString("ДА"))
            {


               //  emit signal_pressed(true);

          }
       }
        else
            if((QPushButton*)obj == noButton)
            {
                if(noButton->text() == QString("НЕТ"))
                {

                 //   emit signal_pressed(false);

                }
            }

//this->close();

       }
return false;
}
*/
