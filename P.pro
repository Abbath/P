#-------------------------------------------------
#
# Project created by QtCreator 2013-10-04T09:26:11
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = P
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imagearea.cpp \
    converter.cpp

HEADERS  += mainwindow.hpp \
    imagearea.hpp \
    converter.hpp \
    helpers.hpp

FORMS    += mainwindow.ui \
    imagearea.ui

QMAKE_CXXFLAGS += -std=c++11
LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -L"/usr/local/qwt-6.1.0/lib" -lqwt

RESOURCES += \
    Icons.qrc

