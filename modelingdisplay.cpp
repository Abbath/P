#include "modelingdisplay.hpp"

/*!
 * \brief ModelingDisplay::ModelingDisplay
 * \param parent
 */
ModelingDisplay::ModelingDisplay(QWidget *parent) :
    QWidget(parent)
{}

/*!
 * \brief ModelingDisplay::paintEvent
 * \param e
 */
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

/*!
 * \brief ModelingDisplay::setImage
 * \param _image
 */
void ModelingDisplay::setImage(QImage _image)
{
    image = _image;
    update();
}
