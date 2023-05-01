#include "dialogmessage.h"
#include "ui_dialogmessage.h"


#include "dialoginput.h"
#include "dialogedit.h"
#include "dialoggraphic.h"

#include "mymessage.h"

#include <QFileDialog>

#include <QPainter>
#include <QGraphicsRectItem>

#include <QDebug>
#include <QScrollBar>
#include <QStandardPaths>

DialogMessage::DialogMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMessage)
{
    ui->setupUi(this);

   // this->setModal(true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::Widget);



   // ui->listWidget->verticalScrollBar()->setStyleSheet(""
    // " QScrollBar:vertical {width: 40px;}"
    // " QScrollBar::handle {background: gray; border-radius: 10px;}"
   //  " QScrollBar::handle:horizontal {height: 40px;}"
   //  " QScrollBar::handle:vertical {height: 40px;}");




    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());

    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену в graphicsView
   // ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);    // Устанавливаем сглаживание
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключаем скроллбар по вертикали
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Отключаем скроллбар по горизонтали





find_files();

ui->listWidget->setCurrentRow(0);

}

DialogMessage::~DialogMessage()
{
    delete ui;
}


void DialogMessage::on_pushButton_create_clicked()
{


    DialogInput *win_input = new DialogInput("", "Введите имя сообщения", this);
    connect(win_input, &DialogInput::signal_OK_input, this, &DialogMessage::slot_OK_input);
    win_input->setFixedSize(this->width(), this->height());
    win_input->move(0,0);
    win_input->show();


}

void DialogMessage::on_pushButton_change_clicked()
{
    QListWidgetItem *item = ui->listWidget->currentItem();
    if(item)
    {
    DialogEdit *win_edit = new DialogEdit(currPath, false, this);
    connect (win_edit, &DialogEdit::signal_send_Gcode, this, &DialogMessage::slot_send_Gcode);
    connect(win_edit, &DialogEdit::signal_list_update, this, &DialogMessage::slot_list_update);
     connect(win_edit, &DialogEdit::signal_open_view, this, &DialogMessage::slot_open_view);
    win_edit->setFixedSize(this->width(), this->height());
    win_edit->move(0,0);
    win_edit->show();
    }
    else
    {
       // QMessageBox::warning(this, "Внимание","Не выделена строка");
        MyMessage::warning(this, "Внимание","Не выделена строка");
    }
}

void DialogMessage::find_files()
{

   QString path = QString(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/Laser/");
   QDir dir(path + "Layout/");
   dir.setNameFilters(QStringList("*.ltc"));
       dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);




        ui->listWidget->clear();

          fileInfoList.clear();

          fileInfoList = dir.entryInfoList();

       for (int i=0; i<fileInfoList.count(); i++)
       {


               QListWidgetItem *item = new QListWidgetItem(fileInfoList[i].baseName());

               QFont f = item->font();

               f.setPixelSize(25);
               item->setFont(f);
               item->setSizeHint(QSize(ui->listWidget->width(), 70));
               ui->listWidget->addItem(item);



       }






}

void DialogMessage::parser_file(const QString &str)
{


QList<MessData> xml_list = DialogEdit::parser_xml(str);

scene->clear();

  for(const MessData &data: qAsConst(xml_list))
  {

  QGraphicsTextItem * io = new QGraphicsTextItem;
QFont f =io->font();
f.setPixelSize(20);

io->setFont(f);


  io->setPos(data.point.x()*5.f, data.point.y()*5.f);
  io->setPlainText(data.mess);

  scene->addItem(io);

  }

}

void DialogMessage::on_listWidget_currentTextChanged(const QString &currentText)
{




for(const QFileInfo &info : qAsConst(fileInfoList))
{

 if(info.baseName() == currentText)
 {
 currPath = info.absoluteFilePath();

 parser_file(currPath);
//last_row = ui->listWidget->currentRow();
//ui->listWidget->setCurrentRow(last_row);



 break;
 }


}

emit signal_select(currPath);
}

void DialogMessage::slot_send_Gcode(QStringList code)
{
    emit signal_send_Gcode(code);
}


void DialogMessage::slot_show_win_edit()
{

}

void DialogMessage::slot_OK_input(const QString &str)
{

      DialogEdit *win_edit = new DialogEdit(str, true, this);
      connect (win_edit, &DialogEdit::signal_send_Gcode, this, &DialogMessage::slot_send_Gcode);
      connect(win_edit, &DialogEdit::signal_list_update, this, &DialogMessage::slot_list_update);
      connect(win_edit, &DialogEdit::signal_open_view, this, &DialogMessage::slot_open_view);
      win_edit->setFixedSize(this->width(), this->height());
      win_edit->move(0,0);
      win_edit->show();

}

void DialogMessage::slot_list_update(const QString &file)
{
    find_files();
   for(int row = 0; row< ui->listWidget->count(); row++)
   {
       QFileInfo info(file);
   if(ui->listWidget->item(row)->text() == info.baseName())
   {
    ui->listWidget->setCurrentRow(row);
    break;


   }


   }


}

void DialogMessage::slot_open_view(const QString &file)
{
    DialogGraphic *win_graphic = new DialogGraphic(file, this);
    connect(win_graphic, &DialogGraphic::signal_coord_changed, this, &DialogMessage::slot_coord_changed);
    win_graphic->setFixedSize(this->width(), this->height());
    win_graphic->move(0,0);
    win_graphic->show();
}

void DialogMessage::slot_coord_changed(QList<MessData> data)
{
//сохранить изменения
 //  QDomDocument doc = DialogEdit::create_xml(data);
   DialogEdit::create_xml(currPath, data);
   parser_file(currPath);//обновление окна предпросмотра
}

void DialogMessage::on_pushButton_view_clicked()
{
    QListWidgetItem *item = ui->listWidget->currentItem();
    if(item)
    {
    DialogGraphic *win_graphic = new DialogGraphic(currPath, this);
    connect(win_graphic, &DialogGraphic::signal_coord_changed, this, &DialogMessage::slot_coord_changed);
    win_graphic->setFixedSize(this->width(), this->height());
    win_graphic->move(0,0);
    win_graphic->show();
    }
    else
     // QMessageBox::warning(this, "Внимание","Не выделена строка");
    MyMessage::warning(this, "Внимание", "Не выделена строка!");
}

void DialogMessage::on_pushButton_delete_clicked()
{
    QListWidgetItem *item = ui->listWidget->currentItem();
    if(item)
    {
    MyMessage *msgBox = new MyMessage(this);
    msgBox->set_text("Удалить файл\n" + item->text() + " ?" );


if(msgBox->exec() == QDialog::Accepted)
{

    //удаление файла
    QDir dir;
       dir.remove(currPath);




    //удаление NC файла
       QFileInfo info(currPath);
   QString name = info.baseName();

   QString str = QString(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/Laser/");

         QString NC_fileName = str +  "Gcoge/" + name + ".nc";
 dir.remove(NC_fileName);


  find_files();
ui->listWidget->setCurrentRow(0);//???????
  //last_row = ui->listWidget->currentRow()-1;

}
    }
    else
    MyMessage::warning(this, "Внимание", "Не выделена строка!");


}

void DialogMessage::on_pushButton_select_clicked()
{
   emit signal_select(currPath);

   this->close();
}

void DialogMessage::on_pushButton_cancel_clicked()
{
     this->close();
}
