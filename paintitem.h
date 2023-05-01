#ifndef PAINTITEM_H
#define PAINTITEM_H

#include <QGraphicsPathItem>
#include <QObject>

#include <QPainter>
#include <QGraphicsSceneMouseEvent>



struct FontData
{
 QString name;
 QRectF rect;
 QString figures;
 float width;
};


class PaintItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    PaintItem(const QString &data, QObject *parent = nullptr, int font_height = 6);




    QPainterPath Ppath;
    QString mess;

protected:

       QRectF boundingRect() const;
       void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
       void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
       void mousePressEvent(QGraphicsSceneMouseEvent *event);
       void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:
    /* Сигнал, который будет посылаться в том случае,
     * если произошел клик мышью по объекту
     * */
    void signal_drop(QPointF pos);
    void signal_click();


public slots:

    void draw_text(const QString &str);
    float draw_symbol(float left, float top, const QString &figure);
    QList<FontData> parser_font(const QString &str);
    void slot_reset_item();

    bool edit_enable(){return flag_edit;}
    void set_edit_enable(bool set){flag_edit = set; update();}



    void set_font_height(int set);
    int get_font_height(){return _font_height;}

    void set_num_line(int set){_num_line = set;}
    int get_num_line(){return _num_line;}

private:

bool flag_edit = false;


  QPointF offset;
  QPointF old_pos;

float _scale;

       QList<FontData> font_list;
       int _font_height;

        int _num_line;
};

#endif // PAINTITEM_H
