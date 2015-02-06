#include "modelingwindow.hpp"
#include "ui_modelingwindow.h"
#include "modelingcore.hpp"

#include <QMessageBox>
#include <QFileDialog>
#include <QtConcurrent/QtConcurrent>

ModelingWindow::ModelingWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ModelingWindow)
{
    ui->setupUi(this);
    ia = new ModelingDisplay(this);
    p = new ModelingCore(this);
    ia->resize(423,423);
    ia->setMinimumSize(423,423);
    ui->scrollArea->setWidget(ia);
    ui->scrollArea->setWidgetResizable(true);
    ui->label_11->hide();
    ui->doubleSpinBox_10->hide();
    ui->label_4->hide();
    ui->doubleSpinBox_3->hide();
    ui->label_14->hide();
    ui->doubleSpinBox_13->hide();
    ui->progressBar->setFormat("Raytracing progress: %p%");
    ui->progressBar->hide();
    label = new QLabel(this);
    ui->statusBar->addWidget(label);
    readSettings();
    if(ui->doubleSpinBox_13->value() == 3.0){
        ui->doubleSpinBox_13->setValue(7.0);
    }
    if(ui->doubleSpinBox_10->value() == 1.0){
        ui->doubleSpinBox_10->setValue(25.0);
    }
    label->setText("Ready.");
//    ui->pushButton->hide();
//    ui->pushButton_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->pushButton_5->hide();
    QObject::connect(p,SIGNAL(lil(int)),this, SLOT(lil(int)), Qt::QueuedConnection);
    QObject::connect(p,SIGNAL(saved()),this, SLOT(saved()), Qt::QueuedConnection);
    QObject::connect(p,SIGNAL(sendImage(QImage)), this, SLOT(setImage(QImage)));
    QObject::connect(p, SIGNAL(error(QString)), this, SLOT(error(QString)));
}

ModelingWindow::~ModelingWindow()
{
    writeSettings();
    delete ui;
}

void ModelingWindow::setImage(QImage image)
{
//    ui->pushButton_2->setText("Start");
//    ui->actionStart->setText("Start");
    ui->progressBar->hide();
    ia->setImage(image);
    label->setText("Done.");
    //p->Stop();
}

void ModelingWindow::writeSettings(){
    QSettings settings("CAD", "ANN");
    settings.beginGroup("Set");
    settings.setValue("ds", ui->doubleSpinBox->value());
    settings.setValue("ms", ui->doubleSpinBox_2->value());
    settings.setValue("pres", ui->doubleSpinBox_6->value());
    settings.setValue("thickness", ui->doubleSpinBox_4->value());
    settings.setValue("sh", ui->doubleSpinBox_5->value());
    settings.setValue("rn", ui->spinBox->value());
    settings.setValue("wl", ui->doubleSpinBox_7->value());
    settings.setValue("xa", ui->doubleSpinBox_8->value());
    settings.setValue("ya", ui->doubleSpinBox_9->value());
    settings.setValue("cxa", ui->doubleSpinBox_11->value());
    settings.setValue("cya", ui->doubleSpinBox_12->value());
    settings.setValue("magic", ui->doubleSpinBox_10->value());
    settings.setValue("bem", ui->doubleSpinBox_13->value());
    settings.endGroup();
    settings.beginGroup("MSet");
    settings.setValue("ym", ui->doubleSpinBox_14->value());
    settings.setValue("pr", ui->doubleSpinBox_15->value());
    settings.endGroup();
    settings.beginGroup("HSet");
    settings.setValue("rows", ui->spinBox_2->value());
    settings.setValue("hs", ui->spinBox_3->value());
    settings.setValue("ss", ui->spinBox_4->value());    
    settings.endGroup();
}

void ModelingWindow::readSettings(){
    QSettings settings("CAD", "ANN");
    ui->doubleSpinBox->setValue(settings.value("Set/ds").toDouble());
    ui->doubleSpinBox_2->setValue(settings.value("Set/ms").toDouble());
    ui->doubleSpinBox_6->setValue(settings.value("Set/pres").toDouble());
    ui->doubleSpinBox_4->setValue(settings.value("Set/thickness").toDouble());
    ui->doubleSpinBox_5->setValue(settings.value("Set/sh").toDouble());
    ui->spinBox->setValue(settings.value("Set/rn").toInt());
    ui->doubleSpinBox_7->setValue(settings.value("Set/wl").toDouble());
    ui->doubleSpinBox_8->setValue(settings.value("Set/xa").toDouble());
    ui->doubleSpinBox_9->setValue(settings.value("Set/ya").toDouble());
    ui->doubleSpinBox_11->setValue(settings.value("Set/cxa").toDouble());
    ui->doubleSpinBox_12->setValue(settings.value("Set/cya").toDouble());
    ui->doubleSpinBox_10->setValue(settings.value("Set/magic").toDouble());
    ui->doubleSpinBox_13->setValue(settings.value("Set/bem").toDouble());
    ui->doubleSpinBox_14->setValue(settings.value("MSet/ym").toDouble());
    ui->doubleSpinBox_15->setValue(settings.value("MSet/pr").toDouble());
    ui->spinBox_2->setValue(settings.value("HSet/rows").toInt());
    ui->spinBox_3->setValue(settings.value("HSet/hs").toInt());
    ui->spinBox_4->setValue(settings.value("HSet/ss").toInt());
}

void ModelingWindow::on_pushButton_3_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save image","","Images (*.png *.bmp)");
    ui->centralwidget->setDisabled(true);
    QtConcurrent::run(p,&ModelingCore::saveImage, filename);
}

void ModelingWindow::lil(int i)
{
    ui->progressBar->setValue(i);
    update();
}

void ModelingWindow::saved()
{
    ui->centralwidget->setEnabled(true);
    QMessageBox::information(this,"Done!", "Image has been saved successfully!");
}

void ModelingWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,"About","Ray tracer for pressure sensor v0.6\nDanylo Lizanets © 2014");
}

void ModelingWindow::error(QString s)
{
    QMessageBox::critical(this, "Error", s);
}

void ModelingWindow::keyPressEvent(QKeyEvent *e)
{
    e->accept();
    if(e->key() == Qt::Key_M && e->modifiers() & Qt::ShiftModifier){
        if(ui->label_14->isHidden()){
            ui->label_14->show();
            ui->doubleSpinBox_13->show();
        }else{
            ui->label_14->hide();
            ui->doubleSpinBox_13->hide();
        }
    }else if(e->modifiers() & Qt::ControlModifier){
        if(e->key() == Qt::Key_M){
            if(ui->doubleSpinBox_10->isHidden()){
                ui->doubleSpinBox_10->show();
            }else{
                ui->doubleSpinBox_10->hide();
            }
            if(ui->label_11->isHidden() ){
                ui->label_11->show();
            }else{
                ui->label_11->hide();
            }
        }
        if(e->key() == Qt::Key_R){
            on_actionStart_triggered();
        }
        if(e->key() == Qt::Key_S){
            on_pushButton_3_clicked();
        }
        if(e->key() == Qt::Key_O){
            on_actionLoad_data_triggered();
        }
    }else if(e->key() == Qt::Key_M && e->modifiers() == Qt::AltModifier){
        if(ui->groupBox_2->isHidden()){
            ui->groupBox_2->show();
        }else{
            ui->groupBox_2->hide();
        }
    }
}

void ModelingWindow::keyReleaseEvent(QKeyEvent *e)
{
    e->accept();
}

void ModelingWindow::saveConfig()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save config", ".", "Config files (*.conf)");
    if(filename.isEmpty()) return;    
    QFile file(filename);
    if(!file.open(QFile::WriteOnly)){
        error("Can not write to file: "+filename);
    }
    QTextStream str(&file);
    str << ui->doubleSpinBox->value() << "\n"
        <<ui->doubleSpinBox_2->value() << "\n" 
       << ui->doubleSpinBox_6->value() << "\n"
       << ui->doubleSpinBox_4->value() << "\n"
       << ui->doubleSpinBox_5->value() << "\n"
       << ui->spinBox->value() << "\n"
       << ui->doubleSpinBox_7->value() << "\n"
       << ui->doubleSpinBox_8->value() << "\n"
       << ui->doubleSpinBox_9->value() << "\n"
       << ui->doubleSpinBox_11->value() << "\n"
       << ui->doubleSpinBox_12->value() << "\n"
       << ui->doubleSpinBox_10->value() << "\n"
       << ui->doubleSpinBox_13->value() << "\n"
       << ui->doubleSpinBox_14->value() << "\n"
       << ui->doubleSpinBox_15->value() << "\n" 
       << ui->spinBox_2->value() << "\n" 
       << ui->spinBox_3->value() << "\n" 
       << ui->spinBox_4->value() << "\n";
}

void ModelingWindow::loadConfig()
{
    QString filename = QFileDialog::getOpenFileName(this, "Load config", ".", "Config files (*.conf)");
    if(filename.isEmpty()) return;
    QFile file(filename);
    if(!file.open(QFile::ReadOnly)){
        error("Can not read file: "+filename);
    }
    QTextStream str(&file);
    ui->doubleSpinBox->setValue(str.readLine().toDouble());
    ui->doubleSpinBox_2->setValue(str.readLine().toDouble());
    ui->doubleSpinBox_6->setValue(str.readLine().toDouble());
    ui->doubleSpinBox_4->setValue(str.readLine().toDouble());
    ui->doubleSpinBox_5->setValue(str.readLine().toDouble());
    ui->spinBox->setValue(str.readLine().toInt());
    ui->doubleSpinBox_7->setValue(str.readLine().toDouble());
    ui->doubleSpinBox_8->setValue(str.readLine().toDouble());
    ui->doubleSpinBox_9->setValue(str.readLine().toDouble());
    ui->doubleSpinBox_11->setValue(str.readLine().toDouble());
    ui->doubleSpinBox_12->setValue(str.readLine().toDouble());
    ui->doubleSpinBox_10->setValue(str.readLine().toDouble());
    ui->doubleSpinBox_13->setValue(str.readLine().toDouble());
    ui->doubleSpinBox_14->setValue(str.readLine().toDouble());
    ui->doubleSpinBox_15->setValue(str.readLine().toDouble());
    ui->spinBox_2->setValue(str.readLine().toInt());
    ui->spinBox_3->setValue(str.readLine().toInt());
    ui->spinBox_4->setValue(str.readLine().toInt());
}


void ModelingWindow::on_pushButton_4_clicked()
{
    saveConfig();
}

void ModelingWindow::on_pushButton_5_clicked()
{
    loadConfig();
}

void ModelingWindow::on_actionStart_triggered()
{
    //p->Stop();
    if(ui->actionStart->text() == QString("Stop")){
        //ui->pushButton_2->setText("Start");
        ui->actionStart->setText("Start");        
        ui->progressBar->hide();
        p->Stop();
        label->setText("Stopped.");
    }else{
        p->setDie_size(ui->doubleSpinBox->value()*1e-3);
        p->setMem_size(ui->doubleSpinBox_2->value()*1e-3);
        p->setMargin(ui->doubleSpinBox_3->value()*1e-3);
        p->setPressure(ui->doubleSpinBox_6->value()*1000/ui->doubleSpinBox_10->value());
        p->setThickness(ui->doubleSpinBox_4->value()*1e-6);
        p->setSpacer_height(ui->doubleSpinBox_5->value()*1e-3);
        p->setRay_number(ui->spinBox->value());
        p->setWavelength(ui->doubleSpinBox_7->value());
        p->setX_angle(-tan(ui->doubleSpinBox_8->value() / 57.3));
        p->setY_angle(-tan(ui->doubleSpinBox_9->value() / 57.3));
        p->setCam_angle_x(ui->doubleSpinBox_11->value() / 57.3);
        p->setCam_angle_y(ui->doubleSpinBox_12->value() / 57.3);
        p->setN(ui->doubleSpinBox_13->value());
        p->setYm(ui->doubleSpinBox_14->value() * 1e9);
        p->setPr(ui->doubleSpinBox_15->value());
        p->setHoles_rows_number(ui->spinBox_2->value());
        p->setHole_size(ui->spinBox_3->value());
        p->setSpace_size(ui->spinBox_4->value());
        //ui->pushButton_2->setText("Stop");
        ui->actionStart->setText("Stop");        
        ui->progressBar->show();
        label->setText("Processing...");
        QtConcurrent::run(p,&ModelingCore::run);
    }
}

void ModelingWindow::on_actionLoad_data_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Load data","", "Text (*.txt)");
    if(!filename.isEmpty()){
        p->setFilename(filename);
        p->setIs_data_external(true);
    }else{
        p->setIs_data_external(false);
    }
}

void ModelingWindow::on_actionLoad_config_triggered()
{
    on_pushButton_5_clicked();
}

void ModelingWindow::on_actionSave_config_triggered()
{
    on_pushButton_4_clicked();
}

void ModelingWindow::on_actionSave_image_triggered()
{
    on_pushButton_3_clicked();
}

void ModelingWindow::setData(ModelingData data)
{
    ui->doubleSpinBox->setValue(data.getDie_size()*1e3);
    ui->doubleSpinBox_2->setValue(data.getMem_size()*1e3);
    ui->doubleSpinBox_6->setValue(data.getPressure()*ui->doubleSpinBox_10->value()/1000.0);
    ui->doubleSpinBox_4->setValue(data.getMem_thickness()*1e6);
    ui->doubleSpinBox_5->setValue(data.getSpacer_height()*1e3);
    ui->spinBox->setValue(data.getRay_number());
    ui->doubleSpinBox_7->setValue(data.getWavelength());
    ui->doubleSpinBox_8->setValue(data.getX_angle()*57.3);
    ui->doubleSpinBox_9->setValue(data.getY_angle()*57.3);
    ui->doubleSpinBox_11->setValue(data.getCamx()*57.3);
    ui->doubleSpinBox_12->setValue(data.getCamy()*57.3);
    ui->doubleSpinBox_14->setValue(data.getYm()/1e9);
    ui->doubleSpinBox_15->setValue(data.getPr());
    ui->spinBox_2->setValue(data.getRows());
    ui->spinBox_3->setValue(data.getHole_size());
    ui->spinBox_4->setValue(data.getSpace_size());
    p->setData(data);
}
