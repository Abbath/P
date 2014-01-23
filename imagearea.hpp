#ifndef IMAGEAREA_HPP
#define IMAGEAREA_HPP

#include <QWidget>
#include <QtGui>
#include <helpers.hpp>

namespace Ui {
class ImageArea;
}

class ImageArea : public QWidget
{
    Q_OBJECT
    Image im;
    QPoint origin[2] = {{0,0},{0,0}};
    double sum = 0,GY=25,YR=35;
    QPoint zoom;
    bool rect = false,zoom_b = false,d3 = false, vid = false;
    void update();
public:
    explicit ImageArea(QWidget *parent = 0);
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    
    void setDisplay(Display dis);
    ~ImageArea();
signals:
    void viewUpdated(Display d);
private:
    Ui::ImageArea *ui;
};

#endif // IMAGEAREA_HPP
