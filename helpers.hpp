#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <QtCore>

enum Modes{ ISO, TOP, LEFT, BOTTOM, RIGHT};

struct Point {
    double x,y,z;
};

struct Line{
double x1,y1,z1;
double x2,y2,z2;
int c;
};

#endif // HELPERS_HPP
