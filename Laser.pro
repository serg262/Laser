QT       += core gui serialport xml multimedia# svg virtualkeyboard


#QT_FOR_CONFIG += virtualkeyboard
#QTPLUGIN += qtvirtualkeyboardplugin
#CONFIG += lang-all

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialogedit.cpp \
    dialoggraphic.cpp \
    dialoginput.cpp \
    dialogmenu.cpp \
    dialogmessage.cpp \
    dialogmotion.cpp \
    key.cpp \
    keyboard.cpp \
    main.cpp \
    mainwindow.cpp \
    mymessage.cpp \
    paintitem.cpp \
    rectitem.cpp

HEADERS += \
    dialogedit.h \
    dialoggraphic.h \
    dialoginput.h \
    dialogmenu.h \
    dialogmessage.h \
    dialogmotion.h \
    key.h \
    keyboard.h \
    mainwindow.h \
    mymessage.h \
    paintitem.h \
    rectitem.h

FORMS += \
    dialogedit.ui \
    dialoggraphic.ui \
    dialoginput.ui \
    dialogmenu.ui \
    dialogmessage.ui \
    dialogmotion.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rec.qrc

win32: RC_ICONS = $$PWD/Image/laser.ico
