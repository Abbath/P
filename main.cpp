#include "mainwindow.hpp"
#include "modelingwindow.hpp"
#include "modelingwizard.hpp"
#include "dialog.hpp"
#include "processor.hpp"
#include "db.hpp"
#include <QApplication>
#include <QtGui>
#include <QThreadPool>
#include <QSplashScreen>
#include <QWizard>
#include <QSystemTrayIcon>
#include <iostream>

/*!
 * \brief main
 * \param argc
 * \param argv
 * \return 
 */
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    
    a.setQuitOnLastWindowClosed(false);
    a.processEvents();
    
    Dialog *d = new Dialog;
    d->setWindowIcon(QIcon(":/ops.png"));
    d->show();
    
    QSystemTrayIcon * qsti = new QSystemTrayIcon(d);
    QMenu *trayIconMenu;
    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    
    minimizeAction = new QAction("Minimize", d);
    restoreAction = new QAction("Restore", d);
    quitAction = new QAction("Quit", d);
    
    QObject::connect(minimizeAction, SIGNAL(triggered()), d, SLOT(hide()));
    QObject::connect(restoreAction, SIGNAL(triggered()), d,SLOT(showNormal()));
    QObject::connect(quitAction, SIGNAL(triggered()), &a, SLOT(quit()));
    QObject::connect(d, SIGNAL(destroyed()), &a, SLOT(quit()));
    
    trayIconMenu = new QMenu(d);
    trayIconMenu->addAction (minimizeAction);
    trayIconMenu->addAction (restoreAction);
    trayIconMenu->addAction (quitAction);
    
    qsti->setContextMenu(trayIconMenu);
    qsti->setIcon(QIcon(":/ops.png"));
    qsti->show();
    
    QObject::connect(qsti, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), d, SLOT(trayHandle(QSystemTrayIcon::ActivationReason)));
    
//    DB db;
    
//    qDebug() << db.init().text();
    
//    ModelingWizard *wiz = new ModelingWizard;
    
//    wiz->exec();
    
//    qDebug() << db.addSensor("sensor2", wiz->getDataRef(),2 ,2);
    
    return a.exec();
}
