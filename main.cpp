#include "mainwindow.hpp"
#include "modelingwindow.hpp"
#include "modelingwizard.hpp"
#include "dialog.hpp"
#include "processor.hpp"
#include <QApplication>
#include <QtGui>
#include <QThreadPool>
#include <QSplashScreen>
#include <QWizard>
#include <iostream>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
//    QPixmap splash("splash.png");
//    QSplashScreen ss(splash);
//    ss.show();
    a.processEvents();
    Dialog *d = new Dialog;
    d->show();
//    auto mode = d.getMod();
//    bool afd = d.getAsk_for_data();
//    if(d.result() == QDialog::Accepted){
//        if(mode == Dialog::MODELING){
//            ModelingWindow w;
//            if(afd){
//                ModelingWizard mw;
//                mw.exec();
//                if(mw.result() == QDialog::Accepted){
//                    w.setData(mw.getDataRef());
//                }
//            }
//            w.show();
//           // ss.finish(&w);    
//            return a.exec();
//        }else {
//            MainWindow w;
//            Processor p;
//            w.setProcessor(&p);
//            qRegisterMetaType<Display>("Display");
//            qRegisterMetaType<QVector<double> >("QVector<double>");
//            QObject::connect(&p, SIGNAL(Update(Display)), &w, SLOT(Update(Display)), Qt::ConnectionType::QueuedConnection);
//            QObject::connect(&p, SIGNAL(somethingWentWrong(QString, QString)), &w, SLOT(Error(QString, QString)), Qt::ConnectionType::QueuedConnection);
//            QObject::connect(&p, SIGNAL(plot(QVector<double>)), &w, SLOT(plot(QVector<double>)), Qt::QueuedConnection);
//            QObject::connect(&p, SIGNAL(plot(QVector<double>, QVector<double>)), &w, SLOT(plot(QVector<double>, QVector<double>)), Qt::QueuedConnection);
//            QObject::connect(&w, SIGNAL(stop()), &p, SLOT(stopThis()), Qt::QueuedConnection);
            
//            p.loadConf("default.conf");
//            w.show();
//            if(mode == Dialog::CALIBRATION){
//                w.runCalibration();
//            }
//           // ss.finish(&w);
//            return a.exec();
//        }
//    }
    //    ModelingWizard wizard;
    //    wizard.setWindowTitle("Modeling Wizard");
    //    wizard.exec();
    //    ModelingData& d = wizard.getDataRef();
    //    std::cerr << d.getIs_ext() << std::endl;
    //    std::cerr << d.getFilename().toStdString() << std::endl;
    //    std::cerr << d.getDie_size() << std::endl;
    //    std::cerr << d.getMem_size() << std::endl;
    //    std::cerr << d.getMem_thickness() << std::endl;
    //    std::cerr << d.getSpacer_height() << std::endl;
    //    std::cerr << d.getRay_number() << std::endl;
    //    std::cerr << d.getWavelength() << std::endl;
    //    std::cerr << d.getPressure() << std::endl;
    //    std::cerr << d.getRows() << std::endl;
    //    std::cerr << d.getHole_size() << std::endl;
    //    std::cerr << d.getSpace_size() << std::endl;
    //    std::cerr << d.getYm() << std::endl;
    //    std::cerr << d.getPr() << std::endl;
    //    std::cerr << d.getCamx() << std::endl;
    //    std::cerr << d.getCamy() << std::endl;
    //    std::cerr << d.getX_angle() << std::endl;
    //    std::cerr << d.getY_angle() << std::endl;
    //Dialog d;
    //d.setModal(true);
    // d.show();
    //d.exec();
    //std::cout << "After dialog" << std::endl;
    //QObject::connect(&d, SIGNAL(rejected()), &w, SLOT(close()));
    //QObject::connect(&d, SIGNAL(accepted()), &w, SLOT(show()));
   // ss.close();
    return a.exec();
}
