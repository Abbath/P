#include "processor.hpp"

/*!
 * \brief Processor::Processor
 * \param parent
 */
Processor::Processor(QObject* parent)
    : QObject(parent)
{
    images.resize(1);
}

/*!
 * \brief Processor::leastsquares
 * \param x
 * \param yy
 * \return
 */
std::pair<long double, long double> Processor::leastsquares(const QVector<double>& x, const QVector<double>& yy) const
{
    QVector<double> y = yy;
    for (int i = 0; i < y.size(); ++i) {
        y[i] += 1.0;
    }
    long double A, B, a = 0, b = 0, at = 0, tt = 0, bt = 0, tmp = 0;
    for (int i = 0; i < x.size(); ++i) {
        tmp += x[i] * x[i] * y[i];
    }

    a = tmp;
    tmp = 0;

    for (int i = 0; i < x.size(); ++i) {
        tmp += y[i] * log(y[i]);
    }

    a *= tmp;
    tmp = 0;

    for (int i = 0; i < x.size(); ++i) {
        tmp += y[i] * x[i];
    }

    at = tmp;
    tmp = 0;

    for (int i = 0; i < x.size(); ++i) {
        tmp += y[i] * x[i] * log(y[i]);
    }

    at *= tmp;
    tmp = 0;

    a -= at;

    for (int i = 0; i < x.size(); ++i) {
        tmp += y[i];
    }

    at = tmp;
    tmp = 0;

    for (int i = 0; i < x.size(); ++i) {
        tmp += x[i] * x[i] * y[i];
    }

    tt = at * tmp;
    tmp = 0;

    for (int i = 0; i < x.size(); ++i) {
        tmp += x[i] * y[i];
    }

    tt -= tmp * tmp;
    tmp = 0;

    a /= tt;

    A = exp(a);

    for (int i = 0; i < x.size(); ++i) {
        tmp += y[i];
    }

    b = tmp;
    tmp = 0;

    for (int i = 0; i < x.size(); ++i) {
        tmp += x[i] * y[i] * log(y[i]);
    }

    b *= tmp;
    tmp = 0;

    for (int i = 0; i < x.size(); ++i) {
        tmp += y[i] * x[i];
    }

    bt = tmp;
    tmp = 0;

    for (int i = 0; i < x.size(); ++i) {
        tmp += y[i] * log(y[i]);
    }

    bt *= tmp;
    tmp = 0;

    b -= bt;

    b /= tt;

    B = b;

    return std::make_pair(A, B);
}

/*!
 * \brief Processor::calculate
 * \param res
 * \param pres
 * \param val
 * \return
 */
double Processor::calculate(const QVector<double>& res, const QVector<double>& pres, double val) const
{
    auto p = leastsquares(res, pres);
    qDebug() << (double)p.first << " " << (double)p.second;
    double sum = p.first * exp(p.second * val) - 1.0;
    return sum;
}

/*!
 * \brief Processor::loadImage
 * \param name
 * \return
 */
QImage Processor::loadImage(const QString& name)
{
    QImage image;
    if (image.load(name)) {
        for (int i = 0; i < image.width(); ++i) {
            for (int j = 0; j < image.height(); ++j) {
                int gray = (qRed(image.pixel(i, j)) + 2 * qGreen(image.pixel(i, j)) + qBlue(image.pixel(i, j))) / 4; //
                image.setPixel(i, j, qRgb(gray, gray, gray));
            }
        }
        //image = sharpen(image);
        repaint();
        return image;
    } else {
        emit somethingWentWrong("Error", "Can not load an image(s)");
        return image;
    }
}

/*!
 * \brief Processor::prev
 */
void Processor::prev()
{
    if (!currentImageNumber) {
        repaint();
    } else {
        currentImageNumber--;
        repaint();
    }
}

/*!
 * \brief Processor::next
 */
void Processor::next()
{
    if (currentImageNumber == (unsigned)fileNames.size() - 1) {
        repaint();
    } else {
        currentImageNumber++;
        repaint();
    }
}

/*!
 * \brief Processor::openImage
 * \param names
 */
void Processor::openImage(const QStringList& names)
{
    videoFileName.clear();
    fileNames.clear();
    fileNames.reserve(names.size());
    for (int i = 0; i < names.size(); ++i) {
        fileNames.push_back(names[i]);
    }
    images.clear();
    if (!fileNames.empty()) {
        images.resize(fileNames.size());
        for (currentImageNumber = 0; currentImageNumber < static_cast<unsigned>(fileNames.size()); ++currentImageNumber) {
            currImage().setImage(loadImage(fileNames[currentImageNumber]));
            currImage().resetCounter();
            currImage().setIsLoaded(true);
            currImage().setIsProcessed(false);
        }
        currentImageNumber = 0;
        repaint();
    }
}

/*!
 * \brief Processor::die
 */
void Processor::die()
{
    this->deleteLater();
}

Image &Processor::currImage()
{
    return images[currentImageNumber];
}

/*!
 * \brief Processor::setDisplay
 * \param dis
 */
void Processor::setDisplay(const Display& dis)
{
    images[currentImageNumber] = dis.im;
    origin = dis.origin;
}

/*!
 * \brief Processor::getDisplay
 * \return
 */
Display Processor::getDisplay()
{
    Display dis;
    dis.im = currImage();
    dis.origin = origin;
    return dis;
}

/*!
 * \brief Processor::sharpen
 * \param im
 * \return
 */
QImage Processor::sharpen(const QImage& im)
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
 * \brief Processor::repaint
 */
void Processor::repaint()
{

    Display dis;
    dis.im = currImage();
    if (!fileNames.isEmpty()) {
        dis.im.setFileName(fileNames[currentImageNumber]);
    }
    dis.origin = origin;
    emit Update(dis);
}

/*!
 * \brief Processor::searchTheLight
 * \param image
 * \param tre
 * \param x1
 * \param y1
 * \param x2
 * \param y2
 */
unsigned Processor::searchTheLight(const Image& im, QRect rect)
{
    rect = rect.normalized();
    unsigned tre = im.getThreshold();
    QImage image = sharpen(im.getImage());
    unsigned counter = 0;
    for (auto i = rect.left() + 1; i != rect.right(); ++i) {
        for (auto j = rect.top() + 1; j != rect.bottom(); ++j) {
            if(i < image.width() && j < image.height()){
                if (static_cast<unsigned>(qGray(image.pixel(i, j))) >= tre) {
                    counter++;
                }
            }
        }
    }
    return counter;
}

/*!
 * \brief Processor::run
 */
void Processor::run()
{
    CaptureWrapper capture(videoFileName);
    try{
        capture.isOpened();
        unsigned frameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
        cv::Mat frame;
        pressureValues.clear();
        pixelValues.clear();
        vid = true;
        fileNames.clear();
        images.clear();
        for (auto i = 0u; i < frameNumber; ++i) {
            if (stop) {
                stop = false;
                break;
            }
            currentImageNumber = i;
            images.resize(i + 1);
            capture.read(frame);
            QImage timage = ImageConverter::Mat2QImage(frame);
            for (int i = 0; i < timage.width(); ++i) {
                for (int j = 0; j < timage.height(); ++j) {
                    int gray = qGray(timage.pixel(i, j));
                    timage.setPixel(i, j, qRgb(gray, gray, gray));
                }
            }
            timage = timage.mirrored();
            
            currImage().setImage(timage);
            Image& image = currImage();
            image.setCrop(config.crop);
            image.setSquare(config.square);
            image.cropImage();
            image.setFullCounter();
            align();
            image.setFullCounter();
            image.setSquare(config.square0);
            run(true);
            image.setPressure(calculate(preparedPixels, preparedPressures, image.getSum() / area()));
            pressureValues.push_back(image.getPressure());
            pixelValues.push_back(image.getSum());
            QThread::currentThread()->usleep(50);
            
            emit plot(pressureValues);
        }
    }
    catch(CaptureError e){
        emit somethingWentWrong("Error", e.getMessage());
        return;
    }

    vid = false;
    
    emit plot(preparedPixels, preparedPressures);
}

/*!
 * \brief Processor::calibrate
 * \param name
 * \param named
 * \param names
 */
void Processor::calibrate(const QString& name, const QString& named, const QStringList& names)
{
    loadConf(name);
    images.clear();
    images.resize(1);
    preparedPixels.clear();
    preparedPressures.clear();
    for (int i = 0; i < 4; ++i) {
        res4[i].clear();
    }
    currentImageNumber = 0;
    fileNames.append(names[0]);
    for (auto it = names.begin(); it != names.end(); ++it) {
        currImage().setImage(loadImage(*it));
        currImage().setIsLoaded(true);
        currImage().setIsProcessed(false);
        currImage().setFullCounter();
        QString tmp = *it;
        int a = tmp.lastIndexOf('.');
        tmp.chop(tmp.size() - a);
        a = tmp.lastIndexOf('/');
        tmp = tmp.right(tmp.size() - a - 1);
        int n = tmp.toInt();
        autorun();
        for (int i = 0; i < 4; ++i) {
            res4[i].push_back(currImage().getBoundCounter()[i]);
        }
        preparedPixels.push_back(currImage().getSum() / area());
        preparedPressures.push_back(n);
    }
    saveData(named);
}

/*!
 * \brief Processor::loadData
 * \param name
 */
void Processor::loadData(const QString& name)
{
    QString filename = name;
    QFile file(filename);
    QVector<double> r4;
    r4.resize(4);
    preparedPixels.clear();
    preparedPressures.clear();
    if (file.open(QFile::ReadOnly)) {
        QTextStream str(&file);
        while (1) {
            double r = 0;
            double p = 0;
            str >> r4[0] >> r4[1] >> r4[2] >> r4[3];
            str >> r >> p;
            if (str.atEnd())
                break;
            res4[0].push_back(r4[0]);
            res4[1].push_back(r4[1]);
            res4[2].push_back(r4[2]);
            res4[3].push_back(r4[3]);
            preparedPixels.push_back(r);
            preparedPressures.push_back(p);
        }
    } else {
        emit somethingWentWrong("Error", "Can not open data file");
    }
}

/*!
 * \brief Processor::saveData
 * \param name
 */
void Processor::saveData(const QString& name)
{
    QString filename = name;
    QFile file(filename);
    if (file.open(QFile::WriteOnly)) {
        QTextStream str(&file);
        for (int i = 0; i < preparedPixels.size(); ++i) {
            str << res4[0][i] << " " << res4[1][i] << " " << res4[2][i] << " " << res4[3][i] << " ";
            str << preparedPixels[i] << " " << preparedPressures[i] << " \n";
        }
    } else {
        emit somethingWentWrong("Error", "Can not open data file");
    }
}

/*!
 * \brief Processor::stopThis
 */
void Processor::stopThis()
{
    stop = true;
}

/*!
 * \brief Processor::saveConf
 * \param name
 * \param def
 */
void Processor::saveConf(const QString& name, bool def)
{
    Image& image = currImage();
    static bool fp = true;
    static QString filename = QString("default.conf");
    if (!def) {
        if (fp) {
            filename = name;
            QFile file(filename);
            if (file.open(QFile::WriteOnly)) {
                QTextStream str(&file);
                str << image.getCrop().left() << " " << image.getCrop().top() << "\n";
                str << image.getCrop().right() << " " << image.getCrop().bottom() << "\n";
                str << image.getSquare()[0].x() << " " << image.getSquare()[0].y() << "\n";
                str << image.getSquare()[1].x() << " " << image.getSquare()[1].y() << "\n";
                str << image.getSquare()[2].x() << " " << image.getSquare()[2].y() << "\n";
                emit somethingWentWrong("Next step", "Put 3 points then press Save again");
            } else {
                emit somethingWentWrong("Error", "Can not open a file");
            }
            fp = false;
        } else {
            QFile file(filename);
            if (file.open(QFile::Append)) {
                QTextStream str(&file);
                str << image.getSquare()[0].x() << " " << image.getSquare()[0].y() << "\n";
                str << image.getSquare()[1].x() << " " << image.getSquare()[1].y() << "\n";
                str << image.getSquare()[2].x() << " " << image.getSquare()[2].y() << "\n";
            } else {
                emit somethingWentWrong("Error", "Can not open a file");
            }
            fp = true;
            filename = QString("default.conf");
        }
    } else {
        QFile file(filename);
        if (file.open(QFile::WriteOnly)) {
            QTextStream str(&file);
            str << config.crop.left() << " " << config.crop.top() << "\n";
            str << config.crop.right() << " " << config.crop.bottom() << "\n";
            str << config.square[0].x() << " " << config.square[0].y() << "\n";
            str << config.square[1].x() << " " << config.square[1].y() << "\n";
            str << config.square[2].x() << " " << config.square[2].y() << "\n";
            str << config.square0[0].x() << " " << config.square0[0].y() << "\n";
            str << config.square0[1].x() << " " << config.square0[1].y() << "\n";
            str << config.square0[2].x() << " " << config.square0[2].y() << "\n";
        } else {
            emit somethingWentWrong("Error", "Can not open a file");
        }
    }
}

/*!
 * \brief Processor::loadConf
 * \param name
 */
void Processor::loadConf(const QString& name)
{
    Image& image = currImage();
    QString filename = name;
    QFile file(filename);
    if (file.open(QFile::ReadOnly)) {
        QTextStream str(&file);
        int r, t, l, b;
        str >> l >> t;
        str >> r >> b;
        config.crop.setRight(r);
        config.crop.setLeft(l);
        config.crop.setTop(t);
        config.crop.setBottom(b);
        str >> config.square[0].rx() >> config.square[0].ry();
        str >> config.square[1].rx() >> config.square[1].ry();
        str >> config.square[2].rx() >> config.square[2].ry();
        str >> config.square0[0].rx() >> config.square0[0].ry();
        str >> config.square0[1].rx() >> config.square0[1].ry();
        str >> config.square0[2].rx() >> config.square0[2].ry();
        if (str.status() & QTextStream::ReadCorruptData || str.status() & QTextStream::ReadPastEnd) {
            emit somethingWentWrong("Error", "Can not read config!");
            return;
        }
    } else {
        emit somethingWentWrong("Error", "Can not open a file");
        repaint();
    }
    image.setConfig(config);
    image.setFullCounter();
}

/*!
 * \brief Processor::align
 * \param vu_flag
 */
void Processor::align()
{
    Image& image = currImage();
    QMatrix matrix;
    matrix.shear(4 * (image.getSquare()[1].x() - image.getSquare()[2].x()) / (double)image.getImage().width(), 4 * (image.getSquare()[0].y() - image.getSquare()[1].y()) / (double)image.getImage().height());
    image.setImage(image.getImage().transformed(matrix));
    image.resetCounter();
    if (!vid)
        repaint();
}

/*!
 * \brief Processor::run
 * \param vu_flag
 */
void Processor::run(bool vu_flag)
{
    Image& image = currImage();

    if (image.isCounterFull() && !image.getImage().isNull()) {
        unsigned x1 = image.getSquare()[1].x() - origin.second.x();
        unsigned x2 = image.getImage().width() - 1;
        unsigned y1 = image.getSquare()[1].y() - origin.second.y();
        unsigned y2 = image.getSquare()[2].y() - origin.second.y();
        image.getBoundCounterRef()[0] = searchTheLight(image, QRect(QPoint(x1, y1), QPoint(x2, y2)));
        x1 = 0;
        x2 = image.getSquare()[0].x() - origin.second.x();
        y1 = image.getSquare()[0].y() - origin.second.y();
        y2 = image.getSquare()[2].y() - origin.second.y();
        image.getBoundCounterRef()[1] = searchTheLight(image, QRect(QPoint(x1, y1), QPoint(x2, y2)));
        x1 = image.getSquare()[0].x() - origin.second.x();
        x2 = image.getSquare()[1].x() - origin.second.x();
        y1 = 0;
        y2 = image.getSquare()[0].y() - origin.second.y();
        image.getBoundCounterRef()[2] = searchTheLight(image, QRect(QPoint(x1, y1), QPoint(x2, y2)));
        x1 = image.getSquare()[0].x() - origin.second.x();
        x2 = image.getSquare()[1].x() - origin.second.x();
        y1 = image.getSquare()[2].y() - origin.second.y();
        y2 = image.getImage().height() - 1;
        image.getBoundCounterRef()[3] = searchTheLight(image, QRect(QPoint(x1, y1), QPoint(x2, y2)));
    } else {
        emit somethingWentWrong("No points or image", "Put 3 points or open image");
    }
    if (!vid && vu_flag)
        repaint();
}

/*!
 * \brief Processor::reset
 */
void Processor::reset()
{
    Image& image = currImage();
    image.setImage(loadImage(fileNames[currentImageNumber]));
    image.resetCounter();
    origin = {{0, 0 }, {0, 0}};
    image.setIsProcessed(false);
    repaint();
}

/*!
 * \brief Processor::autorun
 * \param vu_flag
 */
void Processor::autorun(bool vu_flag)
{
    if (!videoFileName.isEmpty()) {
        vid = true;
        fileNames.clear();
        images.clear();
        images.resize(frame_num);
        QList<QFuture<double> > sums;
        for (unsigned i = 0; i < frame_num; ++i) {
            fileNames.push_back((QString("frame_") + QString::number(i) + QString(".bmp")));
            currentImageNumber = i;
            currImage().setImage(loadImage(fileNames[currentImageNumber]));
            Image& image = currImage();
            image.setCrop(config.crop);
            image.setSquare(config.square);
            image.cropImage();
            image.setFullCounter();
            align();
            image.setFullCounter();
            image.setSquare(config.square0);
            run(true);
            image.setPressure(calculate(preparedPixels, preparedPressures, image.getSum() / area()));
            pressureValues.push_back(image.getPressure());
            pixelValues.push_back(image.getSum());
        }
        vid = false;
    } else {
        Image& image = currImage();
        if (!image.getIsProcessed() && image.getIsLoaded()) {
            image.setCrop(config.crop);
            image.setSquare(config.square);
            image.cropImage();
            image.setFullCounter();
            repaint();
            align();
            image.setSquare(config.square0);
            image.setFullCounter();
            run(true);
            image.setPressure(calculate(preparedPixels, preparedPressures, image.getSum() / area()));
            image.setIsProcessed(true);
            if (vu_flag) {
                repaint();
            }
        } else {
            repaint();
            emit somethingWentWrong("Fail", "Image already processed or not loaded");
        }
    }
}

double Processor::area(){
    Image& image = currImage();
    int a = abs(image.getSquare()[0].x() - image.getSquare()[1].x());
    int b = abs(image.getSquare()[1].y() - image.getSquare()[2].y());
    return a*b;
}
