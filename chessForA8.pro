#-------------------------------------------------
#
# Project created by QtCreator 2015-06-07T14:55:12
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = chess
TEMPLATE = app


SOURCES += main.cpp\
        choosewidget.cpp \
    mybutton.cpp \
    playwidget.cpp

HEADERS  += choosewidget.h \
    mybutton.h \
    playwidget.h

FORMS    += choosewidget.ui \
    playwidget.ui

RESOURCES += \
    image.qrc \
    image2.qrc
	
RC_ICONS = chess.ico
