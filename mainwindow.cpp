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
    ui->imageArea->setDisplay(dis);
}

void MainWindow::on_actionOpen_Image_s_triggered()
{
    QStringList names = QFileDialog::getOpenFileNames(this, "Open Image(s)",".","Images (*.bmp)");
    emit openImage(names);
}
