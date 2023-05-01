#ifndef DIALOGINPUT_H
#define DIALOGINPUT_H

#include <QDialog>

namespace Ui {
class DialogInput;
}

class DialogInput : public QDialog
{
    Q_OBJECT

public:
    explicit DialogInput(const QString &str, const QString &mess, QWidget *parent = nullptr);
    ~DialogInput();

private slots:


    void on_pushButton_OK_clicked();

    void on_pushButton_cancel_clicked();

signals:

  // void signal_show_win_graphic();
   void signal_OK_input(const QString &str, bool edit);

public slots:

    void slot_keyPressed(const QChar &t);
    void slot_returnPressed( );
    void slot_backspacePressed( );
    void slot_leftPressed();
    void slot_rightPressed();




private:
    Ui::DialogInput *ui;

int last_cursor_pos = 0;
    bool flag_edit = false;
};

#endif // DIALOGINPUT_H
