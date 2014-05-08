#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <QtCore>
#include <QtGui>
#include <opencv/cv.h>
#include <QPointF>

/*!
 * \brief The Config struct
 */
struct Config
{
  QRect crop;
  QPoint square[3];
  QPoint square0[3];
};

/*!
 * \brief The Image struct
 */
struct Image
{
  QImage image;
  //QPoint crop[2];
  QRect crop;
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

/*!
 * \brief The Display struct
 */
struct Display
{
  Image im;
  QPoint origin[2];
};


#endif // HELPERS_HPP
