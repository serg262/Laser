#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mymessage.h"


#include <QStandardPaths>
#include <QSplashScreen>
#include <QElapsedTimer>
#include <QDebug>
#include <QTimer>
#include <QSound>


MainWindow::MainWindow(const QString &version, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


this->setWindowFlags(Qt::CustomizeWindowHint);


   ui->listWidget_2->addItem("Laser " + version);


/*
    QPixmap pix(":/Image/laser.jpg");
    QSplashScreen splashScreen(pix);


           splashScreen.setAttribute(Qt::WA_TranslucentBackground);
           splashScreen.setWindowFlags(Qt::FramelessWindowHint);
           splashScreen.setAutoFillBackground(false);

    QFont f(splashScreen.font());
    f.setPixelSize(25);

    splashScreen.setFont(f);

    splashScreen.show();

    QElapsedTimer time;
        time.start();
        while( time.elapsed() < 3000) {

            splashScreen.showMessage(
                "Подключение устройств...", Qt::AlignHCenter, Qt::white);
    }

*/


    QString path = QString(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/Laser/");

        QDir dir(path + "Gcoge/");
        if (!dir.exists())
            dir.mkpath(".");

        QDir dir2(path + "Layout/");
        if (!dir2.exists())
            dir2.mkpath(".");




    qApp->installEventFilter(this);



    QStringList list;

    for(const QSerialPortInfo &port : QSerialPortInfo::availablePorts())
           list.append(port.portName());

    // Инициализация последовательного порта.
        Laser_port = new QSerialPort(this);
        Laser_port->setPortName("COM3");
        // Скорость передачи данных. 19200 бит/с.
        Laser_port->setBaudRate(QSerialPort::Baud115200);
        Laser_port->setDataBits(QSerialPort::Data8);
        Laser_port->setParity(QSerialPort::NoParity);
        Laser_port->setStopBits(QSerialPort::OneStop);
        Laser_port->setFlowControl(QSerialPort::NoFlowControl);

        if (!Laser_port->open(QIODevice::ReadWrite)) {
              qDebug() << QString("Failed to open port %1, error: %2").arg(Laser_port->portName()).arg(Laser_port->errorString());

            }


        connect(Laser_port , SIGNAL(readyRead()),this,SLOT(readData()));




      //  QKeyEvent pressEnter(QEvent::KeyPress,0x01000005,Qt::NoModifier);//??????
      //  QApplication::sendEvent(ui->pushButton_OK, &pressEnter);




   QTimer::singleShot(2000, [=](){slot_send(QByteArray("$H"));});






}

MainWindow::~MainWindow()
{

   Laser_port->close();


    delete ui;
}



void MainWindow::on_pushButton_menu_clicked()
{




    int w = ui->label->height()+50;

    DialogMenu *win_menu = new DialogMenu(this);
    win_menu->setFixedSize(this->width(), this->height()-w);
    win_menu->move(0,w);
    win_menu->show();
}

void MainWindow::on_pushButton_message_clicked()
{

    int w = ui->label->height()+50;

    DialogMessage *win_message = new DialogMessage(this);
    //connect(win_message, &DialogMessage::signal_send_Gcode, this, &MainWindow::slot_send_Gcode);//????
    connect(win_message, &DialogMessage::signal_select, this, &MainWindow::slot_select);

    win_message->setFixedSize(this->width(), this->height()-w);
    win_message->move(0,w);
    win_message->show();
}


void MainWindow::readData()
{
    QByteArray arr = Laser_port->readAll();


if(!flag_first)
{
  slot_send(QByteArray("?"));//позиция

 flag_first = true;
}

if(arr == "ok\r\nok\r\n")
{
flag_OK = true;

}
if(!win_motion.isNull())
{
 win_motion->set_text(QString(arr));

}


if(flag_Gcode &&  flag_OK)
{

    QByteArray str = list_Gcode.at(count++).toLocal8Bit();

    if(str == "M2")//конец программы
    {
    flag_Gcode = false;

    count=0;
    ui->pushButton_start->setChecked(false);
    on_pushButton_start_clicked(false);
    QSound::play("qrc:/Sound/uvedomlenie11.wav");
    }


    if(!str.isEmpty())
    {
    str.append("\n\r");
    Laser_port->write(str);

  /////  qDebug()<<str;
    }

}



}

void MainWindow::on_pushButton_motion_clicked()
{

    win_motion = new DialogMotion(this);
    connect(win_motion, &DialogMotion::signal_send, this, &MainWindow::slot_send);
    win_motion->show();
}

void MainWindow::slot_send(QByteArray arr)
{



    if(arr == "$H" || arr == "$h" || arr == "\030")//\030 - reset
        flag_first = false;





    arr.append("\n\r");
    Laser_port->write(arr);




flag_OK = false;
//qDebug()<<"OK = " << flag_OK;

}

void MainWindow::run_Gcode(const QString &path)
{

     list_Gcode.clear();


if(path.isEmpty())
{
     MyMessage::warning(this, "Внимание", "Не выделена строка!");

     ui->pushButton_start->setChecked(false);
     on_pushButton_start_clicked(false);

                   return;
}

 QString dir = QString(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/Laser/");

QFile file(dir + "Gcoge/" + path + ".nc");
if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
               qDebug()<<"Load cnc File Problem";

 MyMessage::warning(this, "Внимание", "Нет файла cnc!\nНажмите кнопку Расстановка!");

 ui->pushButton_start->setChecked(false);
 on_pushButton_start_clicked(false);

               return;
       }



       QTextStream in(&file);

   while(!in.atEnd())
   {

   list_Gcode.append(in.readLine());

   }
   file.close();



    QByteArray arr = list_Gcode.at(count++).toLocal8Bit();
    arr.append("\n\r");

    Laser_port->write(arr);


}

void MainWindow::slot_select(const QString &str)
{
QList<MessData> xml_list = DialogEdit::parser_xml(str);

ui->listWidget->clear();



for (int i=0; i<xml_list.count(); i++)
{


        QListWidgetItem *item = new QListWidgetItem(xml_list[i].mess);

        QFont f = item->font();

        f.setPixelSize(80);
        item->setFont(f);
        item->setSizeHint(QSize(ui->listWidget->width(), 100));
        ui->listWidget->addItem(item);



}

Current_path = str;
}

bool MainWindow::eventFilter(QObject *target, QEvent *event)
{

      if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *keyEvent = (QMouseEvent *)event;
        if (keyEvent->button() ==  Qt::MiddleButton) {

            ui->pushButton_start->setChecked(!ui->pushButton_start->isChecked());
            on_pushButton_start_clicked(ui->pushButton_start->isChecked());


          return true;
        }
      }

    return QMainWindow::eventFilter(target, event);
}

void MainWindow::on_pushButton_start_clicked(bool checked)
{
    if(checked)
    {
         ui->label_wait->setText("РАБОТАЕТ ЛАЗЕР!");
ui->label_wait->setStyleSheet("QLabel{ background-color: rgb(255, 0, 0); color: white;}");



    ui->pushButton_start->setStyleSheet("QPushButton{ background-color: rgb(255, 0, 0); color: white;}");
ui->pushButton_start->setText("Стоп");


QFileInfo info(Current_path);
QString name = info.baseName();

 flag_Gcode = true;
    run_Gcode(name);


    }
    else
    {
         ui->label_wait->setText("ОЖИДАНИЕ");
         ui->label_wait->setStyleSheet("QLabel{ background-color: rgb(255, 255, 127); color: black;}");


    ui->pushButton_start->setStyleSheet("QPushButton{ background-color: rgb(0, 255, 0); color: black;}");
 ui->pushButton_start->setText("Старт");

 flag_Gcode = false;

    }




}
