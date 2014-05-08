#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QtConcurrent/QtConcurrent>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

/*!
 * \brief MainWindow::MainWindow
 * \param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
    ui->widget_3->setTitle("Pressure");
    ui->widget_3->setAxisTitle(ui->widget_3->xBottom, "Frame");
    ui->widget_3->setAxisTitle(ui->widget_3->yLeft,"Pressure [kPa]");
    ui->widget_3->setAxisAutoScale( ui->widget_3->xBottom, true );
    ui->widget_3->setAxisAutoScale( ui->widget_3->yLeft, true );
    zoom = new QwtPlotZoomer(ui->widget_3->canvas());
    zoom->setRubberBandPen(QPen(Qt::white));
    QPen pen = QPen( Qt::red );
    curve = new QwtPlotCurve;
    curve->setRenderHint( QwtPlotItem::RenderAntialiased );
    curve->setPen( pen );
    curve->attach( ui->widget_3 );
    
    ui->widget_4->setTitle("Pressure");
    ui->widget_4->setAxisTitle(ui->widget_4->xBottom, "Pressure [kPa]");
    ui->widget_4->setAxisTitle(ui->widget_4->yLeft,"Pixels");
    ui->widget_4->setAxisAutoScale( ui->widget_4->xBottom, true );
    ui->widget_4->setAxisAutoScale( ui->widget_4->yLeft, true );
    zoom0 = new QwtPlotZoomer(ui->widget_4->canvas());
    zoom0->setRubberBandPen(QPen(Qt::white));
    QPen pen0 = QPen( Qt::red );
    curve0 = new QwtPlotCurve;
    curve0->setRenderHint( QwtPlotItem::RenderAntialiased );
    curve0->setPen( pen0 );
    curve0->attach( ui->widget_4 );
    
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    //ui->tab_6->setDisabled(true);
    
    player = new QMediaPlayer;
    player->setVideoOutput(ui->widget_2);
    connect(ui->imageArea, SIGNAL(viewUpdated(Display)), this, SLOT(imageAreaUpdated(Display)));
}

/*!
 * \brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief MainWindow::disableUi
 * \param b
 */
void MainWindow::disableUi(bool b)
{
    if(b){
        ui->splitter->setDisabled(b);
        ui->mainToolBar->setDisabled(b);
        ui->menuBar->setDisabled(b);
    }else{
        ui->splitter->setEnabled(!b);
        ui->mainToolBar->setEnabled(!b);
        ui->menuBar->setEnabled(!b);      
    }
}

/*!
 * \brief MainWindow::Update
 * \param dis
 */
void MainWindow::Update(Display dis)
{
    disableUi(false);
    ui->imageArea->setDisplay(dis);
    ui->pressureLabel->setText(QString::number(dis.im.sum));
    ui->sumLabel->setText(QString::number(std::accumulate(&dis.im.bound_counter[0],dis.im.bound_counter+4,0)));
    ui->averageLabel->setText(QString::number(std::accumulate(&dis.im.bound_counter[0],dis.im.bound_counter+4,0)/4.0));
    ui->rightLabel->setText(QString::number(dis.im.bound_counter[0]));
    ui->leftLabel->setText(QString::number(dis.im.bound_counter[1]));
    ui->topLabel->setText(QString::number(dis.im.bound_counter[2]));
    ui->bottomLabel->setText(QString::number(dis.im.bound_counter[3]));
    scene->addPixmap(QPixmap(dis.im.fileName));
}

/*!
 * \brief MainWindow::imageAreaUpdated
 * \param dis
 */
void MainWindow::imageAreaUpdated(Display dis)
{
    p->setDisplay(dis);
}

/*!
 * \brief MainWindow::Error
 * \param a
 * \param b
 */
void MainWindow::Error(QString a, QString b)
{
    QMessageBox::warning(this,a,b);
}

/*!
 * \brief MainWindow::plot
 * \param t
 * \param res
 */
void MainWindow::plot(QVector<double> res)
{
    QVector < QPointF > points( res.size() );
    quint32 counter = 0;
    auto pointsIt = points.begin();
    
    for ( auto ri = res.constBegin(); ri != res.constEnd(); ++ ri, ++ pointsIt, ++ counter ) {
            (*pointsIt) = QPointF( counter, (*ri));
    }
    
    QwtPointSeriesData * data = new QwtPointSeriesData(points);
    
    ui->widget_3->detachItems( QwtPlotItem::Rtti_PlotCurve, false );
    ui->widget_3->replot();
    curve->setData(data);
    curve->attach( ui->widget_3 );
    ui->widget_3->replot();
}

/*!
 * \brief MainWindow::plot
 * \param res0
 * \param res
 */
void MainWindow::plot(QVector<double> res0, QVector<double> res){
    ui->widget_4->detachItems( QwtPlotItem::Rtti_PlotCurve, false );
    ui->widget_4->replot();
    
    QVector < QPointF > points( res.size() );
    auto pointsIt = points.begin();
    
    for ( int i = 0; i < res.size(); ++i) {
        (*(pointsIt+i)) = QPointF( res[i], res0[i] );
    }
    
    QwtPointSeriesData * data = new QwtPointSeriesData(points);
    curve0->setData(data);
    curve0->attach( ui->widget_4 );
    ui->widget_4->replot();
}

/*!
 * \brief MainWindow::on_actionOpen_Image_s_triggered
 */
void MainWindow::on_actionOpen_Image_s_triggered()
{
    disableUi();
    QStringList names = QFileDialog::getOpenFileNames(this, "Open Image(s)",".","Images (*.bmp)");
    QtConcurrent::run(p, &Processor::openImage, names);
}

/*!
 * \brief MainWindow::on_actionAlign_triggered
 */
void MainWindow::on_actionAlign_triggered()
{
    disableUi();
    QtConcurrent::run(p, &Processor::align);
}

/*!
 * \brief MainWindow::on_actionRun_triggered
 */
void MainWindow::on_actionRun_triggered()
{
    disableUi();
    QtConcurrent::run(p, &Processor::run, true);
}

/*!
 * \brief MainWindow::on_actionReset_triggered
 */
void MainWindow::on_actionReset_triggered()
{
    disableUi();
    QtConcurrent::run(p, &Processor::reset);
}

/*!
 * \brief MainWindow::on_actionAutorun_triggered
 */
void MainWindow::on_actionAutorun_triggered()
{
    disableUi();
    QtConcurrent::run(p, &Processor::autorun,true);
}

/*!
 * \brief MainWindow::on_actionPrev_triggered
 */
void MainWindow::on_actionPrev_triggered()
{
    disableUi();
    QtConcurrent::run(p, &Processor::prev);
}

/*!
 * \brief MainWindow::on_actionNext_triggered
 */
void MainWindow::on_actionNext_triggered()
{
    disableUi();
    QtConcurrent::run(p, &Processor::next);
}

/*!
 * \brief MainWindow::on_actionExit_triggered
 */
void MainWindow::on_actionExit_triggered()
{
    this->close();
}

/*!
 * \brief MainWindow::on_actionLoad_triggered
 */
void MainWindow::on_actionLoad_triggered()
{
    QtConcurrent::run(p, &Processor::loadConf, QFileDialog::getOpenFileName(this, tr("Open config"), "", tr("Config files (*.conf)")) );
}

/*!
 * \brief MainWindow::on_actionSave_triggered
 */
void MainWindow::on_actionSave_triggered()
{
    QtConcurrent::run(p, &Processor::saveConf, QFileDialog::getSaveFileName(this, tr("Save config"), "", tr("Config files (*.conf)")), false);
}

/*!
 * \brief MainWindow::on_actionLoad_2_triggered
 */
void MainWindow::on_actionLoad_2_triggered()
{
    QtConcurrent::run(p, &Processor::loadData, QFileDialog::getOpenFileName(this,tr("Load data"), "", tr("Data (*.dat)")));
}

/*!
 * \brief MainWindow::on_actionSave_2_triggered
 */
void MainWindow::on_actionSave_2_triggered()
{
    QtConcurrent::run(p, &Processor::saveData, QFileDialog::getSaveFileName(this,tr("Save data"), "", tr("Data (*.dat)")));
}

/*!
 * \brief MainWindow::on_actionSave_as_Default_triggered
 */
void MainWindow::on_actionSave_as_Default_triggered()
{
    QtConcurrent::run(p, &Processor::saveConf, QString("default.conf"),true);
}

/*!
 * \brief MainWindow::on_action3D_triggered
 * \param checked
 */
void MainWindow::on_action3D_triggered(bool checked)
{
    if(checked){
        ui->widget->setStep((float)ui->imageArea->getThreshold()/255.0);
        ui->widget->setImage(ui->imageArea->getImage());
    }else{
        QImage empty;
        ui->widget->setStep(1.0f);
        ui->widget->setImage(empty);
    }
}

/*!
 * \brief MainWindow::on_actionCalibrate_triggered
 */
void MainWindow::on_actionCalibrate_triggered()
{    
    QString name = QFileDialog::getOpenFileName(this, tr("Open config"), "", tr("Config files (*.conf)"));
    QStringList names = QFileDialog::getOpenFileNames(this, "Open Image(s)",".","Images (*.bmp)");
    QString named = QFileDialog::getSaveFileName(this,tr("Save data"), "", tr("Data (*.dat)"));
    disableUi();
    QtConcurrent::run(p, &Processor::calibrate, name, named, names);
}

/*!
 * \brief MainWindow::on_actionOpen_Video_triggered
 */
void MainWindow::on_actionOpen_Video_triggered()
{
    QString fileNameV = QFileDialog::getOpenFileName( this, tr("Open data file"), "", tr("Video files (*.avi)"));
    if(!fileNameV.isNull()){
        p->setFileNameV(fileNameV);
        player->setMedia(QUrl::fromLocalFile(p->getVName()));
        QtConcurrent::run(p,&Processor::run);
    }
}

/*!
 * \brief MainWindow::on_pushButton_2_clicked
 */
void MainWindow::on_pushButton_2_clicked()
{
    auto res = p->getRes();
    auto pol = p->getPol();
    if(!res.isEmpty()){
        QString name = QFileDialog::getSaveFileName(this,tr("Save results"), "", tr("Data (*.dat)"));
        QFile file(name);
        if(file.open(QFile::WriteOnly)){
            QTextStream s(&file);
            s << p->getImage().threshold << "\n";
            s << res.size() << "\n";
            for(int i=0; i< res.size(); ++i){
                s << res[i] << " " << pol[i] << "\n";
            }
        }
        QString name1 = QFileDialog::getSaveFileName(this,tr("Save plot image"), "", tr("Images (*.png)"));
        if(!name1.isEmpty()){
            ui->widget_3->grab().save(name1);
        }
    }
}

/*!
 * \brief MainWindow::on_pushButton_3_clicked
 */
void MainWindow::on_pushButton_3_clicked()
{
    auto res = p->getARes();
    auto pol = p->getAPres();
    if(!res.isEmpty()){
        QString name = QFileDialog::getSaveFileName(this,tr("Save results"), "", tr("Data (*.dat)"));
        QFile file(name);
        if(file.open(QFile::WriteOnly)){
            QTextStream s(&file);
            for(int i=0; i< res.size(); ++i){
                s << res[i] << " " << pol[i] << "\n";
            }
        }
        QString name1 = QFileDialog::getSaveFileName(this,tr("Save plot image"), "", tr("Images (*.png)"));
        if(!name1.isEmpty()){
            ui->widget_4->grab().save(name1);
        }
    }
}

/*!
 * \brief MainWindow::on_pushButton_clicked
 * \param checked
 */
void MainWindow::on_pushButton_clicked(bool checked)
{
    if(checked){
        player->play();
    }else{
        player->pause();
    }
}

/*!
 * \brief MainWindow::on_horizontalSlider_valueChanged
 * \param value
 */
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    player->setPosition(player->duration() / 100 * value);
}

void MainWindow::on_actionStop_triggered()
{
    emit stop();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,"About","Radiation sensor toolkit :)\nDanylo Lizanets © 2013-2014.");
}


void MainWindow::on_actionHelp_triggered()
{
     QDesktopServices::openUrl(QUrl("manual.pdf"));
}
