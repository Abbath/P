#ifndef CONVERTER_HPP
#define CONVERTER_HPP
#include <QtCore>
#include <QtGui>
#include <helpers.hpp>

class Converter
{
public:
    Converter();
    void rotate(QVector<Line> &l, double angle_x =0, double angle_y =0, double angle_z =0);
    QVector<Line> convert(QImage &image, Modes mode);
};

#endif // CONVERTER_HPP
