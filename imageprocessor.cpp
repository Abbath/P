#include "imageprocessor.hpp"

/*!
 * \brief ImageProcessor::ImageProcessor
 * \param parent
 */
ImageProcessor::ImageProcessor(QObject *parent) : QObject(parent)
{
    
}

/*!
 * \brief ImageProcessor::~ImageProcessor
 */
ImageProcessor::~ImageProcessor()
{
    
}

/*!
 * \brief Processor::sharpen
 * \param im
 * \return
 */
QImage ImageProcessor::sharpen(const QImage& im)
{
    QImage image = im;
    QImage oldImage = im;
    int kernel[3][3] = { { 0, -1, 0 },
                         { -1, 5, -1 },
                         { 0, -1, 0 } };
    /* int kernel  [5][5] ={
                        {0,0,-1,0,0},
                        {0,-1,-2,-1,0},
                        {-1,-2,20,-2,-1},
                        {0,-1,-2,-1,0},
                        {0,0,-1,0,0}};*/
    int kernelSize = 3;
    int sumKernel = 1;
    int r, g, b;
    QColor color;
    for (int x = kernelSize / 2; x < image.width() - (kernelSize / 2); x++) {
        for (int y = kernelSize / 2; y < image.height() - (kernelSize / 2); y++) {
            r = 0;
            g = 0;
            b = 0;
            for (int i = -kernelSize / 2; i <= kernelSize / 2; i++) {
                for (int j = -kernelSize / 2; j <= kernelSize / 2; j++) {
                    color = QColor(oldImage.pixel(x + i, y + j));
                    r += color.red() * kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                    g += color.green() * kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                    b += color.blue() * kernel[kernelSize / 2 + i][kernelSize / 2 + j];
                }
            }
            r = qBound(0, r / sumKernel, 255);
            g = qBound(0, g / sumKernel, 255);
            b = qBound(0, b / sumKernel, 255);
            image.setPixel(x, y, qRgb(r, g, b));
        }
    }
    return image;
}

/*!
 * \brief ImageProcessor::searchTheLight
 * \param im
 * \param rect
 * \param tre
 */
unsigned ImageProcessor::searchTheLight(const QImage& im, QRect rect, unsigned int tre)
{
    rect = rect.normalized();
    QImage image = sharpen(im);
    unsigned counter = 0;
    int maxi=rect.left(), mini = rect.right(), maxj=rect.top(), minj = rect.bottom();
    for (auto i = rect.left() + 1; i != rect.right(); ++i) {
        for (auto j = rect.top() + 1; j != rect.bottom(); ++j) {
            if(i < image.width() && j < image.height()){
                if (static_cast<unsigned>(qGray(image.pixel(i, j))) >= tre) {
                    counter++;
                    if(i > maxi) maxi = i;
                    if(i < mini) mini = i;
                    if(j > maxj) maxj = j;
                    if(j < minj) minj = j;
                }
            }
        }
    }
    //qDebug() << rect.left() << rect.right() << rect.top() << rect.bottom();
    //qDebug() << mini << maxi << minj << maxj << "\n";
    return counter;
}

/*!
 * \brief ImageProcessor::align
 * \param image
 * \param sq
 * \return 
 */
QImage ImageProcessor::align(QImage image, const Square& sq)
{
    QMatrix matrix;
    matrix.shear(4 * (sq[1].x() - sq[2].x()) / (double)image.width(), 4 * (sq[0].y() - sq[1].y()) / (double)image.height());
    image = image.transformed(matrix);
    return image;
}

std::array<unsigned, 4> ImageProcessor::getLightAmount(QImage image, Config conf, QPair<QPoint, QPoint> origin, unsigned int tre){
    std::array<unsigned, 4> res;
    unsigned x1 = conf.getSquare0()[1].x() - origin.second.x();
    unsigned x2 = image.width() - 1;
    unsigned y1 = conf.getSquare0()[1].y() - origin.second.y();
    unsigned y2 = conf.getSquare0()[2].y() - origin.second.y();
    res[0] = searchTheLight(image, QRect(QPoint(x1, y1), QPoint(x2, y2)), tre);
    x1 = 0;
    x2 = conf.getSquare0()[0].x() - origin.second.x();
    y1 = conf.getSquare0()[0].y() - origin.second.y();
    y2 = conf.getSquare0()[2].y() - origin.second.y();
    res[1] = searchTheLight(image, QRect(QPoint(x1, y1), QPoint(x2, y2)), tre);
    x1 = conf.getSquare0()[0].x() - origin.second.x();
    x2 = conf.getSquare0()[1].x() - origin.second.x();
    y1 = 0;
    y2 = conf.getSquare0()[0].y() - origin.second.y();
    res[2] = searchTheLight(image, QRect(QPoint(x1, y1), QPoint(x2, y2)), tre);
    x1 = conf.getSquare0()[0].x() - origin.second.x();
    x2 = conf.getSquare0()[1].x() - origin.second.x();
    y1 = conf.getSquare0()[2].y() - origin.second.y();
    y2 = image.height() - 1;
    res[3] = searchTheLight(image, QRect(QPoint(x1, y1), QPoint(x2, y2)), tre);
    return res;
}
