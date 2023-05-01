#ifndef MYMESSAGE_H
#define MYMESSAGE_H


#include <QDialog>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>


//#include "workwindow.h"




class MyMessage : public QDialog
{
    Q_OBJECT

public:
    explicit MyMessage(QWidget *parent = nullptr);
    ~MyMessage() override;


static void warning(QWidget *parent, const QString &title,const QString &str);


    void set_text(const QString &str);



//protected:
    // bool eventFilter(QObject *obj, QEvent *event) override;

private slots:

signals:

   //  void signal_pressed(bool set);

private:


QLabel *label;
QPushButton *yesButton;
QPushButton *noButton;




};




#endif // MYMESSAGE_H
