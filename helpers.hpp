#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <QtCore>
#include <QtGui>
#include <opencv/cv.h>
#include <cmath>

class Comparator  {
public:
    QPoint M;
    Comparator(QPoint origin) {
        M = origin;
    }
    bool operator()(QPoint o1, QPoint o2) {
        double angle1 = atan2(o1.y() - M.y(), o1.x() - M.x());
        double angle2 = atan2(o2.y() - M.y(), o2.x() - M.x());
        return (angle1 < angle2);
    }
};

enum Modes{ ISO, TOP, LEFT, BOTTOM, RIGHT};

struct Point {
    double x,y,z;
};

struct Line{
    double x1,y1,z1;
    double x2,y2,z2;
    int c;
};

struct Config{
    QPoint crop[2];
    QPoint square[3];
    QPoint square0[3];
};

struct Image{
    QImage image;
    QPoint crop[2];
    QPoint square[3];
    Config conf;
    QString fileName;
    unsigned threshold = 128, counter = 0,bound_counter[4] = {0,0,0,0};
    double sums[4] = {0.0};
    double sum = 0.0;
    bool r = false, l = false, cl = false;
};

struct Display{
    Image im;
    QPoint origin[2];
};
#endif // HELPERS_HPP
