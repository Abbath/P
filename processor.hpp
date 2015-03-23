#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <QObject>
#include "helpers.hpp"
#include <QRunnable>
#include <QMessageBox>
#include <QThread>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "capturewrapper.hpp"
#include "imageconverter.hpp"
#include "imageprocessor.hpp"
#include "calculator.hpp"
#include <fstream>

/*!
 * \brief The Processor class
 */
class Processor : public QObject
{
    Q_OBJECT
    
public:
    explicit Processor(QObject *parent = 0);
    QImage loadImage(const QString &name);
    void saveImage(const QString &name);
    void run(bool vu_flag);
    void calibrate(const QString &name, const QString &named, const QStringList &names);
    void saveResults(const QString &name);
    void getFrame(int n);
    void processVideo();
    int openVideo(const QString &name);
    void setDisplay(const Display& dis);
    void setVideoFileName(QString _f){videoFileName = _f;}
    Display getDisplay();
    Image& getImage(){ return images[currentImageNumber];}
    const QVector<double>& getPressureValues() const {return pressureValues;}
    const QVector<double>& getPixelValues() const {return pixelValues;}
    const QVector<double>& getPreparedPixels() const { return preparedPixels;}
    const QVector<double>& getPreparedPressures() const { return preparedPressures;}
    const QString getVName() const { return videoFileName;}    

    int extractPressure(QStringList::const_iterator it);
    Config getConfig() const;
    void setConfig(const Config &value);
    void setDatum(const QVector<double>& pix, const QVector<double> pres){ preparedPixels = pix; preparedPressures = pres;}
    
signals:
    void Update(Display dis);
    void somethingWentWrong(QString, QString);
    void plot( QVector<double> preparedPixels);
    void plot(QVector<double>, QVector<double>);
public slots:
    void align();
    void reset();
    void autorun(bool vu_flag = true);
    void run();
    void prev();
    void next();
    void loadConf(const QString &name);
    void saveConf(const QString &name);
    void loadData(const QString &name);
    void saveData(const QString &name);
    void stopThis();
    void openImage(const QStringList &names);
    void die();
    Image& currImage();

private:
    void repaint();
private:
    QVector<Image> images;
    unsigned currentImageNumber = 0, frame_num = 0;
    QPair<QPoint, QPoint> origin = {{0,0},{0,0}};
    QString videoFileName;
    QStringList fileNames;
    Config config;

    QVector<double> pressureValues;
    QVector<double> pixelValues;

    QVector<double> preparedPressures;
    QVector<double> preparedPixels;

    QVector<double> res4[4];
    bool vid = false;
    unsigned int threshold = 255;    
    volatile bool stop = false;
    double area();
};

#endif // PROCESSOR_HPP
