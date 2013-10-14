#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <QtCore>
#include <opencv/cv.h>

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

static QImage IplImage2QImage(const IplImage *iplImage)
{
       int height = iplImage->height;
       int width = iplImage->width;

       const uchar *qImageBuffer =(const uchar*)iplImage->imageData;
       QImage img(qImageBuffer, width, height, QImage::Format_RGB888);
       return img.rgbSwapped();
}
#endif // HELPERS_HPP
