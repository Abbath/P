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
    converter.cpp \
    modalwaitdialog.cpp

HEADERS  += mainwindow.hpp \
    imagearea.hpp \
    converter.hpp \
    helpers.hpp \
    modalwaitdialog.h

FORMS    += mainwindow.ui \
    imagearea.ui

QMAKE_CXXFLAGS += -std=c++11

unix{
LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -L"/usr/local/qwt-6.1.0/lib" -lqwt 
INCLUDEPATH += "/usr/local/qwt-6.1.0/include"
}

win32{
LIBS += -lopencv_core246 -lopencv_highgui246 -lopencv_imgproc246 -lqwt 
LIBS += -L"C:\opencv-build\bin"
INCLUDEPATH += "C:\opencv\build\include"
INCLUDEPATH += "C:\Users\Dan\Downloads\qwt-6.1.0\qwt-6.1.0"
LIBS += -L"C:\Users\Dan\Downloads\qwt-6.1.0\qwt-6.1.0\lib"
RESOURCES +=
}
