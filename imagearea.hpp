#ifndef IMAGEAREA_HPP
#define IMAGEAREA_HPP

#include <QWidget>
#include <QtGui>
#include "helpers.hpp"
#include "processor.hpp"

namespace Ui {
  class ImageArea;
}

/*!
 * \brief The ImageArea class
 */
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

  void setProcessor(Processor * p){ processor = p; }
  void setDisplay(Display dis);
  QImage getImage(){return im.getImage();}
  unsigned getThreshold(){return im.getThreshold();}
  ~ImageArea();

signals:
  void viewUpdated(Display d);

private:
  void Update();

private:
  Ui::ImageArea *ui;
  Processor * processor;
  Image im;
  QPair<QPoint, QPoint> origin = { {0,0}, {0,0} };
  double fromGreenToYellow = 25;
  double fromYellowToRed = 35;
  QPoint zoom;
  bool isRectangleDrawn = false;
  bool isZoomed = false;
  bool d3 = false;
};

#endif // IMAGEAREA_HPP
