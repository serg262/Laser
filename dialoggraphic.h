#ifndef DIALOGGRAPHIC_H
#define DIALOGGRAPHIC_H

#include <QDialog>
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QFileDialog>
#include <QPainter>
#include <QWheelEvent>
#include <QtMath>
#include <QAbstractButton>

#include "dialogedit.h"
#include "paintitem.h"
/////////#include "rectitem.h"

//QPointF fonts_arr =

namespace Ui {
class DialogGraphic;
}


struct  Pix
{
 bool  G;
 float x;
 float y;
};

class DialogGraphic : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGraphic(const QString &path, QWidget *parent = nullptr);
    ~DialogGraphic();



QList<Pix> create_points();

const QStringList &create_Gcode();


private slots:
    void on_pushButton_cancel_clicked();



    void on_pushButton_OK_clicked();

    void draw_greed(int w, int h, int step);
    void create_icons(QPushButton *button, const QString &url);



    void on_pushButton_up_clicked();

    void zoomBy(qreal factor);
     qreal zoomFactor() const;

     float roundTo(double value, int precision);



     void on_pushButton_right_clicked();

     void on_pushButton_left_clicked();

     void on_pushButton_down_clicked();

     void on_pushButton_enable_all_clicked();


     void slot_drop(QPointF pos);
     void slot_click();

    // void on_pushButton_zoom_up_clicked();

   //  void on_pushButton_zoom_down_clicked();

    // void on_radioButton_tara1_clicked(bool checked);

    // void on_radioButton_tara2_clicked(bool checked);
     void group_clicked(QAbstractButton *button);

     void spinBox_font_height_valueChanged(int arg1);

protected:
    void wheelEvent(QWheelEvent *event) override;
    bool eventFilter(QObject *target, QEvent *event) override;

signals:
    void signal_send_Gcode(QStringList code);
    void signal_reset_item();
    void signal_coord_changed(QList<MessData> data);

private:
    Ui::DialogGraphic *ui;

    QGraphicsScene *scene;

    QString _path;


    QList <MessData> xml_list;
    bool flag_all = false;
    int tara;

};

#endif // DIALOGGRAPHIC_H
