#include "rectitem.h"


#include <QCursor>
//#include <QFile>
//#include <QDomDocument>
#include <QDebug>


RectItem::RectItem(QObject *parent)
{

}


QRectF RectItem::boundingRect() const
{
  return _rect;
}

void RectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    painter->setPen(QPen(QColor(50, 0, 0),2));
    painter->drawRect(_rect);
/*



if(flag_edit)
{

 painter->setPen(QPen(QColor(0, 0, 255),2));
 painter->drawRect(_rect);

}
else
{
    painter->setPen(QPen(QColor(50, 0, 0),2));
    painter->drawRect(_rect);
}






*/


           Q_UNUSED(option);
    Q_UNUSED(widget);
}

void RectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{


    if(!(event->buttons() & Qt::LeftButton)) {
               return;
           }

    QPointF new_pos = mapToScene(event->pos() - offset);
 ////////////////this->setPos(new_pos);

 /////////////  emit signal_drop(new_pos-old_pos) ;

    old_pos = new_pos;


}

void RectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{


///////////////////flag_edit = true;

             offset = event->pos();
             old_pos = this->pos();//?????????????????????
/////////// this->setCursor(QCursor(Qt::ClosedHandCursor));
////////////update();


//////////emit signal_click();

       Q_UNUSED(event);
}

void RectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   ///////////// this->setCursor(QCursor(Qt::ArrowCursor));

    Q_UNUSED(event);
}

QRectF RectItem::get_rect()
{

  return _rect;
}
