#include "dispatcher.hpp"

Dispatcher::Dispatcher(QObject *parent) :
    QObject(parent)
{
    
}

void Dispatcher::openImage()
{
    ip->openImage(QFileDialog::getOpenFileNames( ia, tr("Open data file"), "", tr("Image files (*.bmp)")));
}

void Dispatcher::openVideo()
{
    ip->openVideo(QFileDialog::getOpenFileName( ia, tr("Open video file"), "", tr("Video files (*.avi)")));
}

void Dispatcher::saveImage()
{
    ip->saveImage(QFileDialog::getSaveFileName(ia,tr("Save image"), "", tr("Image files (*.bmp)")));
}

void Dispatcher::saveConf()
{
    ip->saveConf(QFileDialog::getSaveFileName(ia, tr("Save config"), "", tr("Config files (*.conf)")),false);
}

void Dispatcher::loadConf()
{
    QFileDialog::getOpenFileName(ia, tr("Open config"), "", tr("Config files (*.conf)"));
}

void Dispatcher::calibrate()
{
    ip->calibrate(QFileDialog::getOpenFileName(ia, tr("Open config"), "", tr("Config files (*.conf)")),QFileDialog::getOpenFileNames(ia,tr("Open images"), "", tr("Images (*.bmp)")));
}

void Dispatcher::saveResults()
{
    ip->saveResults(QFileDialog::getSaveFileName(ia,tr("Save results"), "", tr("Data (*.dat)")));
}

void Dispatcher::loadData()
{
    ip->loadData(QFileDialog::getOpenFileName(ia,tr("Load data"), "", tr("Data (*.dat)")));
}

void Dispatcher::saveData()
{
    ip->saveData(QFileDialog::getSaveFileName(ia,tr("Save data"), "", tr("Data (*.dat)")));
}
