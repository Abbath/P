#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <QObject>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <processor.hpp>
#include <mainwindow.hpp>

class Dispatcher : public QObject
{
    Q_OBJECT
    MainWindow * w;
    Processor * p;
public:
    explicit Dispatcher(QObject *parent = 0, MainWindow* w = nullptr, Processor * p = nullptr);
signals:
    
public slots:
    
};

#endif // DISPATCHER_HPP
