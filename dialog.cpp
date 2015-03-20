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
    this->setMaximumSize(this->size());
    this->setMinimumSize(this->size());
}

/*!
 * \brief Dialog::~Dialog
 */
Dialog::~Dialog()
{
    delete ui;
}

/*!
 * \brief Dialog::getMod
 * \return 
 */
Dialog::Mode Dialog::getMod() const
{
    return mod;
}

/*!
 * \brief Dialog::setMod
 * \param value
 */
void Dialog::setMod(const Mode &value)
{
    mod = value;
}

/*!
 * \brief Dialog::closeEvent
 */
void Dialog::closeEvent(QCloseEvent *)
{
    this->deleteLater();
}


/*!
 * \brief Dialog::on_pushButton_clicked
 */
void Dialog::on_pushButton_clicked()
{
    w = new ModelingWindow;
    if(ask_for_data){
        ModelingWizard mw;
        mw.exec();
        if(mw.result() == QDialog::Accepted){
            w->setData(mw.getDataRef());
        }
    }
    w->show();
    this->hide();
    connect(w, SIGNAL(death()), this, SLOT(show()), Qt::QueuedConnection);
}

/*!
 * \brief Dialog::on_pushButton_4_clicked
 */
void Dialog::on_pushButton_4_clicked()
{
    mainw = new MainWindow;
    Processor *p = new Processor(mainw);
    mainw->setProcessor(p);
    mainw->setWindowTitle("Calibration");
    qRegisterMetaType<Display>("Display");
    qRegisterMetaType<QVector<double> >("QVector<double>");
    QObject::connect(p, SIGNAL(Update(Display)), mainw, SLOT(Update(Display)), Qt::ConnectionType::QueuedConnection);
    QObject::connect(p, SIGNAL(somethingWentWrong(QString, QString)), mainw, SLOT(Error(QString, QString)), Qt::ConnectionType::QueuedConnection);
    QObject::connect(p, SIGNAL(plot(QVector<double>)), mainw, SLOT(plot(QVector<double>)), Qt::QueuedConnection);
    QObject::connect(p, SIGNAL(plot(QVector<double>, QVector<double>)), mainw, SLOT(plot(QVector<double>, QVector<double>)), Qt::QueuedConnection);
    QObject::connect(mainw, SIGNAL(stop()), p, SLOT(stopThis()), Qt::QueuedConnection);
    
    //p->loadConf("default.conf");
    mainw->show();
    mainw->runCalibration();
    this->hide();
    connect(mainw, SIGNAL(death()), this, SLOT(show()), Qt::QueuedConnection);
}

/*!
 * \brief Dialog::on_pushButton_2_clicked
 */
void Dialog::on_pushButton_2_clicked()
{
    mainw = new MainWindow;
    Processor *p = new Processor(mainw);
    mainw->setProcessor(p);
    mainw->setWindowTitle("Measurements");
    qRegisterMetaType<Display>("Display");
    qRegisterMetaType<QVector<double> >("QVector<double>");
    QObject::connect(p, SIGNAL(Update(Display)), mainw, SLOT(Update(Display)), Qt::ConnectionType::QueuedConnection);
    QObject::connect(p, SIGNAL(somethingWentWrong(QString, QString)), mainw, SLOT(Error(QString, QString)), Qt::ConnectionType::QueuedConnection);
    QObject::connect(p, SIGNAL(plot(QVector<double>)), mainw, SLOT(plot(QVector<double>)), Qt::QueuedConnection);
    QObject::connect(p, SIGNAL(plot(QVector<double>, QVector<double>)), mainw, SLOT(plot(QVector<double>, QVector<double>)), Qt::QueuedConnection);
    QObject::connect(mainw, SIGNAL(stop()), p, SLOT(stopThis()), Qt::QueuedConnection);
    //p->loadConf("default.conf");
    mainw->show();
    mainw->runMeasurements();
    this->hide();
    connect(mainw, SIGNAL(death()), this, SLOT(show()), Qt::QueuedConnection);
}

/*!
 * \brief Dialog::on_checkBox_toggled
 * \param checked
 */
void Dialog::on_checkBox_toggled(bool checked)
{
    ask_for_data = checked;
}

/*!
 * \brief Dialog::getAsk_for_data
 * \return 
 */
bool Dialog::getAsk_for_data() const
{
    return ask_for_data;
}

/*!
 * \brief Dialog::setAsk_for_data
 * \param value
 */
void Dialog::setAsk_for_data(bool value)
{
    ask_for_data = value;
}

/*!
 * \brief Dialog::on_pushButton_3_clicked
 */
void Dialog::on_pushButton_3_clicked()
{
    this->close();
}

/*!
 * \brief Dialog::trayHandle
 * \param reason
 */
void Dialog::trayHandle(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger){
        if(this->isVisible()){
            this->hide();
        }else{
            this->showNormal();
        }
    }
    if(reason == QSystemTrayIcon::MiddleClick){
        this->close();
    }
}

