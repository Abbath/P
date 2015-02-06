#include "mainwindow.hpp"
#include "modelingwizard.hpp"
#include <processor.hpp>
#include <QApplication>
#include <QtGui>
#include <QThreadPool>
#include <QSplashScreen>
#include <QWizard>
#include <dialog.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QPixmap splash("splash.png");
    QSplashScreen ss(splash);
    ss.show();
    a.processEvents();
    MainWindow w;
    Processor p;
    w.setProcessor(&p);
    qRegisterMetaType<Display>("Display");
    qRegisterMetaType<QVector<double> >("QVector<double>");
    QObject::connect(&p, SIGNAL(Update(Display)), &w, SLOT(Update(Display)), Qt::ConnectionType::QueuedConnection);
    QObject::connect(&p, SIGNAL(somethingWentWrong(QString, QString)), &w, SLOT(Error(QString, QString)), Qt::ConnectionType::QueuedConnection);
    QObject::connect(&p, SIGNAL(plot(QVector<double>)), &w, SLOT(plot(QVector<double>)), Qt::QueuedConnection);
    QObject::connect(&p, SIGNAL(plot(QVector<double>, QVector<double>)), &w, SLOT(plot(QVector<double>, QVector<double>)), Qt::QueuedConnection);
    QObject::connect(&w, SIGNAL(stop()), &p, SLOT(stopThis()), Qt::QueuedConnection);

    p.loadConf("default.conf");
    w.show();
    
    ModelingWizard wizard;
    wizard.setWindowTitle("Modeling Wizard");
    wizard.show();
    //Dialog d;
    //d.setModal(true);
    // d.show();
    //d.exec();
    //std::cout << "After dialog" << std::endl;
    //QObject::connect(&d, SIGNAL(rejected()), &w, SLOT(close()));
    //QObject::connect(&d, SIGNAL(accepted()), &w, SLOT(show()));
    ss.finish(&w);
    return a.exec();
}
