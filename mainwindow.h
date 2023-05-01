#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "dialogmenu.h"
#include "dialogmessage.h"
#include "dialogmotion.h"
#include "dialogedit.h"
#include "dialoggraphic.h"

#include <QSerialPortInfo>
#include <QSerialPort>
#include <QPointer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString &version, QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void on_pushButton_menu_clicked();
    void on_pushButton_message_clicked();
    void readData();
    void on_pushButton_motion_clicked();

    void on_pushButton_start_clicked(bool checked);

public slots:

     void slot_send(QByteArray arr);

     void run_Gcode(const QString &path);
     void slot_select(const QString &str);

protected:
   // virtual void mousePressEvent(QMouseEvent *event);
    bool eventFilter(QObject *target, QEvent *event) override;


private:
    Ui::MainWindow *ui;


bool flag_OK = false;
bool flag_first = false;


    QSerialPort *Laser_port = nullptr;

    QPointer <DialogMotion> win_motion;
   // QPointer <DialogGraphic> win_graphic;



    QString Current_path;
   QStringList list_Gcode;


    bool flag_Gcode = false;

    int count = 0;

};
#endif // MAINWINDOW_H
