#ifndef RECTITEM_H
#define RECTITEM_H

#include <QGraphicsPathItem>
#include <QObject>

#include <QPainter>
#include <QGraphicsSceneMouseEvent>


class RectItem : public QObject, public QGraphicsItem
{
     Q_OBJECT

public:
       RectItem(QObject *parent = nullptr);


        QRectF get_rect();

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
    void set_rect(QRectF rect){_rect = rect;}


private:

    bool flag_edit = false;
QRectF _rect;
    QPointF offset;
    QPointF old_pos;
};

#endif // RECTITEM_H
