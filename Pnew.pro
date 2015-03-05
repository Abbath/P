#-------------------------------------------------
#
# Project created by QtCreator 2014-01-23T17:09:47
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
qtHaveModule(opengl) {
    QT += opengl
}
TARGET = Pnew
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    processor.cpp \
    imagearea.cpp \
    dialog.cpp \
    capturewrapper.cpp \
    imageconverter.cpp \
    modelingwindow.cpp \
    modelingcore.cpp \
    modelingdisplay.cpp \
    modelingwizard.cpp \
    pages/startpage.cpp \
    pages/sizespage.cpp \
    pages/matpage.cpp \
    pages/miscpage.cpp \
    pages/holespage.cpp \
    pages/campage.cpp

HEADERS  += mainwindow.hpp \
    processor.hpp \
    imagearea.hpp \
    helpers.hpp \
    dialog.hpp \
    capturewrapper.hpp \
    imageconverter.hpp \
    modelingwindow.hpp \
    modelingcore.hpp \
    modelingdisplay.hpp \
    modelingwizard.hpp \
    pages/startpage.hpp \
    pages/sizespage.hpp \
    pages/matpage.hpp \
    pages/miscpage.hpp \
    pages/holespage.hpp \
    pages/campage.hpp

FORMS    += mainwindow.ui \
    imagearea.ui \
    setup.ui \
    modelingwindow.ui \
    pages/startpage.ui \
    pages/sizespage.ui \
    pages/matpage.ui \
    pages/miscpage.ui \
    pages/holespage.ui \
    pages/campage.ui \
    dialog.ui

unix{
LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -L"/usr/local/qwt-6.1.0/lib" -lqwt
INCLUDEPATH += "/usr/local/qwt-6.1.0/include"
LIBS += -lqwt
QMAKE_CXXFLAGS += -Wall -Wextra -std=c++1y   -fopenmp
QMAKE_LFLAGS += -fopenmp
}

win32{
LIBS += -lopencv_core249 -lopencv_highgui249 -lopencv_imgproc249 -lqwt
LIBS += -L"C:\opencv-build\bin"
#INCLUDEPATH += "C:\opencv\build\include"
#INCLUDEPATH += "C:\Users\Dan\Downloads\qwt-6.1.0\qwt-6.1.0\src"
INCLUDEPATH += "E:\opencv\build\include"
INCLUDEPATH += "E:\qwt-6.1.0\qwt-6.1.0\qwt"
LIBS += -L"C:\Users\Dan\Downloads\qwt-6.1.0\qwt-6.1.0\lib"
RESOURCES +=
QMAKE_CXXFLAGS += -openmp
}

RESOURCES += \
    shaders.qrc \
    images.qrc

OTHER_FILES +=

win32:CONFIG(release, debug|release): LIBS += -L"C:/Qwt-6.1.0/lib/" -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L"C:/Qwt-6.1.0/lib/" -lqwtd

win32:CONFIG(release, debug|release): LIBS += -L"E:\opencv-2.4.9\opencv-2.4.9-build\lib" -lopencv_core249
else:win32:CONFIG(debug, debug|release): LIBS += -L"E:\opencv-2.4.9\opencv-2.4.9-build\lib" -lopencv_core249

win32:CONFIG(release, debug|release): LIBS += -L"E:\opencv-2.4.9\opencv-2.4.9-build\lib" -lopencv_imgproc249
else:win32:CONFIG(debug, debug|release): LIBS += -L"E:\opencv-2.4.9\opencv-2.4.9-build\lib" -lopencv_imgproc249

win32:CONFIG(release, debug|release): LIBS += -L"E:\opencv-2.4.9\opencv-2.4.9-build\lib" -lopencv_highgui249
else:win32:CONFIG(debug, debug|release): LIBS += -L"E:\opencv-2.4.9\opencv-2.4.9-build\lib" -lopencv_highgui249
