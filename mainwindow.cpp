#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
    ui->horizontalSlider->hide();
    ui->groupBox->hide();
    connect(ui->widget,SIGNAL(giveImage(Image)),this,SLOT(getImage(Image)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    ui->widget->openImage();
    ui->horizontalSlider->hide();
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
    ui->horizontalSlider->hide();
}

void MainWindow::on_actionSave_triggered()
{
    //ui->widget->saveImage();
    ui->widget->saveConf(false);
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
    ui->widget->loadConf(false);
}

void MainWindow::on_actionCalibrate_triggered()
{
    ui->widget->calibrate();
    ui->horizontalSlider->hide();
}

void MainWindow::on_actionAutorun_triggered()
{
    ui->widget->autorun();
}

void MainWindow::on_actionOpen_Video_triggered()
{
    int n = ui->widget->openVideo();
    if(n){
        ui->horizontalSlider->show();
        ui->horizontalSlider->setMaximum(n-1);
        ui->horizontalSlider->setValue(0);
        ui->widget->getFrame(0);
    }
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->widget->getFrame(value);
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionSave_as_Default_triggered()
{
    ui->widget->saveConf(true);
}

void MainWindow::on_actionSave_2_triggered()
{
    ui->widget->saveData();
}

void MainWindow::on_actionLoad_2_triggered()
{
    ui->widget->loadData();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About"), tr("Danylo Lizanets © 2013"));
}


void MainWindow::on_actionBounds_triggered(bool checked)
{
    if(checked){
        ui->groupBox->show();
    }else{
        ui->groupBox->hide();
    }
}

void MainWindow::getImage(Image im)
{
    ui->thresh_sb->setValue(im.threshold);
    ui->pressure_l->setNum(im.sum);
    ui->left_l->setNum(static_cast<int>(im.bound_counter[2]));
    ui->top_l->setNum(static_cast<int>(im.bound_counter[1]));
    ui->right_l->setNum(static_cast<int>(im.bound_counter[0]));
    ui->bottom_l->setNum(static_cast<int>(im.bound_counter[3]));
    ui->ave_l->setNum(static_cast<int>((im.bound_counter[2]+im.bound_counter[1]+im.bound_counter[0]+im.bound_counter[3])/4));
    ui->sum_l->setNum(static_cast<int>(im.bound_counter[2]+im.bound_counter[1]+im.bound_counter[0]+im.bound_counter[3]));
}
