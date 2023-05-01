#ifndef DIALOGMENU_H
#define DIALOGMENU_H

#include <QDialog>

namespace Ui {
class DialogMenu;
}

class DialogMenu : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMenu(QWidget *parent = nullptr);
    ~DialogMenu();



public slots:



private slots:


    void on_pushButton_cancel_clicked();

    void on_pushButton_exit_clicked();

    void on_pushButton_reboot_clicked();

private:
    Ui::DialogMenu *ui;
};

#endif // DIALOGMENU_H
