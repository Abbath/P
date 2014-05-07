#ifndef IMAGEAREA_HPP
#define IMAGEAREA_HPP

#include <QWidget>
#include <QtGui>
#include <helpers.hpp>
#include <processor.hpp>

namespace Ui {
  class ImageArea;
}

class ImageArea : public QWidget
{
  Q_OBJECT
public:
  explicit ImageArea(QWidget *parent = 0);
  void paintEvent(QPaintEvent *e);
  void mousePressEvent(QMouseEvent *e);
  void mouseMoveEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);
  void wheelEvent(QWheelEvent *e);

  void setProcessor(Processor * p){ proc = p; }
  void setDisplay(Display dis);
  QImage getImage(){return im.image;}
  unsigned getThreshold(){return im.threshold;}
  ~ImageArea();

signals:
  void viewUpdated(Display d);

private:
  void Update();

private:
  Ui::ImageArea *ui;
  Processor * proc;
  Image im;
  QPoint origin[2] = { {0,0}, {0,0} };
  double sum = 0;
  double GY = 25;
  double YR = 35;
  QPoint zoom;
  bool rect = false;
  bool zoom_b = false;
  bool d3 = false;
  bool vid = false;
};

#endif // IMAGEAREA_HPP
