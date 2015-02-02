#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <QtCore>
#include <QtGui>
#include <QPointF>
#include <array>

enum class DataType{PIXELS, PRESSURE}; 

typedef std::array<QPoint, 3> Square;

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

//typedef QVector3D Point;

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
  QRect crop;
  //QPoint square[3];
  Square square;
  Square square0;
  //QPoint square0[3];
};

class Image
{

public:
  double pressure = 0.0l;
  bool isProcessed = false;
  bool isLoaded = false;
  void resetCounter(){ counter = 0u; }
  void setFullCounter(){ counter = 3u; }
  bool isCounterFull(){ return counter == 3u; }
  unsigned getSum() const { return std::accumulate(bound_counter.begin(), bound_counter.end(), 0); }
  const QImage& getImageRef() const { return image; }
  QRect& getCropRef(){ return crop; }
  std::array<unsigned, 4>& getBoundCounterRef(){ return bound_counter; }
  QImage getImage() const {return image; }
  unsigned getThreshold() const { return threshold; }
  std::array<unsigned, 4> getBoundCounter() const { return bound_counter; }
  QString getFileName() const { return fileName; }
  QRect getCrop() const { return crop; }
  Square& getSquare() { return square; }
  unsigned getCounter() const { return counter; }
  void incrementCounter(){ counter++; }
  void setImage(const QImage& _image){ image = _image; }
  void setFileName(const QString& filename){ fileName = filename; }
  void setCrop(const QRect& rect){ crop = rect; }
  void setSquare(const Square& sq){ square = sq; }
  void setConfig(const Config& conf){ config = conf; }
  void setThreshold(unsigned th){ threshold = th; }
  bool isImageNull(){ return image.isNull(); }
  void cropImage(){ image = image.copy(crop); }
private:
  QImage image;
  QRect crop;
  //QPoint square[3];
  Square square;
  Config config;
  QString fileName;
  unsigned int threshold = 128u;
  unsigned int counter = 0u;
  //unsigned int bound_counter[4] = {0, 0, 0, 0};
  std::array<unsigned, 4> bound_counter;
};

struct Display
{
  Image im;
  QPair<QPoint, QPoint> origin;
};


#endif // HELPERS_HPP
