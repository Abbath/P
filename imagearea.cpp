#include "imagearea.hpp"
#include "ui_imagearea.h"

ImageArea::ImageArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageArea)
{
    ui->setupUi(this);
}

void ImageArea::update()
{
    Display dis;
    dis.im = im;
    dis.origin[0] = origin[0];
    dis.origin[1] = origin[1];
    emit viewUpdated(dis);
}

void ImageArea::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawImage(origin[1].x(), origin[1].y(), im.image);
    if(im.counter > 0){
        for(unsigned i = 0; i < im.counter; ++i){
            painter.setPen(Qt::red);
            painter.drawLine(im.square[i].x() - 4, im.square[i].y(), im.square[i].x() + 4, im.square[i].y());
            painter.drawLine(im.square[i].x(), im.square[i].y() - 4, im.square[i].x(), im.square[i].y() + 4);
            painter.drawText(im.square[i].x() + 3, im.square[i].y() - 3, QString::number(i+1));
            painter.setPen(Qt::white);
        }
    }
    if(rect){
        painter.setPen(Qt::green);
        painter.drawRect(im.crop[0].x(), im.crop[0].y(), im.crop[1].x() - im.crop[0].x(), im.crop[1].y() - im.crop[0].y());
        painter.setPen(Qt::white);
    }else{
        if(zoom_b){
            QImage zoomed = im.image.copy(zoom.x() - origin[1].x() - 10, zoom.y() - origin[1].y() - 10, 20, 20).scaled(41, 41);
            zoomed.setPixel(20, 20, qRgb(0, 255, 0));
            painter.drawImage(zoom.x(), zoom.y(), zoomed);
        }
    }
    update();
    if(im.sum <= GY){
        painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
        painter.setPen(Qt::green);
    }else if(im.sum > GY && im.sum < YR){
        painter.setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
        painter.setPen(Qt::yellow);
    }else if(im.sum >= YR){
        painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        painter.setPen(Qt::red);
    }
    painter.drawEllipse(this->width() - 100, this->height() - 100, 80, 80);
    e->accept();
}

void ImageArea::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton && !im.image.isNull() ){
        rect = true;
        im.crop[0].setX(e->x());
        im.crop[0].setY(e->y());
        repaint();
    }
    if(e->button() == Qt::LeftButton && !im.image.isNull()){
        zoom_b = true;
        zoom.setX(e->x());
        zoom.setY(e->y());
        repaint();
    }
    if(e->button() == Qt::MiddleButton && !im.image.isNull()){
        origin[0].setX(origin[0].x() + e->x());
        origin[0].setY(origin[0].y() + e->y());
       update();
    }
}

void ImageArea::mouseMoveEvent(QMouseEvent *e)
{
    if(!im.image.isNull()){
        if(rect){
            im.crop[1].setX(e->x());
            im.crop[1].setY(e->y());

        }else{
            zoom.setX(e->x());
            zoom.setY(e->y());
        }
    }if(e->buttons() & Qt::MiddleButton && !im.image.isNull()){
        origin[1].setX(e->x() - origin[0].x());
        origin[1].setY(e->y() - origin[0].y());
    }
    repaint();
}

void ImageArea::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton && !im.image.isNull()){
        if(im.crop[0].x() > im.crop[1].x()){
            int tmp = im.crop[0].x();
            im.crop[0].setX(im.crop[1].x());
            im.crop[1].setX(tmp);
        }
        if(im.crop[0].y() > im.crop[1].y()){
            int tmp = im.crop[0].y();
            im.crop[0].setY(im.crop[1].y());
            im.crop[1].setY(tmp);
        }
        im.image = im.image.copy(im.crop[0].x()- origin[1].x(),im.crop[0].y()-origin[1].y(),im.crop[1].x() - im.crop[0].x(),im.crop[1].y() - im.crop[0].y());
        origin[0].setX(0);
        origin[0].setY(0);
        origin[1].setX(0);
        origin[1].setY(0);
        rect = false;
    }
    if(e->button() == Qt::LeftButton && !im.image.isNull()){
        if(im.counter < 3){
            im.square[im.counter].setX(e->x());
            im.square[im.counter].setY(e->y());
            im.counter++;
        }else{
            im.counter = 0;
        }
        zoom_b = false;
    }
    if(e->button() == Qt::MiddleButton && !im.image.isNull()){
        origin[0].setX(origin[0].x()- e->x());
        origin[0].setY(origin[0].y()- e->y());
    }
    repaint();
}

void ImageArea::wheelEvent(QWheelEvent *e)
{
    qint32 a = im.threshold + e->delta()/80;
    if( a > 255 ) a = 255;
    if( a < 0 ) a = 0;
    im.threshold = quint8(a);
    update();
}

void ImageArea::setDisplay(Display dis)
{
    im = dis.im;
    repaint();
}

ImageArea::~ImageArea()
{
    delete ui;
}
