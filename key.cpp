#include <QDebug>
#include "key.h"


key::key(QString t, QSize size, QObject *parent) : QObject(parent)
{
  text = t;
  X =10;
  Y =10;





 // W = t.length()*5 + 50;
  W = size.width();
  H = size.height();

  if(t == " ")
      W *= 10.5;

  if(t == "Shift")
      W *= 2.0;

  if(t == "Caps" || t == "Enter")
       W *=1.5;

  if(t == "RUS" || t == "ENG")
      W *=1.5;



  pressed = false;
}

QRect key::getRect()
{
    return QRect(X,Y,W,H);
}

void key::setX(int x )
{
  //  qDebug() << "setX " << x << " " << text;
  X = x;
}


void key::setY(int y )
{
  //  qDebug() << "setY " << y;
  Y = y;
}



void key::setPressed( bool b)
{
//   qDebug() << "setPessed " << b;
   pressed = b;
}

void key::draw(QPainter *p,QStyle *style) {

    QStyleOptionButton opt;
    QFont font = p->font();
    font.setPixelSize(24);

    p->setFont(font);
    opt.palette = QPalette(Qt::gray);

    //if ( pressed )
     //      {
     //          opt.state = QStyle::State_Active;
     //      } else {
      //          opt.state = QStyle::State_Enabled;
     //      }

   // if (pressed)
   //    opt.state = QStyle::State_Enabled | QStyle::State_On;
  //  else
   //    opt.state = QStyle::State_Enabled | QStyle::State_Off;

    opt.rect = QRect(X, Y, W, H);

   // if ( iconFilename !="" )
    //{
    //    opt.icon = QIcon(iconFilename);
    //    opt.iconSize=QSize(16,16);
   // }
   // else
   // {
   //     if (text =="&")
     //       opt.text = "&&";
     //   else
            opt.text = text;
   // }
    style->drawControl(QStyle::CE_PushButton, &opt, p);

}
