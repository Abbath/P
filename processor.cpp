#include "processor.hpp"
#include "db.hpp"
#include "detector.hpp"
#include <vector>
#include "imagestorage.hpp"
/*!
 * \brief Processor::Processor
 * \param parent
 */
Processor::Processor(QObject* parent)
    : QObject(parent)
{
    //images.resize(1);
    ImageStorage::getInstance().resize(1);
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
    if (!currentImageNumber()) {
        repaint();
    } else {
        currentImageNumber()--;
        repaint();
    }
}

/*!
 * \brief Processor::next
 */
void Processor::next()
{
    if (currentImageNumber() == (unsigned)fileNames.size() - 1) {
        repaint();
    } else {
        currentImageNumber()++;
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
    //images.clear();
    ImageStorage::getInstance().clear();
    if (!fileNames.empty()) {
        //images.resize(fileNames.size());
        ImageStorage::getInstance().resize(fileNames.size());
        for (currentImageNumber() = 0; currentImageNumber() < static_cast<unsigned>(fileNames.size()); ++currentImageNumber()) {
            currImage().setImage(loadImage(fileNames[currentImageNumber()]));
            currImage().resetCounter();
            currImage().setIsLoaded(true);
            currImage().setIsProcessed(false);
        }
        currentImageNumber() = 0;
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

/*!
 * \brief Processor::currImage
 * \return 
 */
Image &Processor::currImage()
{
    //return images[currentImageNumber];
    return ImageStorage::getInstance().getCurrImageRef();
}

/*!
 * \brief Processor::setDisplay
 * \param dis
 */
void Processor::setDisplay(const Display& dis)
{
    //images[currentImageNumber] = dis.im;
    ImageStorage::getInstance().getCurrImageRef() = dis.im;
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

Image &Processor::getImage()
{
    return ImageStorage::getInstance().getCurrImageRef();
}


/*!
 * \brief Processor::repaint
 */
void Processor::repaint()
{
    
    Display dis;
    dis.im = currImage();
    if (!fileNames.isEmpty()) {
        dis.im.setFileName(fileNames[currentImageNumber()]);
    }
    dis.origin = origin;
    emit Update(dis);
}
Config Processor::getConfig() const
{
    return config;
}

void Processor::setConfig(const Config &value)
{
    config = value;
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
        //images.clear();
        ImageStorage::getInstance().clear();
        for (auto i = 0u; i < frameNumber; ++i) {
            if (stop) {
                stop = false;
                break;
            }
            currentImageNumber() = i;
            //images.resize(i + 1);
            ImageStorage::getInstance().resize(i + 1);
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
            image.setConfig(config);
            image.cropImage();
            image.setFullCounter();
            align();
            image.setFullCounter();
            run(true);
            Calculator calc;
            image.setPressure(calc.calculate(preparedPixels, preparedPressures, image.getSum() / area()));
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
int Processor::extractPressure(QStringList::const_iterator it)
{
    QString tmp = *it;
    int a = tmp.lastIndexOf('.');
    tmp.chop(tmp.size() - a);
    a = tmp.lastIndexOf('/');
    tmp = tmp.right(tmp.size() - a - 1);
    int n = tmp.toInt();
    
    return n;
}

void Processor::calibrate(const QString& name, const QString& named, const QStringList& names)
{
    loadConf(name);
    //images.clear();
    //images.resize(1);
    ImageStorage::getInstance().clear();
    ImageStorage::getInstance().resize(1);
    preparedPixels.clear();
    preparedPressures.clear();
    for (int i = 0; i < 4; ++i) {
        res4[i].clear();
    }
    currentImageNumber() = 0;
    fileNames.append(names[0]);
    for (auto it = names.begin(); it != names.end(); ++it) {
        currImage().setImage(loadImage(*it));
        currImage().setIsLoaded(true);
        currImage().setIsProcessed(false);
        currImage().setFullCounter();
        int n = extractPressure(it);
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
        
        qDebug() << DB::getInstance().addData(preparedPixels, preparedPressures).text() << DB::getInstance().addConf(config);
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
void Processor::saveConf(const QString& name)
{
    Image& image = currImage();
    if(!image.getConfig().saveConf(name)){
        emit somethingWentWrong("Error", "Can not write config!");
    }
    DB::getInstance().addConf(image.getConfig());
}

/*!
 * \brief Processor::loadConf
 * \param name
 */
void Processor::loadConf(const QString& name)
{
    Image& image = currImage();
    if(!config.loadConf(name)){
        emit somethingWentWrong("Error", "Can not read config!");
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
    ImageProcessor ip;
    image.setImage(ip.align(image.getImage(), image.getSquare(0)));
    image.resetCounter();
    image.setSi(1);
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
        ImageProcessor ip;
        image.getBoundCounterRef() = ip.getLightAmount(image.getImage(), image.getConfig(), origin, image.getThreshold());
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
    image.setImage(loadImage(fileNames[currentImageNumber()]));
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
        //images.clear();
        //images.resize(frame_num);
        ImageStorage::getInstance().clear();
        ImageStorage::getInstance().resize(frame_num);
        QList<QFuture<double> > sums;
        for (unsigned i = 0; i < frame_num; ++i) {
            fileNames.push_back((QString("frame_") + QString::number(i) + QString(".bmp")));
            currentImageNumber() = i;
            currImage().setImage(loadImage(fileNames[currentImageNumber()]));
            Image& image = currImage();
            image.setConfig(config);
            image.cropImage();
            image.setFullCounter();
            align();
            image.setFullCounter();
            run(true);
            Calculator calc;
            image.setPressure(calc.calculate(preparedPixels, preparedPressures, image.getSum() / area()));
            pressureValues.push_back(image.getPressure());
            pixelValues.push_back(image.getSum());
        }
        vid = false;
    } else {
        Image& image = currImage();
        if (!image.getIsProcessed() && image.getIsLoaded()) {
            image.setConfig(config);
            image.cropImage();
            //            QImage t("templ3.png");
            //            Detector d;
            //            image.setImage(image.getImage().copy(d.detect(t, image.getImage())));
            image.setFullCounter();
            repaint();
            align();
            image.setFullCounter();
            run(true);
            Calculator calc;
            image.setPressure(calc.calculate(preparedPixels, preparedPressures, image.getSum() / area()));
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

void Processor::detectRun()
{
    Image& image = currImage();
    if (!image.getIsProcessed() && image.getIsLoaded()) {
        QImage t("templ2.png");
        Detector d;
        QRect r = d.detect(t, image.getImage());
        image.setImage(image.getImage().copy(r));
        repaint();
        std::vector<std::vector<cv::Point> > arr;
        cv::Mat mat = ImageConverter::QImage2Mat(image.getImage());
        std::vector<cv::Vec4i> h;            
        findContours(mat, arr, h, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
        double sum = 0;
        for (unsigned int j = 0; j < arr.size();++j) {
            if(h[j][3] < 0){
                sum += contourArea(arr[j]);
            }
        }
        Calculator calc;
        image.setPressure(calc.calculate(preparedPixels, preparedPressures, sum));
        image.setIsProcessed(true);
        repaint();
    }
}

/*!
 * \brief Processor::area
 * \return 
 */
double Processor::area(){
    Image& image = currImage();
    int a = abs(image.getSquare(1)[0].x() - image.getSquare(1)[1].x());
    int b = abs(image.getSquare(1)[1].y() - image.getSquare(1)[2].y());
    return a*b;
}

unsigned &Processor::currentImageNumber()
{
    return ImageStorage::getInstance().getCurrNumRef();
}
