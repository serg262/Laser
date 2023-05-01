#include "paintitem.h"


#include <QCursor>
#include <QFile>
#include <QDomDocument>
#include <QDebug>
#include <QStandardPaths>




PaintItem::PaintItem(const QString &data, QObject *parent, int font_height)
    : QObject(parent), QGraphicsItem()
{

       //setFlag(QGraphicsItem::ItemIsMovable, true);
      // setFlag(QGraphicsItem::ItemIsSelectable, true);

_font_height = font_height;

_scale = font_height/4.f;


        QString dir = QString(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/Laser/");

 font_list = parser_font(dir + "FontStroke/Гост.fsc");

    draw_text(data);

mess = data;
}

void PaintItem::draw_text(const QString &str)
{

    float x = 0.f;
    float y = 0.f;
float margin = _scale * 5.f;//????????



if(font_list.count() > 0)
{

    for(QChar c: qAsConst(str))
    {
     int num = (int)c.unicode();
     if(num>127)
         num-=848;



  float p_width =  draw_symbol(x, y, font_list.at(num).figures);

    x += (p_width + margin);//?????

//x+=ширина символа

    }
}
else
    qDebug()<<"Нет файла шрифта!";

}

float PaintItem::draw_symbol(float left, float top, const QString &figure)
{

 float x_max = 0.f;

    if(!figure.isEmpty())
    {


        QStringList list = figure.split(" ");






for(int j=0; j<list.size();j++)
{

   QString str = list.at(j);

QStringList list2;

int last = str.indexOf('L');
QString ffh = str.mid(1,last-1);
list2 = ffh.split(",");

float x = list2.at(0).toFloat()*_scale;
float y = list2.at(1).toFloat()*_scale;

x_max = qMax(x_max, x);


Ppath.moveTo(QPointF(x+left, y+top));

str.remove(0,last+1);
last = str.indexOf('L');


       while(last!=-1)
       {
       QString ffh22 = str.mid(0, last-1);


        list2 = ffh22.split(",");

        float x2 = list2.at(0).toFloat()*_scale;
        float y2 = list2.at(1).toFloat()*_scale;


        x_max = qMax(x_max, x2);

        Ppath.lineTo(QPointF(x2+left, y2+top));


        str.remove(0,last+1);
        last = str.indexOf('L');

       }

       QString ffh22 = str.mid(0, last-1);


        list2 = ffh22.split(",");

        float x2 = list2.at(0).toFloat()*_scale;
        float y2 = list2.at(1).toFloat()*_scale;

        Ppath.lineTo(QPointF(x2+left, y2+top));

x_max = qMax(x_max, x2);



}




    }
return x_max;
}

QList<FontData> PaintItem::parser_font(const QString &str)
{


    QList< FontData>  xml_list;

  QFile file(str);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                 qDebug()<<"Load XML File Problem";
return xml_list;
         }
  else
  {

      QDomDocument doc("mydocument");
        if (!doc.setContent(&file))
        {
         return QList<FontData>();
        }

        //Get the root element
        QDomElement docElem = doc.documentElement();

  QString rootTag = docElem.tagName();
  QDomNodeList nodeList = docElem.elementsByTagName("Canvas");

  for(int ii = 0;ii < nodeList.count(); ii++)

    {
  FontData data;


     QDomElement el = nodeList.at(ii).toElement();

     QString name = el.attribute("Name");
    QString  Left = el.attribute("Canvas.Left");
    QString  Top = el.attribute("Canvas.Top");
    QString  Right = el.attribute("Canvas.Right");
    QString  Bottom = el.attribute("Canvas.Bottom");

data.name = name;
data.rect.setLeft(Left.toFloat());  //???????????????
data.rect.setTop(Top.toFloat());//???????????????
data.rect.setWidth(Right.toFloat()-Left.toFloat());//???????????????
data.rect.setHeight(Bottom.toFloat() - Top.toFloat());//???????????????

    QDomNode n = el.firstChild();

        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {


            QDomNode nddd = e.firstChild();
            QDomElement eeee = nddd.toElement();


            QDomNode nddd22 = eeee.firstChild();
            QDomElement eeee22 = nddd22.toElement();




 data.figures = eeee22.attribute("Figures");



  }

   xml_list.append(data);

  }

         file.close();

  }

  return  xml_list;
}

void PaintItem::slot_reset_item()
{
    flag_edit = false;
    update();
}

void PaintItem::set_font_height(int set)
{
     Ppath.clear();



    _font_height = set;

    _scale = _font_height/4.f;




        draw_text(mess);



    update();
}

QRectF PaintItem::boundingRect() const
{
  return QRectF(Ppath.boundingRect());
}

void PaintItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{




if(flag_edit)
{

 painter->setPen(QPen(QColor(0, 0, 255),2));
 painter->drawPath(Ppath);

}
else
{
    painter->setPen(QPen(QColor(50, 0, 0),2));
    painter->drawPath(Ppath);
}









           Q_UNUSED(option);
    Q_UNUSED(widget);
}

void PaintItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{


    if(!(event->buttons() & Qt::LeftButton)) {
               return;
           }

    QPointF new_pos = mapToScene(event->pos() - offset);
// this->setPos(new_pos);

  emit signal_drop(new_pos-old_pos) ;
//update();
    old_pos = new_pos;


}

void PaintItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{


flag_edit = true;

             offset = event->pos();
             old_pos = this->pos();//?????????????????????
 //this->setCursor(QCursor(Qt::ClosedHandCursor));
/////


emit signal_click();
update();
       Q_UNUSED(event);
}

void PaintItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   /////// this->setCursor(QCursor(Qt::ArrowCursor));

    Q_UNUSED(event);
}
