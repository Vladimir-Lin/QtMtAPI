NAME         = QtMtAPI
TARGET       = $${NAME}
QT           = core
QT          -= gui
QT          -= script
QT          += network

load(qt_module)

INCLUDEPATH += $${PWD}/../../include/QtMtAPI

HEADERS     += $${PWD}/../../include/QtMtAPI/QtMtAPI
HEADERS     += $${PWD}/../../include/QtMtAPI/qtmtmanapi.h
HEADERS     += $${PWD}/../../include/QtMtAPI/MT4ManagerAPI.h

SOURCES     += $${PWD}/qtmtmanapi.cpp

OTHER_FILES += $${PWD}/../../include/$${NAME}/headers.pri

include ($${PWD}/../../doc/Qt/Qt.pri)
