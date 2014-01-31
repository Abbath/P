#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
    scene = new QGraphicsScene(this);
    ui->widget->hide();
    ui->graphicsView->setScene(scene);
    connect(ui->imageArea, SIGNAL(viewUpdated(Display)), this, SLOT(imageAreaUpdated(Display)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Update(Display dis)
{
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

void MainWindow::imageAreaUpdated(Display dis)
{
    p->setDisplay(dis);
}

void MainWindow::Error(QString a, QString b)
{
    QMessageBox::warning(this,a,b);
}

void MainWindow::on_actionOpen_Image_s_triggered()
{
    QStringList names = QFileDialog::getOpenFileNames(this, "Open Image(s)",".","Images (*.bmp)");
    QtConcurrent::run(p, &Processor::openImage, names);
}

void MainWindow::on_actionAlign_triggered()
{
    QtConcurrent::run(p, &Processor::align);
}

void MainWindow::on_actionRun_triggered()
{
    QtConcurrent::run(p, &Processor::run);
}

void MainWindow::on_actionReset_triggered()
{
    QtConcurrent::run(p, &Processor::run);
}

void MainWindow::on_actionAutorun_triggered()
{
    QtConcurrent::run(p, &Processor::autorun);
}

void MainWindow::on_actionPrev_triggered()
{
    QtConcurrent::run(p, &Processor::prev);
}

void MainWindow::on_actionNext_triggered()
{
    QtConcurrent::run(p, &Processor::next);
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionLoad_triggered()
{
    QtConcurrent::run(p, &Processor::loadConf, QFileDialog::getOpenFileName(this, tr("Open config"), "", tr("Config files (*.conf)")) );
}

void MainWindow::on_actionSave_triggered()
{
    QtConcurrent::run(p, &Processor::saveConf, QFileDialog::getSaveFileName(this, tr("Save config"), "", tr("Config files (*.conf)")), false);
}

void MainWindow::on_actionLoad_2_triggered()
{
    QtConcurrent::run(p, &Processor::loadData, QFileDialog::getOpenFileName(this,tr("Load data"), "", tr("Data (*.dat)")));
}

void MainWindow::on_actionSave_2_triggered()
{
    QtConcurrent::run(p, &Processor::saveData, QFileDialog::getSaveFileName(this,tr("Save data"), "", tr("Data (*.dat)")));
}

void MainWindow::on_actionSave_as_Default_triggered()
{
    QtConcurrent::run(p, &Processor::saveConf, QString("default.conf"),true);
}

void MainWindow::on_action3D_triggered(bool checked)
{
    if(checked){
        ui->widget->setStep((float)ui->imageArea->getThreshold()/255.0);
        ui->widget->setImage(ui->imageArea->getImage());
        ui->widget->show();
    }else{
        ui->widget->hide();
    }
}
