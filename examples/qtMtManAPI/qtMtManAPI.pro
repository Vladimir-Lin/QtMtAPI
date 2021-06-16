#-------------------------------------------------
#
# Project created by QtCreator 2015-05-20T06:33:42
#
#-------------------------------------------------

QT        += core
QT        += QtMtAPI
QT        -= gui
CONFIG    += console

TARGET    = qtMtManAPI
TEMPLATE  = app

SOURCES  += main.cpp

win32 {
LIBS     += -lws2_32
}
