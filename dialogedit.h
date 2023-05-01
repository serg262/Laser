#ifndef DIALOGEDIT_H
#define DIALOGEDIT_H

#include <QDialog>
#include <QDomDocument>




#define left_1 32
#define top_1 15
#define left_2 102
#define top_2 130

#define width_1 45
#define height_1 60
#define width_2 45
#define height_2 40




namespace Ui {
class DialogEdit;
}



struct MessData
{
 //QString name;
 int font_height = 0;
 int tara = 0;
 int NumLine = 0;
 QPointF point;
 QRectF rect;
 QString mess;
};



class DialogEdit : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEdit(const QString &path, bool create, QWidget *parent = nullptr);
    ~DialogEdit();



    static QList<MessData> parser_xml(const QString &path);
    static void create_xml(const QString &path, QList<MessData> xml_list);
  //  static void edit_xml(const QString &path, QList<MessData> xml_list);


private slots:



    void on_pushButton_cancel_clicked();


    void on_pushButton_edit_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_OK_clicked();

    void update_file();

    void on_pushButton_remove_clicked();

    void on_pushButton_up_clicked();

    void on_pushButton_down_clicked();

public slots:

    void slot_send_Gcode(QStringList code);


void slot_OK_input(const QString &str, bool edit);




signals:

     void signal_send_Gcode(QStringList code);
  // void signal_show_win_graphic();
   void signal_list_update(const QString &file);
   void signal_open_view(const QString &file);

private:
    Ui::DialogEdit *ui;

    QString currPath;


    bool _flag_edit = true;
    bool _flag_remove = false;
};

#endif // DIALOGEDIT_H
