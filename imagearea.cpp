#include "imagearea.hpp"
#include "imagestorage.hpp"
#include "ui_imagearea.h"

/*!
 * \brief ImageArea::ImageArea
 * \param parent
 */
ImageArea::ImageArea(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ImageArea)
{
    ui->setupUi(this);
}

/*!
 * \brief ImageArea::Update
 */
void ImageArea::Update()
{
    Display dis;
    dis.im = im();
    dis.origin = origin;
    emit viewUpdated(dis);
}

Image &ImageArea::im()
{
    return ImageStorage::getInstance().getCurrImageRef();
}

/*!
 * \brief ImageArea::paintEvent
 * \param e
 */
void ImageArea::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    painter.drawImage(origin.second.x(), origin.second.y(), im().getImage());
    if (im().getCounter() > 0) {
        for (unsigned i = 0; i < im().getCounter(); ++i) {
            painter.setPen(Qt::red);
            int n = im().getSi();
            painter.drawLine(im().getSquare(n)[i].x() - origin.first.x() - 4, im().getSquare(n)[i].y() - origin.first.y(), im().getSquare(n)[i].x() - origin.first.x() + 4, im().getSquare(n)[i].y() - origin.first.y());
            painter.drawLine(im().getSquare(n)[i].x() - origin.first.x(), im().getSquare(n)[i].y() - origin.first.y() - 4, im().getSquare(n)[i].x() - origin.first.x(), im().getSquare(n)[i].y() - origin.first.y() + 4);
            painter.drawText(im().getSquare(n)[i].x() - origin.first.x() + 3, im().getSquare(n)[i].y() - origin.first.y() - 3, QString::number(i + 1));
            painter.setPen(Qt::white);
        }
    }
    if (isRectangleDrawn) {
        painter.setPen(Qt::green);
        painter.drawRect(im().getCrop());
        painter.setPen(Qt::white);
    } else {
        if (isZoomed) {
            QImage zoomed = im().getImage().copy(zoom.x() - origin.second.x() - 10, zoom.y() - origin.second.y() - 10, 20, 20).scaled(41, 41);
            zoomed.setPixel(20, 20, qRgb(0, 255, 0));
            painter.drawImage(zoom.x(), zoom.y(), zoomed);
        }
    }
    Update();
    if (im().getPressure() <= fromGreenToYellow) {
        painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
        painter.setPen(Qt::green);
    } else if (im().getPressure() > fromGreenToYellow && im().getPressure() < fromYellowToRed) {
        painter.setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
        painter.setPen(Qt::yellow);
    } else if (im().getPressure() >= fromYellowToRed) {
        painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        painter.setPen(Qt::red);
    }
    painter.setFont(QFont("Ubuntu", 50));
    painter.drawText(30, this->height() - 10, QString::number(im().getPressure()) + " kPa");
    painter.drawEllipse(this->width() - 100, this->height() - 100, 80, 80);
//    painter.drawEllipse(13, 61, 335-13, 261-61);
//    painter.drawLine(QPoint(13, 0), QPoint(13,im.getImage().height()-1));
//    painter.drawLine(QPoint(335, 0), QPoint(335, im.getImage().height()-1));
//    painter.drawLine(QPoint(0, 61), QPoint(im.getImage().width()-1,61));
//    painter.drawLine(QPoint(0, 261), QPoint(im.getImage().width()-1, 261));
    e->accept();
}

/*!
 * \brief ImageArea::mousePressEvent
 * \param e
 */
void ImageArea::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::RightButton && !im().isImageNull()) {
        isRectangleDrawn = true;
        im().getCropRef().setTopLeft(QPoint(e->x(), e->y()));
        update();
    }
    if (e->button() == Qt::LeftButton && !im().isImageNull()) {
        isZoomed = true;
        zoom.setX(e->x());
        zoom.setY(e->y());
        update();
    }
    if (e->button() == Qt::MiddleButton && !im().isImageNull()) {
        origin.first.setX(origin.first.x() + e->x());
        origin.first.setY(origin.first.y() + e->y());
        Update();
    }
}

/*!
 * \brief ImageArea::mouseMoveEvent
 * \param e
 */
void ImageArea::mouseMoveEvent(QMouseEvent* e)
{
    if (!im().isImageNull()) {
        if (isRectangleDrawn) {
            im().getCropRef().setBottomRight(QPoint(e->x(), e->y()));
        } else {
            zoom.setX(e->x());
            zoom.setY(e->y());
        }
    }
    if (e->buttons() & Qt::MiddleButton && !im().isImageNull()) {
        origin.second.setX(e->x() - origin.first.x());
        origin.second.setY(e->y() - origin.first.y());
    }
    update();
}

/*!
 * \brief ImageArea::mouseReleaseEvent
 * \param e
 */
void ImageArea::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::RightButton && !im().isImageNull()) {
        im().getCropRef().setBottomRight(QPoint(e->x(), e->y()));
        im().setCrop(im().getCrop().normalized().translated(origin.first.x(), origin.first.y()));
        im().cropImage();
        origin.first.setX(0);
        origin.first.setY(0);
        origin.second.setX(0);
        origin.second.setY(0);
        isRectangleDrawn = false;
    }
    if (e->button() == Qt::LeftButton && !im().isImageNull()) {
        if (im().getCounter() < 3) {
            im().getSquareRef(im().getSi())[im().getCounter()].setX(e->x() + origin.first.x());
            im().getSquareRef(im().getSi())[im().getCounter()].setY(e->y() + origin.first.y());
            im().incrementCounter();
        } else {
            im().resetCounter();
        }
        isZoomed = false;
    }
    if (e->button() == Qt::MiddleButton && !im().isImageNull()) {
        origin.first.setX(origin.first.x() - e->x());
        origin.first.setY(origin.first.y() - e->y());
    }
    update();
}

/*!
 * \brief ImageArea::wheelEvent
 * \param e
 */
void ImageArea::wheelEvent(QWheelEvent* e)
{
    qint32 newThreshold = im().getThreshold() + e->delta() / 80;
    if (newThreshold > 255)
        newThreshold = 255;
    if (newThreshold < 0)
        newThreshold = 0;
    im().setThreshold(quint8(newThreshold));
    Update();
}

/*!
 * \brief ImageArea::setDisplay
 * \param dis
 */
void ImageArea::setDisplay(Display dis)
{
    im() = dis.im;
    origin = dis.origin;
    update();
}

/*!
 * \brief ImageArea::~ImageArea
 */
ImageArea::~ImageArea()
{
    delete ui;
}
