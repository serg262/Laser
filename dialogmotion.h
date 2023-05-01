#ifndef DIALOGMOTION_H
#define DIALOGMOTION_H

#include <QDialog>

namespace Ui {
class DialogMotion;
}

class DialogMotion : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMotion(QWidget *parent = nullptr);
    ~DialogMotion();

  float X_pos = 0.f, Y_pos = 0.f;

signals:
    void signal_send(QByteArray arr);




private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_left_clicked();

    void on_pushButton_right_clicked();

    void on_pushButton_up_clicked();

    void on_pushButton_down_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_pos_clicked();

    void on_pushButton_unlook_clicked();

    void on_pushButton_OK_clicked();

public slots:
    void set_text(const QString &str);

private:
    Ui::DialogMotion *ui;




};

#endif // DIALOGMOTION_H
