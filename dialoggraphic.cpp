#include "dialoggraphic.h"
#include "ui_dialoggraphic.h"

#include "dialogedit.h"


#include <QChar>
#include <QTextCodec>
#include <QScrollBar>

#include <QDebug>
#include <QFontDatabase>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QStandardPaths>



DialogGraphic::DialogGraphic(const QString &path, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGraphic)
{
    ui->setupUi(this);

    connect(ui->buttonGroup, SIGNAL(buttonClicked(QAbstractButton *)),
            this, SLOT(group_clicked(QAbstractButton *)));

   // this->setModal(true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::Widget);

    connect(ui->spinBox_font_height, SIGNAL(valueChanged(int)),
                                        this, SLOT(spinBox_font_height_valueChanged(int)));




    create_icons(ui->pushButton_left, ":/Image/arrow-left.png");
    create_icons(ui->pushButton_right, ":/Image/arrow-right.png");
    create_icons(ui->pushButton_up, ":/Image/arrow-up.png");
    create_icons(ui->pushButton_down, ":/Image/arrow-down.png");


    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 180*5, 180*5);  //170x170  1:5

    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену в graphicsView
   // ui->graphicsView->setRenderHints(QPainter::Antialiasing);    // Устанавливаем сглаживание
  /////// ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключаем скроллбар по вертикали
 ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключаем скроллбар по горизонтали

  //  ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
   // ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
   /////// ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  //  ui->graphicsView->setMouseTracking(true);


   ui->graphicsView->installEventFilter(this);

  draw_greed(180*5, 180*5, 5*5);//5mm

 //масштаб 1:5


 // ui->graphicsView->verticalScrollBar()->setStyleSheet(""
//   " QScrollBar:vertical {width: 40px;}"
 ////  " QScrollBar::handle {background: gray; border-radius: 10px;}"
//   " QScrollBar::handle:vertical {height: 40px;}");




_path = path;


  xml_list = DialogEdit::parser_xml(path);


 // RectItem *rect = new RectItem(this);
  QGraphicsRectItem *rect = new QGraphicsRectItem();

int count=0;

  for(const MessData &mess: qAsConst(xml_list))
  {



float w = mess.rect.width();
if(!w){w=width_2;}
float h = mess.rect.height();
if(!h){h=height_2;}
float x = mess.rect.left();
if(!x)
{x=left_2;}
float y = mess.rect.top();
if(!y)
{y=top_2;}

tara = mess.tara;

if(!tara){tara = 2;}//крышка по умолчанию

if(tara==1)
ui->radioButton_tara1->setChecked(true);
else
    if(tara==2)
    ui->radioButton_tara2->setChecked(true);





if(!count)
{
   // rect->set_rect(QRectF(0, 0, w * 5.f, h * 5.f));
    rect->setRect(QRectF(0, 0, w * 5.f, h * 5.f));
      scene->addItem(rect);

       rect->setPos(QPointF(x*5, y*5));
}








      PaintItem *mItem = new PaintItem(mess.mess, this, mess.font_height);//размер шрифта
      connect(this, &DialogGraphic::signal_reset_item, mItem, &PaintItem::slot_reset_item);
      connect(mItem, &PaintItem::signal_drop, this, &DialogGraphic::slot_drop);
      connect(mItem, &PaintItem::signal_click, this, &DialogGraphic::slot_click);
      mItem->setPos(mess.point.x()*5.f + x*5,//????????????
                    mess.point.y()*5.f + y*5);//?????????

mItem->set_num_line(mess.NumLine);

      scene->addItem(mItem);

count++;

       }


  if(tara == 1)
  {
      QTimer::singleShot(10,[=](){ QScrollBar *sb = ui->graphicsView->verticalScrollBar();
          sb->setValue(sb->minimum());});

  }
  else
      if(tara == 2)
      {

          QScrollBar *sb = ui->graphicsView->verticalScrollBar();
                        sb->setValue(sb->maximum());

      }

}

DialogGraphic::~DialogGraphic()
{
    delete ui;
}


QList<Pix> DialogGraphic::create_points()
{






QList<Pix> points;









   // points.clear();


 //   double coeff = ui->graphicsView->width() / 170.0;


QString command;
Pix d;






    //QPointF startPoint;
   // QPointF endPoint;
    float x, y;





    QList <QGraphicsItem *>items = scene->items();

          for(QGraphicsItem *item :  qAsConst(items))
          {
          PaintItem *pitem = dynamic_cast<PaintItem *>(item);

          if(pitem)
          {


          float x_offset = pitem->pos().x()/5.f;
          float y_offset = pitem->pos().y()/5.f;



          for(int i=0; i < pitem->Ppath.elementCount(); i++)
              {
                  QPainterPath::Element e = pitem->Ppath.elementAt(i);








//масштаб???????????????????????????????????????????????????????????
                  x = roundTo(e.x/5.f, 2);
                  y = 170.f - roundTo(e.y/5.f, 2);//120 = 170 -_offset



                  d.x = x + x_offset;
                  d.y = y - y_offset;

                  switch (e.type)
                  {
                  case QPainterPath::MoveToElement:

                      d.G = false;

                       points.append(d);//точки




                      break;

                  case QPainterPath::LineToElement:

                       d.G = true;

                        points.append(d);//точки


                       break;

                  case QPainterPath::CurveToElement:

                      d.G = true;

                       points.append(d);//точки

                      break;



                  case QPainterPath::CurveToDataElement:

                      d.G = true;

                       points.append(d);//точки

                      break;

                  }








}


              }
          }






          return points;
}

const QStringList &DialogGraphic::create_Gcode()
{

 QList<Pix> points = create_points();



    QStringList Gcode;

    QFileInfo info(_path);
QString name = info.baseName();

QString dir = QString(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/Laser/");

      QString fileName = dir +  "Gcoge/" + name + ".nc";// = QFileDialog::getSaveFileName(this, tr("Save GCode"),
                                //  "",
                                //  tr("GCode(*.gc *.nc *.txt)"));



          QFile file(fileName);
          if (file.open(QIODevice::WriteOnly | QIODevice::Text) )
          {
              QTextStream stream( &file );
              stream << "; " + _path << Qt::endl;
              stream << "G00 G17 G40 G21 G54" << Qt::endl;
              stream << "G90" << Qt::endl;
              stream << "M4" << Qt::endl;
              stream << "M8" << Qt::endl;



               Gcode <<  "G00 G17 G40 G21 G54";
                Gcode <<  "G90";
                 Gcode <<  "M4";
                  Gcode <<  "M8";

              //алгоритм
       stream << "; Layer C00 begin" << Qt::endl;

       stream << QString("S%0").arg(QString::number(50*10)) << Qt::endl;
       stream << QString("F%0").arg(QString::number(5000)) << Qt::endl;


       Gcode << QString("S%0").arg(QString::number(50*10));
       Gcode << QString("F%0").arg(QString::number(5000));

   for(int i=0; i<points.count(); i++)
   {

       if(points[i].G == false)
       {
       stream << QString("G0 X%0 Y%1").arg(QString::number(points[i].x, 'f', 2)).
                                       arg(QString::number(points[i].y, 'f', 2)) << Qt::endl;

       Gcode << QString("G0 X%0 Y%1").arg(QString::number(points[i].x, 'f', 2)).
                                       arg(QString::number(points[i].y, 'f', 2));
       }
       else
       {
           stream << QString("G1 X%0 Y%1").arg(QString::number(points[i].x, 'f', 2)).
                                           arg(QString::number(points[i].y, 'f', 2)) << Qt::endl;

           Gcode << QString("G1 X%0 Y%1").arg(QString::number(points[i].x, 'f', 2)).
                                          arg(QString::number(points[i].y, 'f', 2));
       }

  }




       stream << "; Layer C00 end" << Qt::endl;
              //алгоритм


   stream << "M9" << Qt::endl;
    stream << "G1S0" << Qt::endl;
     stream << "M5" << Qt::endl;
      stream << "G90" << Qt::endl;


      Gcode << "M9";
       Gcode << "G1S0";
        Gcode << "M5";
         Gcode << "G90";

    //  return to user-defined finish pos

       stream << "G0 X0 Y170" << Qt::endl;
        Gcode << "G0 X0 Y170";


       stream << "M2" << Qt::endl;

       Gcode << "M2";


              file.close();
          }


return Gcode;
}


void DialogGraphic::on_pushButton_cancel_clicked()
{
    this->hide();
}


void DialogGraphic::on_pushButton_OK_clicked()
{
//передать новые координаты
    QList<MessData> data_list;



    QList <QGraphicsItem *>items = scene->items();


QGraphicsRectItem *ritem;
 QRectF rect;
int tara = ui->radioButton_tara1->isChecked() ? 1 : 2;


    for(QGraphicsItem *item :  qAsConst(items))//ищу прямоугольник
    {
    //ritem = dynamic_cast<RectItem *>(item);
    ritem = dynamic_cast<QGraphicsRectItem *>(item);

     if(ritem)
     {
rect = ritem->rect();
//rect.setLeft(ritem->pos().x()/5.f);
//rect.setTop(ritem->pos().y()/5.f);

break;
   }
  }




int count = 0;


int num_item = 0;


for(QGraphicsItem *item :  qAsConst(items))//ищу все строки
{
PaintItem *pitem = dynamic_cast<PaintItem *>(item);


 if(pitem)
  num_item++;
}

while(count != num_item)
{

          for(QGraphicsItem *item :  qAsConst(items))
          {
          PaintItem *pitem = dynamic_cast<PaintItem *>(item);


           if(pitem)
           {
     MessData data;//в миллиметрах

data.NumLine = pitem->get_num_line();

if(!pitem->mess.isEmpty() && data.NumLine == count)
{



    data.point.setX(pitem->pos().x()/5.f - ritem->pos().x()/5.f);//????????????
    data.point.setY(pitem->pos().y()/5.f - ritem->pos().y()/5.f);
               data.mess = pitem->mess;
               data.font_height = pitem->get_font_height();

               data.rect.setLeft(ritem->pos().x()/5.f);
                data.rect.setTop(ritem->pos().y()/5.f);
                 data.rect.setWidth(rect.width()/5.f);
                  data.rect.setHeight(rect.height()/5.f);




    data.tara = tara;


             data_list.append(data);
             count++;
}
           }






          }
}

 create_Gcode();


emit signal_coord_changed(data_list);


   this->close();
}



void DialogGraphic::draw_greed(int w, int h, int step)
{

    //вертикальные
    for(int col = 0; col<=w; col += step)
    scene->addLine(col,0,col,h, QPen(QColor(Qt::gray),1));

    //горизонтальные
    for(int row = 0; row<=h; row += step)
    scene->addLine(0,row,w,row, QPen(QColor(Qt::gray),1));

}

void DialogGraphic::create_icons(QPushButton *button, const QString &url)
{
    QDir folder(QString(QDir::currentPath() + "/image"));//если нет папки, то создаём
    if (!folder.exists())
        folder.mkpath(".");

    //создание пути к файлам
    QString dir(url);
    dir.remove(":");
    dir.remove(".png");
    QString path_pressed(QDir::currentPath() + dir + "_pressed.png");
    QString path_disabled(QDir::currentPath() + dir + "_disabled.png");


    //проверка на существование файлов
if(!QFile::exists(path_pressed) &&  !QFile::exists(path_disabled))
{

QPixmap pixmap(url);

//pixmap = pixmap.scaledToWidth(ui->pushButton_up->width(), Qt::SmoothTransformation);
qint32 width = pixmap.width();
qint32 height = pixmap.height();


QImage image = pixmap.toImage();

QColor color;

        /*  HSV цветовая модель
        H определяет оттенок
        S определяет насыщенность
        V определяет яркость
        */

for (int x = 0; x < width; x++)//pressed
{
    for (int y = 0; y < height; y++)
    {
        color.setRgba(image.pixel(x, y));

        if(color.alpha()>0)
        color.setHsv(color.hsvHue(), 50, color.lightness(), color.alpha());
        else
        color.setHsv(color.hsvHue(), color.hsvSaturation(), color.lightness(), color.alpha());

        image.setPixelColor(x, y, color);
    }
}

image.save(path_pressed);



image = pixmap.toImage();

for (int x = 0; x < width; x++)//disabled
{
    for (int y = 0; y < height; y++)
    {
        color.setRgba(image.pixel(x, y));

        if(color.alpha()>0)
        color.setHsv(color.hsvHue(), color.hsvSaturation(), 200, color.alpha());
        else
        color.setHsv(color.hsvHue(), color.hsvSaturation(), color.lightness(), color.alpha());

        image.setPixelColor(x, y, color);
    }
}

image.save(path_disabled);

}

 button->setStyleSheet(QString("QPushButton {border-image : url(%0); border: none;}"
                               "QPushButton:disabled {border-image : url(%1);}"
 "QPushButton:pressed {border-image : url(%2);}").arg(url).arg(path_disabled).arg(path_pressed));

}




void DialogGraphic::on_pushButton_up_clicked()
{
    QList <QGraphicsItem *>items = scene->items();

          for(QGraphicsItem *item :  qAsConst(items))
          {
          PaintItem *pitem = dynamic_cast<PaintItem *>(item);

           if(pitem && pitem->edit_enable())
               pitem->setPos(item->pos().x(), item->pos().y()-2);

          }
}

void DialogGraphic::on_pushButton_right_clicked()
{

  QList <QGraphicsItem *>items = scene->items();

        for(QGraphicsItem *item :  qAsConst(items))
        {
        PaintItem *pitem = dynamic_cast<PaintItem *>(item);

         if(pitem && pitem->edit_enable())
             pitem->setPos(item->pos().x() +2, item->pos().y());

        }
}

void DialogGraphic::on_pushButton_left_clicked()
{
    QList <QGraphicsItem *>items = scene->items();

          for(QGraphicsItem *item :  qAsConst(items))
          {
          PaintItem *pitem = dynamic_cast<PaintItem *>(item);

           if(pitem && pitem->edit_enable())
               pitem->setPos(item->pos().x() -2, item->pos().y());

          }
}

void DialogGraphic::on_pushButton_down_clicked()
{
    QList <QGraphicsItem *>items = scene->items();

          for(QGraphicsItem *item :  qAsConst(items))
          {
          PaintItem *pitem = dynamic_cast<PaintItem *>(item);

           if(pitem && pitem->edit_enable())
               pitem->setPos(item->pos().x(), item->pos().y()+2);

          }
}




void DialogGraphic::wheelEvent(QWheelEvent *event)
{
    ///////// zoomBy(qPow(1.2, event->angleDelta().y() / 240.0));
}



void DialogGraphic::zoomBy(qreal factor)
{
    const qreal currentZoom = zoomFactor();
    if ((factor < 1 && currentZoom < 0.1) || (factor > 1 && currentZoom > 10))
        return;
    ui->graphicsView->scale(factor, factor);
   // emit zoomChanged();
}

qreal DialogGraphic::zoomFactor() const
{
    return ui->graphicsView->transform().m11();
}



float DialogGraphic::roundTo(double value, int precision)
{
    int mul = 1;

    for (int i = 0; i < precision; i++)
    mul *= 10;
    if (value > 0.0)
    return (float)(floor(value * mul + .5) / mul);
    else
        return (float)(ceil(value * mul - .5) / mul);
}





bool DialogGraphic::eventFilter(QObject *target, QEvent *event)
{
  if (target == ui->graphicsView) {
    if (event->type() == QEvent::MouseButtonPress) {
      QMouseEvent *keyEvent = (QMouseEvent *)event;
      if (keyEvent->button() ==  Qt::LeftButton) {


          emit signal_reset_item();
          flag_all = false;

        return true;
      }
    }
  }
  return QDialog::eventFilter(target, event);
}

void DialogGraphic::on_pushButton_enable_all_clicked()
{
    QList <QGraphicsItem *>items = scene->items();

          for(QGraphicsItem *item :  qAsConst(items))
          {
          PaintItem *pitem = dynamic_cast<PaintItem *>(item);

           if(pitem)
               pitem->set_edit_enable(true);

          }


          flag_all = true;
}

void DialogGraphic::slot_drop(QPointF pos)
{
    PaintItem *drop_item = qobject_cast<PaintItem *>(sender());

    if(drop_item)
    {
    drop_item->setPos(drop_item->pos() + pos);
    //ui->graphicsView->repaint();
    }

    /*
    QList <QGraphicsItem *>items = scene->items();

          for(QGraphicsItem *item :  qAsConst(items))
          {
          PaintItem *pitem = dynamic_cast<PaintItem *>(item);

           if(pitem && pitem->edit_enable() && drop_item != pitem)
           {
               pitem->setPos(pitem->pos() + pos);
               ui->graphicsView->repaint();
           }

          }
          */
}

void DialogGraphic::slot_click()//?????????????????
{

  if(flag_all)
      return;


    PaintItem *drop_item = qobject_cast<PaintItem *>(sender());


    QList <QGraphicsItem *>items = scene->items();

          for(QGraphicsItem *item :  qAsConst(items))
          {
          PaintItem *pitem = dynamic_cast<PaintItem *>(item);

           if(pitem && pitem->edit_enable() && drop_item != pitem)
           {
               pitem->set_edit_enable(false);

           }

           if(pitem && pitem->edit_enable())
           {
               disconnect(ui->spinBox_font_height, SIGNAL(valueChanged(int)),
                                                   this, SLOT(spinBox_font_height_valueChanged(int)));

           ui->spinBox_font_height->setValue(pitem->get_font_height());


           connect(ui->spinBox_font_height, SIGNAL(valueChanged(int)),
                                               this, SLOT(spinBox_font_height_valueChanged(int)));

           }

          }
}
/*
void DialogGraphic::on_pushButton_zoom_up_clicked()
{
 zoomBy(qPow(1.2, 50 / 240.0));
}

void DialogGraphic::on_pushButton_zoom_down_clicked()
{
 zoomBy(qPow(1.2, -50 / 240.0));
}
*/




void DialogGraphic::group_clicked(QAbstractButton *button)
{




    if(button == ui->radioButton_tara1 && tara == 2)
    {

tara = 1;

        QList <QGraphicsItem *>items = scene->items();

              for(QGraphicsItem *item :  qAsConst(items))
              {
             // RectItem *ritem = dynamic_cast<RectItem *>(item);
QGraphicsRectItem *ritem = dynamic_cast<QGraphicsRectItem *>(item);
               if(ritem)
               {
                  // pitem->set_edit_enable(true);

                   ritem->setRect(QRectF(0, 0, width_1 * 5.f, height_1 * 5.f));

                   ritem->setPos(QPointF(left_1*5.f, top_1*5.f));
               }


               PaintItem *pitem = dynamic_cast<PaintItem *>(item);

                if(pitem)
                {
                  pitem->setPos(item->pos().x()-(left_2-left_1)*5, item->pos().y()-(top_2-top_1)*5);

                }



              }


    ui->graphicsView->verticalScrollBar()->setValue(ui->graphicsView->verticalScrollBar()->minimum());
    //ui->graphicsView->verticalScrollBar()->update();

    }
    else
    if(button == ui->radioButton_tara2 && tara == 1)
    {

        tara = 2;
             QList <QGraphicsItem *>items = scene->items();

                   for(QGraphicsItem *item :  qAsConst(items))
                   {
                  QGraphicsRectItem *ritem = dynamic_cast<QGraphicsRectItem *>(item);

                    if(ritem)
                    {
                       // pitem->set_edit_enable(true);

                         ritem->setRect(QRectF(0, 0, width_2 * 5.f, height_2 * 5.f));

                   ritem->setPos(QPointF(left_2*5.f,top_2*5.f));
                    }



                    PaintItem *pitem = dynamic_cast<PaintItem *>(item);

                     if(pitem)
                     {
                         pitem->setPos(item->pos().x()+(left_2-left_1)*5, item->pos().y()+(top_2-top_1)*5);

                     }


                   }

                 ui->graphicsView->verticalScrollBar()->setValue(ui->graphicsView->verticalScrollBar()->maximum());
                 //  ui->graphicsView->update();

    }
}



void DialogGraphic::spinBox_font_height_valueChanged(int arg1)
{
 // QSpinBox *box = qobject_cast<QSpinBox *>(sender());


    QList <QGraphicsItem *>items = scene->items();

          for(QGraphicsItem *item :  qAsConst(items))
          {


           PaintItem *pitem = dynamic_cast<PaintItem *>(item);

            if(pitem && pitem->edit_enable())
            {
//if(pitem == last_pitem)
//{


               // if(box)
                pitem->set_font_height(arg1);

                scene->update();
//}
            }


          }
}
