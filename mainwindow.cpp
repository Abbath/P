#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
    ui->horizontalSlider->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    ui->widget->open();
}

void MainWindow::on_actionReset_triggered()
{
    ui->widget->reset();
}

void MainWindow::on_actionRun_triggered()
{
    ui->widget->run();
}

void MainWindow::on_actionAlign_triggered()
{
    ui->widget->align();
}

void MainWindow::on_action3D_triggered()
{
    ui->widget->switchMode();
}

void MainWindow::on_actionSave_triggered()
{
    //ui->widget->saveImage();
    ui->widget->saveConf();
}

void MainWindow::keyPressEvent(QKeyEvent *e){
    if(e->key() == Qt::Key_1){
        ui->widget->setMode(BOTTOM);
    }
    if(e->key() == Qt::Key_2){
        ui->widget->setMode(ISO);
    }
    if(e->key() == Qt::Key_3){
        ui->widget->setMode(LEFT);
    }
    if(e->key() == Qt::Key_4){
        ui->widget->setMode(RIGHT);
    }
}

void MainWindow::on_actionLoad_triggered()
{
    ui->widget->loadConf();
}

void MainWindow::on_actionCalibrate_triggered()
{
    ui->widget->calibrate();
}

void MainWindow::on_actionAutorun_triggered()
{
    ui->widget->autorun();
}

void MainWindow::on_actionOpen_Video_triggered()
{
    int n = ui->widget->openVideo();
    ui->horizontalSlider->show();
    ui->horizontalSlider->setMaximum(n);
    ui->horizontalSlider->setValue(0);
    ui->widget->getFrame(0);
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->widget->getFrame(value);
}
