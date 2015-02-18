#include "dialog.hpp"
#include "ui_dialog.h"

/*!
 * \brief Dialog::Dialog
 * \param parent
 */
Dialog::Dialog(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

/*!
 * \brief Dialog::~Dialog
 */
Dialog::~Dialog()
{
    delete ui;
}

Dialog::Mode Dialog::getMod() const
{
    return mod;
}

void Dialog::setMod(const Mode &value)
{
    mod = value;
}


void Dialog::on_pushButton_clicked()
{
//    mod = MODELING;
//    this->accept();
    ModelingWindow *w = new ModelingWindow;
    if(ask_for_data){
        ModelingWizard mw;
        mw.exec();
        if(mw.result() == QDialog::Accepted){
            w->setData(mw.getDataRef());
        }
    }
    w->show();
    this->hide();
}

void Dialog::on_pushButton_4_clicked()
{
    MainWindow *w = new MainWindow;
    Processor *p = new Processor;
    w->setProcessor(p);
    w->setWindowTitle("Calibration");
    qRegisterMetaType<Display>("Display");
    qRegisterMetaType<QVector<double> >("QVector<double>");
    QObject::connect(p, SIGNAL(Update(Display)), w, SLOT(Update(Display)), Qt::ConnectionType::QueuedConnection);
    QObject::connect(p, SIGNAL(somethingWentWrong(QString, QString)), w, SLOT(Error(QString, QString)), Qt::ConnectionType::QueuedConnection);
    QObject::connect(p, SIGNAL(plot(QVector<double>)), w, SLOT(plot(QVector<double>)), Qt::QueuedConnection);
    QObject::connect(p, SIGNAL(plot(QVector<double>, QVector<double>)), w, SLOT(plot(QVector<double>, QVector<double>)), Qt::QueuedConnection);
    QObject::connect(w, SIGNAL(stop()), p, SLOT(stopThis()), Qt::QueuedConnection);
    
    p->loadConf("default.conf");
    w->show();
    w->runCalibration();
    this->hide();
}

void Dialog::on_pushButton_2_clicked()
{
//    mod = MEASUREMENT;
//    this->accept();
    MainWindow *w = new MainWindow;
    Processor *p = new Processor;
    w->setProcessor(p);
    w->setWindowTitle("Calibration");
    qRegisterMetaType<Display>("Display");
    qRegisterMetaType<QVector<double> >("QVector<double>");
    QObject::connect(p, SIGNAL(Update(Display)), w, SLOT(Update(Display)), Qt::ConnectionType::QueuedConnection);
    QObject::connect(p, SIGNAL(somethingWentWrong(QString, QString)), w, SLOT(Error(QString, QString)), Qt::ConnectionType::QueuedConnection);
    QObject::connect(p, SIGNAL(plot(QVector<double>)), w, SLOT(plot(QVector<double>)), Qt::QueuedConnection);
    QObject::connect(p, SIGNAL(plot(QVector<double>, QVector<double>)), w, SLOT(plot(QVector<double>, QVector<double>)), Qt::QueuedConnection);
    QObject::connect(w, SIGNAL(stop()), p, SLOT(stopThis()), Qt::QueuedConnection);
    
    p->loadConf("default.conf");
    w->show();
    this->hide();
}

void Dialog::on_checkBox_toggled(bool checked)
{
    ask_for_data = checked;
}
bool Dialog::getAsk_for_data() const
{
    return ask_for_data;
}

void Dialog::setAsk_for_data(bool value)
{
    ask_for_data = value;
}


void Dialog::on_pushButton_3_clicked()
{
    this->close();
}

