#include "modelingdisplay.hpp"

ModelingDisplay::ModelingDisplay(QWidget *parent) :
    QWidget(parent)
{
}

void ModelingDisplay::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    if(image.width() > this->width() || image.height() > this->height()){
        painter.drawImage(0,0,image);
    }else{
        painter.drawImage((this->width() - image.width())/2, (this->height() - image.height())/2, image);
    }
    e->accept();
}

void ModelingDisplay::setImage(QImage _image)
{
    image = _image;
    update();
}
