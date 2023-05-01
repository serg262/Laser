#include "mainwindow.h"

#include <QApplication>


#define DEMO 0
QString VERSION = "1.03";

int main(int argc, char *argv[])
{

  //  qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QApplication a(argc, argv);

if(!DEMO)
a.setOverrideCursor(Qt::BlankCursor);

    MainWindow w(VERSION);

if(!DEMO)
   w.showFullScreen();
else
   w.show();


    return a.exec();
}
