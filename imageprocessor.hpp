#ifndef IMAGEPROCESSOR_HPP
#define IMAGEPROCESSOR_HPP

#include <QObject>
#include "helpers.hpp"

/*!
 * \brief The ImageProcessor class
 */
class ImageProcessor : public QObject
{
    Q_OBJECT
public:
    explicit ImageProcessor(QObject *parent = 0);
    ~ImageProcessor();
    std::array<unsigned, 4> getLightAmount(QImage image, Config conf, QPair<QPoint, QPoint> origin, unsigned int tre);
    QImage align(QImage image, const Square &sq);
signals:
    
public slots:
    
private:
    QImage sharpen(const QImage &im);
    unsigned searchTheLight(const QImage &im, QRect rect, unsigned int tre);
};

#endif // IMAGEPROCESSOR_HPP
