#include <QPainter>
#include <QMouseEvent>
#include<QDebug>
#include<QTimer>



#include "key.h"
#include "keyboard.h"

#define KEYS_TYPE 4

// TODO make an enum
#define LOWERCASE_EN 0
#define UPPERCASE_EN 1
#define LOWERCASE_RU 2
#define UPPERCASE_RU 3


// Declaration off the differente keys...




const char *en_lower_keymap[] = {
     "`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "<<-",
    "Tab", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p","(",")","\\",
    "Caps", "a", "s", "d", "f", "g", "h", "j", "k", "l",";", "\"", "Enter",
    "Shift", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "Shift",
    "ENG", " ", "<-", "->"
};

const char *en_upper_keymap[] = {
     "`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "<<-",
    "Tab", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P","(",")","\\",
    "Caps", "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "''", "Enter",
    "Shift", "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "Shift",
    "ENG", " ", "<-", "->"
};

const char *ru_lower_keymap[] = {
     "`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "<<-",
    "Tab", "й", "ц", "у", "к", "е", "н", "г", "ш", "щ", "з","х","ъ","\\",
    "Caps", "ф", "ы", "в", "а", "п", "р", "о", "л", "д","ж", "э", "Enter",
    "Shift", "я", "ч", "с", "м", "и", "т", "ь", "б", "ю", ".", "Shift",
    "RUS", " ", "<-", "->"
};

const char *ru_upper_keymap[] = {
     "`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "<<-",
    "Tab", "Й", "Ц", "У", "К", "Е", "Н", "Г", "Ш", "Щ", "З","Х","Ъ","\\",
    "Caps", "Ф", "Ы", "В", "А", "П", "Р", "О", "Л", "Д", "Ж", "Э", "Enter",
    "Shift", "Я", "Ч", "С", "М", "И", "Т", "Ь", "Б", "Ю", ".", "Shift",
    "RUS", " ", "<-", "->"
};



/*
const char *en_upper_keymap[] = {
    "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
    "A", "S", "D", "F", "G", "H", "J", "K", "L",";",
    "Caps", "Z", "X", "C", "V", "B", "N", "M",",", "backspace",
    "123", ".", "space", "@", "return",
};

const char *en_number_keymap[] = {
   "`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
    "-", "/", ":", ";", "(", ")", "€", "&", "@", "\"",
    "#+=", ".", ",", "?", "!", "'", "+","\\","%","backspace",
    "ABC", ",", "space", ".", "return",
};

const char *en_punctuation_keymap[] = {
    "[", "]", "{", "}", "#", "%", "^", "*", "+", "=",
    "_", "\\", "|", "~", "<", ">", "=","$", "@", "\"",
    "123", ".", ",", "?", "!", "'","/",":",";", "backspace",
    "ABC", ",", "space", ".", "return",
};
*/
// In witch row are the key... (there's 4 rows )
const int row_keymapp[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,
    3,3,3,3,3,3,3,3,3,3,3,3,
    4,4,4,4
};

const int nbkey = sizeof(en_lower_keymap)/ sizeof(char *);

Keyboard::Keyboard(QWidget *p) : QWidget(p)
{
  //  currentKey = 0x0;
    currentindexkeyboard = LOWERCASE_RU;
   // uppercase = false;


    setAttribute(Qt::WA_AcceptTouchEvents);
  initTooltip();

    keys = QVector<QVector< key * > >(KEYS_TYPE);
    for (int n=0;n < KEYS_TYPE ; n++)
    {
      keys[n] = QVector< key * >(nbkey);
    }


    //initKeys(NUMBER,en_number_keymap);
    initKeys(LOWERCASE_EN,en_lower_keymap);
    initKeys(UPPERCASE_EN,en_upper_keymap);
    initKeys(LOWERCASE_RU,ru_lower_keymap);
    initKeys(UPPERCASE_RU,ru_upper_keymap);
    //  initKeys(PUNCTUATION,en_punctuation_keymap);
}



void Keyboard::initKeys( int indexArraykeys,const char *keymap[])
{
int width = 71;
int height = 73;

    int row = 0;
    for(int n=0; n< nbkey; n++)
    {
        keys[indexArraykeys][n] = new key(keymap[n], QSize(width, height));
        if ( n>0)
        {

            if (row_keymapp[n-1]!=row_keymapp[n])
            {
                row ++;
                keys[indexArraykeys][n]->setX(0); //offetrows[row]);
            } else {
                keys[indexArraykeys][n]->setX(keys[indexArraykeys][n-1]->X + keys[indexArraykeys][n-1]->W);
            }
            keys[indexArraykeys][n]->setY(row_keymapp[n]*height);
        } else {
          //  keys[indexArraykeys][n]->setX(offetrows[0]);
            keys[indexArraykeys][n]->setX(0);
            keys[indexArraykeys][n]->setY(row_keymapp[n]*width);
        }
    }
}

void Keyboard::initTooltip()
{
    tooltip = new QLabel("");
    tooltip->setWindowFlags( Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );

    QFont serifFont("Times", 18, QFont::Bold);
    tooltip->setFont(serifFont);
    tooltip->setAlignment(Qt::AlignCenter);

}

//void Keyboard::mousePressEvent(QMouseEvent * e) {
    //QPoint pos = e->pos();
   // setKeyPressed( findKey(pos), pos );
//}

//void Keyboard::mouseMoveEvent(QMouseEvent * e) {
 //   QPoint pos = e->pos();

 //   if (currentKey != 0x0 && !currentKey->getRect().contains(pos)) {
   /////     tooltip->hide();
   //     currentKey->setPressed(false);
  //      this->repaint();
  //  }
  //  setKeyPressed( findKey(pos), pos );
// }

void Keyboard::mousePressEvent(QMouseEvent *event) {
    QPoint pos = event->pos();
    setKeyPressed( findKey(pos), pos );

    key *k = findKey( pos );



    if (k != 0x0 )
    {

   k->setPressed(true);

        if ( k->text == "RUS")
        {
          //  currentindexkeyboard = LOWERCASE_EN;



            if(currentindexkeyboard == UPPERCASE_RU)
           currentindexkeyboard = UPPERCASE_EN;
            else
                if(currentindexkeyboard == LOWERCASE_RU)
               currentindexkeyboard = LOWERCASE_EN;
        //    repaint();

this->repaint();

          //  return;
        }
        if ( k->text == "ENG" )
        {



            if(currentindexkeyboard == LOWERCASE_EN)
           currentindexkeyboard = LOWERCASE_RU;
            else
                if(currentindexkeyboard == UPPERCASE_EN)
               currentindexkeyboard = UPPERCASE_RU;

           // currentindexkeyboard = LOWERCASE_RU;


this->repaint();
          //  repaint();

         //   return;
        }

        if (k->text=="Caps")
        {


        // flag_caps = !flag_caps;
          change_register();
      this->repaint();
            //return;
        }


            if ( k->text =="<<-" )
            {
                emit backspacePressed();
             //   return;
            }

            if ( k->text == "<-" )
            {
                emit leftPressed();
              //  return;
            }

            if ( k->text == "->" )
            {
                emit rightPressed();
               // return;
            }

            if (k->text=="Enter")
            {
                emit returnPressed();
              //  return;
            }

            if (k->text=="Shift")
            {

            flag_shift = !flag_shift;

           //k->set

            change_register();//?????



          //  return;
            }
        }


   // this->repaint();

}

void Keyboard::mouseReleaseEvent(QMouseEvent *event)
{
   //////// tooltip->hide();
   QPoint pos = event->pos();
  //  setKeyPressed( findKey(pos), pos );

    key *k = findKey( pos );

     if(!k)return;

    k->setPressed(false);


  //this->repaint();

    QTimer::singleShot(200, [=](){tooltip->hide();});
}

key *Keyboard::findKey(QPoint p)
{
    foreach (key *k, keys[currentindexkeyboard])
    {
      if (k->getRect().contains(p))
      {

          return k;
      }
    }
    return 0x0;
}

void Keyboard::setKeyPressed( key *k, QPoint pos)
{
  //  currentKey = k;
    if (k == 0x0 || k->text == "Shift" || k->text == "Tab" ||
        k->text == "RUS" || k->text == "ENG" || k->text == "<<-" ||
            k->text == "Enter" || k->text == "<-"|| k->text == "->" || k->text == "Caps")
    {
       //  this->repaint();
        return;

    }


    k->setPressed(true);

    QPoint p = QWidget::mapToGlobal(QPoint( k->X, k->Y));
     tooltip->setGeometry(p.x()+10, p.y()-40,50, 50);
  tooltip->setText(k->text);
    tooltip->show();


if(flag_shift)
{
change_register();
flag_shift = false;
}



    emit keyPressed(k->text[0]);




}

void Keyboard::change_register()
{
    if ( uppercase == false)
    {
        if(currentindexkeyboard == LOWERCASE_EN)
       currentindexkeyboard = UPPERCASE_EN;
        else
            if(currentindexkeyboard == LOWERCASE_RU)
           currentindexkeyboard = UPPERCASE_RU;


       uppercase = true;
    }
    else
    {
        if(currentindexkeyboard == UPPERCASE_RU)
       currentindexkeyboard = LOWERCASE_RU;
        else
            if(currentindexkeyboard == UPPERCASE_EN)
           currentindexkeyboard = LOWERCASE_EN;


        uppercase = false;
    }

    this->repaint();

}

void Keyboard::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    foreach (key *k, keys[currentindexkeyboard])
    {
        k->draw(&painter,style());
    }
}
