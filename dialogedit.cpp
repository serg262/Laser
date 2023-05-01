#include "dialogedit.h"
#include "ui_dialogedit.h"

#include "mymessage.h"


#include <QDebug>
#include <QStandardPaths>

#include "dialoggraphic.h"
#include "dialoginput.h"






DialogEdit::DialogEdit(const QString &path, bool create,  QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEdit)
{
    ui->setupUi(this);

    this->setModal(true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::Widget);

  //  _flag_edit = edit;

    QFileInfo info(path);

ui->label_input->setText("Сообщение: " + info.baseName());


    if(!create)
    {


    currPath = path;

    QList<MessData> xml_list = parser_xml(currPath);

int count = 0;

while(xml_list.count() != count)//сортировка
{

    for(const MessData &mess : qAsConst(xml_list))
    {



    QListWidgetItem *item = new QListWidgetItem(mess.mess);

    QFont f = item->font();

    f.setPixelSize(25);
    item->setFont(f);
    item->setSizeHint(QSize(ui->listWidget->width(), 70));




    if(mess.NumLine == count)
    {
         item->setData(Qt::UserRole, 1);//существующая строка

         ui->listWidget->addItem(item);
         count++;
    }





    }
}
ui->listWidget->setCurrentRow(0);

    }
    else
    {
QString dir = QString(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/Laser/");
     currPath = dir + "Layout/"+  path + ".ltc";//новый файл

//ui->listWidget->setCurrentRow(0);

    }
}

DialogEdit::~DialogEdit()
{
    delete ui;
}




QList<MessData> DialogEdit::parser_xml(const QString &path)
{





    QList<MessData> xml_list;
    QDomDocument doc("mydocument");


  QFile file(path);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                 qDebug()<<"Load XML File Problem";

         }
  else
  {


        if (!doc.setContent(&file))
        {
         return QList<MessData>();
        }

        //Get the root element
        QDomElement docElem = doc.documentElement();

  QDomNodeList nodeList = docElem.elementsByTagName("LayoutItemBase");

  for(int ii = 0; ii < nodeList.count(); ii++)

    {
  MessData data;

  // get the current one as QDomElement

     QDomElement el = nodeList.at(ii).toElement();

  QDomNode pEntries = el.firstChild();



  while(!pEntries.isNull()) {
        QDomElement peData = pEntries.toElement();
        QString tagNam = peData.tagName();
      //  if(tagNam == "Name") {
 // data.name = peData.text();


     //   }
  //else
  if(tagNam == "Left") {
  data.point.setX(peData.text().toFloat());

        }else if(tagNam == "Top") {
  data.point.setY(peData.text().toFloat());

       }else if(tagNam == "Height") {
  data.font_height = peData.text().toFloat();

        }else if(tagNam == "Caption") {
  data.mess = peData.text();

    }else if(tagNam == "WidthRect") {
 data.rect.setWidth(peData.text().toFloat());

  }else if(tagNam == "HeightRect") {
data.rect.setHeight(peData.text().toFloat());
  }else if(tagNam == "LeftRect") {
data.rect.setLeft(peData.text().toFloat());
  }else if(tagNam == "TopRect") {
data.rect.setTop(peData.text().toFloat());

  }else if(tagNam == "Tara") {
data.tara = peData.text().toInt();
  }
  else if(tagNam == "NumLine") {
data.NumLine = peData.text().toInt();

        }


          pEntries = pEntries.nextSibling();
  }

    xml_list.append(data);

  }

         file.close();

  }






  return  xml_list;
}


void DialogEdit::create_xml(const QString &path, QList<MessData> xml_list)
{

    QDomDocument doc;

    QDomNode node = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
    doc.insertBefore(node, doc.firstChild());



        QDomElement root = doc.createElement("Layout");
        doc.appendChild(root);

        QDomText t;

        for (int i = 0; i < xml_list.size(); ++i) {
            QDomElement serial = doc.createElement("LayoutItemBase");
            root.appendChild(serial);

            //title
            QDomElement title = doc.createElement("Caption");
            t = doc.createTextNode(xml_list.at(i).mess);
            title.appendChild(t);
            serial.appendChild(title);


            QDomElement Top = doc.createElement("Top");
            t = doc.createTextNode(QString::number(xml_list.at(i).point.y()));
            Top.appendChild(t);
            serial.appendChild(Top);


            QDomElement Left = doc.createElement("Left");
            t = doc.createTextNode(QString::number(xml_list.at(i).point.x()));
            Left.appendChild(t);
            serial.appendChild(Left);

            QDomElement Height = doc.createElement("Height");
            t = doc.createTextNode(QString::number(xml_list.at(i).font_height));
            Height.appendChild(t);
            serial.appendChild(Height);

            QDomElement LeftRect = doc.createElement("LeftRect");
            t = doc.createTextNode(QString::number(xml_list.at(i).rect.left()));
            LeftRect.appendChild(t);
            serial.appendChild(LeftRect);

            QDomElement TopRect = doc.createElement("TopRect");
            t = doc.createTextNode(QString::number(xml_list.at(i).rect.top()));
            TopRect.appendChild(t);
            serial.appendChild(TopRect);


            QDomElement HeightRect = doc.createElement("HeightRect");
            t = doc.createTextNode(QString::number(xml_list.at(i).rect.height()));
            HeightRect.appendChild(t);
            serial.appendChild(HeightRect);

            QDomElement WidthRect = doc.createElement("WidthRect");
            t = doc.createTextNode(QString::number(xml_list.at(i).rect.width()));
            WidthRect.appendChild(t);
            serial.appendChild(WidthRect);

            QDomElement Tara = doc.createElement("Tara");
            t = doc.createTextNode(QString::number(xml_list.at(i).tara));
            Tara.appendChild(t);
            serial.appendChild(Tara);

            QDomElement NumLine = doc.createElement("NumLine");
            t = doc.createTextNode(QString::number(xml_list.at(i).NumLine));
            NumLine.appendChild(t);
            serial.appendChild(NumLine);


        }




    QFile file(path);

 //   if( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
  //    {
  //      qDebug( "Failed to open file for writing." );
  //      return;
   //   }







   // QFile file(path);
    if( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
    qDebug( "Failed to open file for writing." );
    return;
    }
   // if (!doc.setContent(&file))
   // {
   // return;
   // }

/*
    //edit
    //Get the root element
    QDomElement docElem = doc.documentElement();


QDomNodeList nodeList = docElem.elementsByTagName("LayoutItemBase");

for(int ii = 0; ii < xml_list.count(); ii++)

{
MessData data;

// get the current one as QDomElement

 QDomElement el = nodeList.at(ii).toElement();

 QDomNode pEntries = el.firstChild();



while(!pEntries.isNull()) {
    QDomElement peData = pEntries.toElement();

    QString tagNam = peData.tagName();
   // if(tagNam == "Name") {
//peData.firstChild().setNodeValue(xml_list.at(ii).name);

   // }else
if(tagNam == "Left") {
peData.firstChild().setNodeValue(QString::number(xml_list.at(ii).point.x()));

    }else if(tagNam == "Top") {
peData.firstChild().setNodeValue(QString::number(xml_list.at(ii).point.y()));

    }else if(tagNam == "Height") {//шрифт
peData.firstChild().setNodeValue(QString::number(xml_list.at(ii).font_height));



    }else if(tagNam == "Caption") {
peData.firstChild().setNodeValue(xml_list.at(ii).mess);


    }else if(tagNam == "WidthRect") {
peData.firstChild().setNodeValue(QString::number(xml_list.at(ii).rect.width()));

}else if(tagNam == "HeightRect") {
peData.firstChild().setNodeValue(QString::number(xml_list.at(ii).rect.height()));

}else if(tagNam == "LeftRect") {
peData.firstChild().setNodeValue(QString::number(xml_list.at(ii).rect.left()));

}else if(tagNam == "TopRect") {
peData.firstChild().setNodeValue(QString::number(xml_list.at(ii).rect.top()));
}


else if(tagNam == "Tara") {
    peData.firstChild().setNodeValue(QString::number(xml_list.at(ii).tara));
    }


else if(tagNam == "NumLine") {
    peData.firstChild().setNodeValue(QString::number(xml_list.at(ii).NumLine));
    }


      pEntries = pEntries.nextSibling();
}



}

file.resize(0);

*/

    QTextStream out(&file);
        doc.save(out, 4);
        file.close();


}




void DialogEdit::on_pushButton_cancel_clicked()
{
    this->close();
}

void DialogEdit::slot_send_Gcode(QStringList code)
{
    emit signal_send_Gcode(code);
}

void DialogEdit::slot_OK_input(const QString &str, bool edit)
{
    if(!edit)
    {
    QListWidgetItem *item = new QListWidgetItem(str);

    QFont f = item->font();

    f.setPixelSize(25);
    item->setFont(f);
    item->setSizeHint(QSize(ui->listWidget->width(), 70));
    ui->listWidget->addItem(item);
 ui->listWidget->setCurrentItem(item);
    }
    else
    {
QListWidgetItem *item = ui->listWidget->currentItem();
item->setText(str);
ui->listWidget->setCurrentItem(item);
    }


    //сохранит в файл
update_file();
}


void DialogEdit::on_pushButton_edit_clicked()
{
    //изменить выделенную строку



    QListWidgetItem *item = ui->listWidget->currentItem();

    if(item)
    {
 _flag_edit = true;
 _flag_remove = false;
    //клавиатура
    DialogInput *win_input = new DialogInput(item->text(), "Введите имя сообщения", this);
    connect(win_input, &DialogInput::signal_OK_input, this, &DialogEdit::slot_OK_input);
    win_input->setFixedSize(this->width(), this->height());
    win_input->move(0,0);
    win_input->show();
    }
    else
    // QMessageBox::warning(this, "Внимание","Не выделена строка");
 MyMessage::warning(this, "Внимание", "Не выделена строка!");

}

void DialogEdit::on_pushButton_add_clicked()
{
    //клавиатура
    DialogInput *win_input = new DialogInput("", "Введите строку",this);
    connect(win_input, &DialogInput::signal_OK_input, this, &DialogEdit::slot_OK_input);
    win_input->setFixedSize(this->width(), this->height());
    win_input->move(0,0);
    win_input->show();

    _flag_edit = _flag_remove = false;
}

void DialogEdit::on_pushButton_OK_clicked()
{
  //  if(ui->listWidget->count())
  //  {

update_file();


emit signal_list_update(currPath);
emit signal_open_view(currPath);


this->close();
  //  }
   // else
      //  QMessageBox::warning(this, "Внимание","Пустая строка");
  //   MyMessage::warning(this, "Внимание", "Пустая строка!");
}

void DialogEdit::update_file()
{


    if(!_flag_edit && !_flag_remove)
    {
        //сохранение в файл
        QList<MessData> new_xml_list;
     QList<MessData> xml_list = parser_xml(currPath);



        for(int i=0;i<ui->listWidget->count();i++)
        {
         QListWidgetItem *item=  ui->listWidget->item(i);

         int user_data = item->data(Qt::UserRole).toInt();

          MessData data;
    if(!user_data)//новая строка
    {
     float height = 8;//????????????

         data.mess = item->text();

    data.NumLine = i;


         data.point.setX(5);//?????????????
         data.point.setY(0 + height * i);//????????????????


    data.font_height = 5;//по умочанию


    data.rect.setLeft(left_2);//??????????????
    data.rect.setTop(top_2);//??????????????
    data.rect.setWidth(width_2);//??????????
    data.rect.setHeight(height_2);//??????????

    data.tara = 2;//крышка


    new_xml_list.append(data);

    }
    else
    {
       data = xml_list.at(i);
       new_xml_list.append(data);

    }
        }



    create_xml(currPath, new_xml_list);//новый


           // emit signal_list_update(currPath);
    }
    else
       {

                 QList<MessData> xml_list = parser_xml(currPath);
                 QList<MessData> new_xml_list;

                 int k=0;

                 for(int i=0; i < xml_list.size(); i++)//???????????
                 {
                  QListWidgetItem *item =  ui->listWidget->item(k);

                  MessData data = xml_list.at(i);
    if(item)
    {
                  if(_flag_edit && !_flag_remove)
                  {

    data.mess = item->text();

                 // data.NumLine = k;//????????

                  new_xml_list.append(data);
                  k++;
                  }
                  else
                  if(_flag_remove && data.mess == item->text())
                  {

                      data.NumLine = k;//????????

                      new_xml_list.append(data);
                      k++;



                  }
    }
                 }



            create_xml(currPath, new_xml_list);//редактирование

         }


}


void DialogEdit::on_pushButton_remove_clicked()
{
    QListWidgetItem *item = ui->listWidget->currentItem();
    if(item)
    {
delete ui->listWidget->takeItem(ui->listWidget->row(item));
 _flag_remove = true;
  _flag_edit = false;

 /*
    //удалить из файла!!!!!!!!!!!!!!!!!!!!!!!!!
        QList<MessData> xml_list = parser_xml(currPath);
     QList<MessData> new_xml_list;

        for(int i=0;i< ui->listWidget->count();i++)
        {
         QListWidgetItem *item=  ui->listWidget->item(i);

         MessData data = xml_list.at(i);

         data.mess = item->text();
         data.NumLine = i;

         new_xml_list.append(data);
        }

        QDir dir;
           dir.remove(currPath);

           create_xml(currPath, new_xml_list);

*/
    emit signal_list_update(currPath);

  // edit_xml(currPath, new_xml_list);

update_file();
    }
    else
      //  QMessageBox::warning(this, "Внимание","Не выделена строка");
    MyMessage::warning(this, "Внимание", "Не выделена строка!");
}

void DialogEdit::on_pushButton_up_clicked()
{

    QListWidgetItem *item = ui->listWidget->currentItem();
    if(item)
    {




    }
    int currRow = ui->listWidget->currentRow();
        if(currRow > 0){
            QListWidgetItem tmp = *(ui->listWidget->currentItem());
            int upRow = currRow - 1;

            tmp.setData(Qt::UserRole, upRow);
            ui->listWidget->item(upRow)->setData(Qt::UserRole, currRow);



            *(ui->listWidget->item(currRow)) = *(ui->listWidget->item(upRow));
            *(ui->listWidget->item(upRow)) = tmp;



           ui->listWidget->setCurrentItem(ui->listWidget->item(upRow));
        }


}

void DialogEdit::on_pushButton_down_clicked()
{
    int currRow = ui->listWidget->currentRow();
        if(currRow < ui->listWidget->count()-1){
            QListWidgetItem tmp = *(ui->listWidget->currentItem());
            int downRow = currRow + 1;

            tmp.setData(Qt::UserRole, downRow);
            ui->listWidget->item(downRow)->setData(Qt::UserRole, currRow);

            *(ui->listWidget->item(currRow)) = *(ui->listWidget->item(downRow));
            *(ui->listWidget->item(downRow)) = tmp;
           ui->listWidget->setCurrentItem(ui->listWidget->item(downRow));
        }
}
