#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QString>



class key;

class Keyboard : public QWidget
{
    Q_OBJECT

public:
    Keyboard(QWidget *p);

    signals:
    void keyPressed(const QChar &t);
    void backspacePressed( );
    void returnPressed();
    void leftPressed();
    void rightPressed();


protected:


      void paintEvent(QPaintEvent *event);
      void mousePressEvent(QMouseEvent *event);
     // void mouseMoveEvent(QMouseEvent * e);
      void mouseReleaseEvent(QMouseEvent *event);
private :


    void initTooltip();
    void initKeys( int indexArraykeys, const char *keymap[]);
    key *findKey(QPoint p);
    void setKeyPressed( key *k,QPoint );

    void change_register();

    QVector<QVector< key * > > keys;
    QLabel *tooltip;
   // key *currentKey;
    int currentindexkeyboard;
    bool uppercase  = false;
    bool flag_shift = false;
  //  bool flag_caps = false;


};

#endif // KEYBOARD_H
