#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Update(Display dis)
{
    qDebug() << "Why i'm not hawk? Why i'm not flying?\n";
    ui->imageArea->setDisplay(dis);
}

void MainWindow::Error(QString a, QString b)
{
    QMessageBox::warning(this,a,b);
}

void MainWindow::on_actionOpen_Image_s_triggered()
{
    QStringList names = QFileDialog::getOpenFileNames(this, "Open Image(s)",".","Images (*.bmp)");
    emit openImage(names);
}

void MainWindow::on_actionAlign_triggered()
{
    emit align();
}

void MainWindow::on_actionRun_triggered()
{
    emit run();
}

void MainWindow::on_actionReset_triggered()
{
    emit reset();
}

void MainWindow::on_actionAutorun_triggered()
{
    emit autorun();
}

void MainWindow::on_actionPrev_triggered()
{
    emit prev();
}

void MainWindow::on_actionNext_triggered()
{
    emit next();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionLoad_triggered()
{
    emit loadConf(QFileDialog::getOpenFileName(this, tr("Open config"), "", tr("Config files (*.conf)")));
}

void MainWindow::on_actionSave_triggered()
{
    emit saveConf(QFileDialog::getSaveFileName(this, tr("Save config"), "", tr("Config files (*.conf)")),false);
}

void MainWindow::on_actionLoad_2_triggered()
{
    emit loadData(QFileDialog::getOpenFileName(this,tr("Load data"), "", tr("Data (*.dat)")));
}

void MainWindow::on_actionSave_2_triggered()
{
    emit saveData(QFileDialog::getSaveFileName(this,tr("Save data"), "", tr("Data (*.dat)")));
}

void MainWindow::on_actionSave_as_Default_triggered()
{
    emit saveConf(QString("default.conf"),true);
}
