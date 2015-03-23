#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <QtCore>
#include <QtGui>
#include "config.hpp"

/*!
 * \brief The Image class
 */
class Image
{
public:
    explicit Image();
    ~Image();
    void resetCounter(){ counter = 0u; }
    void setFullCounter(){ counter = 3u; }
    bool isCounterFull(){ return counter == 3u; }
    unsigned getSum() const { return std::accumulate(bound_counter.begin(), bound_counter.end(), 0); }
    const QImage& getImageRef() const { return image; }
    QRect& getCropRef(){ return config.getCropRef(); }
    std::array<unsigned, 4>& getBoundCounterRef(){ return bound_counter; }
    QImage getImage() const {return image; }
    unsigned getThreshold() const { return threshold; }
    std::array<unsigned, 4> getBoundCounter() const { return bound_counter; }
    QString getFileName() const { return fileName; }
    QRect getCrop() const { return config.getCrop(); }
    Square getSquare(int index) const { if(index == 0) return config.getSquare(); else return config.getSquare0(); }
    Square& getSquareRef(int index) { if(index == 0) return config.getSquare0Ref(); else return config.getSquare0Ref(); }
    unsigned getCounter() const { return counter; }
    void incrementCounter(){ counter++; }
    void setImage(const QImage& _image){ image = _image; }
    void setFileName(const QString& filename){ fileName = filename; }
    void setCrop(const QRect& rect){ config.setCrop(rect); }
    void setSquare(int index, const Square& sq){ if(index == 1) config.setSquare0(sq); else config.setSquare(sq); }
    void setConfig(const Config& conf){ config.setCrop(conf.getCrop()); config.setSquare(conf.getSquare()); config.setSquare0(conf.getSquare0()); }
    void setThreshold(unsigned th){ threshold = th; }
    bool isImageNull(){ return image.isNull(); }
    void cropImage(){ image = image.copy(config.getCrop()); }
    bool getIsProcessed() const { return isProcessed; }
    void setIsProcessed(bool value) { isProcessed = value; }
    bool getIsLoaded() const { return isLoaded; }
    void setIsLoaded(bool value) { isLoaded = value; }
    double getPressure() const { return pressure; }
    void setPressure(double value) { pressure = value; }
    int getSi() const { return si; }
    void setSi(int value) { si = value;}
    Config getConfig(){return config;}
    
private:
    double pressure = 0.0l;
    bool isProcessed = false;
    bool isLoaded = false;
    QImage image;
    //QRect crop;
    //Square square;
    int si = 0;
    Config config;
    QString fileName;
    unsigned int threshold = 128u;
    unsigned int counter = 0u;
    std::array<unsigned, 4> bound_counter;
};

#endif // IMAGE_HPP
