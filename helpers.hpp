#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <QtCore>
#include <QtGui>
#include <opencv/cv.h>
#include <QPointF>

class Comparator
{
public:
  QPoint M;
  Comparator(const QPoint& origin)
  {
    M = origin;
  }

  bool operator()(const QPoint& o1, const QPoint& o2)
  {
    const double angle1 = atan2(o1.y() - M.y(), o1.x() - M.x());
    const double angle2 = atan2(o2.y() - M.y(), o2.x() - M.x());
    return (angle1 < angle2);
  }
};

enum Modes
{
  ISO,
  TOP,
  LEFT,
  BOTTOM,
  RIGHT

};

typedef QVector3D Point;

struct Line
{
  double x1;
  double y1;
  double z1;
  double x2;
  double y2;
  double z2;
  int c;
  Line() : x1( 0.0 ), y1( 0.0 ), z1( 0.0 ), x2( 0.0 ), y2( 0.0 ), z2( 0.0 ), c( 0 ) {}
};

struct Config
{
  QPoint crop[2];
  QPoint square[3];
  QPoint square0[3];
};

struct Image
{
  QImage image;
  QPoint crop[2];
  QPoint square[3];
  Config conf;
  QString fileName;
  unsigned int threshold = 128;
  unsigned int counter = 0;
  unsigned int bound_counter[4] = {0,0,0,0};
  double sums[4] = {0.0};
  double sum = 0.0;
  bool r = false;
  bool l = false;
  bool cl = false;
};

struct Display
{
  Image im;
  QPoint origin[2];
};


#endif // HELPERS_HPP
