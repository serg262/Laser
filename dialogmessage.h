#ifndef DIALOGMESSAGE_H
#define DIALOGMESSAGE_H

#include <QDialog>
 #include <QGraphicsScene>
 #include <QFileInfoList>

#include "dialogedit.h"

namespace Ui {
class DialogMessage;
}

class DialogMessage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMessage(QWidget *parent = nullptr);
    ~DialogMessage();

private slots:



    void on_pushButton_create_clicked();

    void on_pushButton_change_clicked();

    void find_files();
    void parser_file(const QString &str);

    void on_listWidget_currentTextChanged(const QString &currentText);



    void on_pushButton_view_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_select_clicked();

    void on_pushButton_cancel_clicked();

public slots:


    void slot_send_Gcode(QStringList code);
    //void slot_show_win_graphic();
    void slot_show_win_edit();

    void slot_OK_input(const QString &str);
    void slot_list_update(const QString &file);
    void slot_open_view(const QString &file);

    void slot_coord_changed(QList<MessData> data);



signals:


    void signal_send_Gcode(QStringList code);
    void signal_select(const QString &str);


private:
    Ui::DialogMessage *ui;

    QGraphicsScene *scene;  // Графическая сцена
    //  QString path;           // Путь сохранения файла


      QFileInfoList fileInfoList;
      QString currPath;

     // int last_row = 0;
};

#endif // DIALOGMESSAGE_H
