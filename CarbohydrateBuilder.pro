#-------------------------------------------------
#
# Project created by QtCreator 2017-01-30T11:25:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CarbohydrateBuilder
TEMPLATE = app


SOURCES += main.cpp\
        carbohydratebuilder.cpp

HEADERS  += carbohydratebuilder.h

FORMS    += carbohydratebuilder.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../gems/gmml/bin/release/ -lgmml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../gems/gmml/bin/debug/ -lgmml
else:unix: LIBS += -L$$PWD/../../../gems/gmml/bin/ -lgmml

INCLUDEPATH += $$PWD/../../../gems/gmml/bin
DEPENDPATH += $$PWD/../../../gems/gmml/bin
