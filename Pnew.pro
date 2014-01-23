#-------------------------------------------------
#
# Project created by QtCreator 2014-01-23T17:09:47
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pnew
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    processor.cpp \
    imagearea.cpp \
    dispatcher.cpp

HEADERS  += mainwindow.hpp \
    processor.hpp \
    imagearea.hpp \
    dispatcher.hpp \
    helpers.hpp

FORMS    += mainwindow.ui \
    imagearea.ui

QMAKE_CXXFLAGS += -std=c++11

unix{
LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -L"/usr/local/qwt-6.1.0/lib" -lqwt
INCLUDEPATH += "/usr/local/qwt-6.1.0/include"
LIBS += -lqwt
}

win32{
LIBS += -lopencv_core246 -lopencv_highgui246 -lopencv_imgproc246 -lqwt 
LIBS += -L"C:\opencv-build\bin"
INCLUDEPATH += "C:\opencv\build\include"
INCLUDEPATH += "C:\Users\Dan\Downloads\qwt-6.1.0\qwt-6.1.0"
LIBS += -L"C:\Users\Dan\Downloads\qwt-6.1.0\qwt-6.1.0\lib"
RESOURCES +=
}
