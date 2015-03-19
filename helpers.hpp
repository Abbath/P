#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <QtCore>
#include <QtGui>
#include <QPointF>
#include <array>
#include <numeric>

enum class DataType{PIXELS, PRESSURE}; 

typedef std::array<QPoint, 3> Square;

/*!
 * \brief The Comparator class
 */
class Comparator
{
public:
    QPoint M;
    Comparator(const QPoint& origin)
    {
        M = origin;
    }
    
    bool operator()(const QPoint& o1, const QPoint& o2)
    {
        const double angle1 = atan2(o1.y() - M.y(), o1.x() - M.x());
        const double angle2 = atan2(o2.y() - M.y(), o2.x() - M.x());
        return (angle1 < angle2);
    }
};

/*!
 * \brief The Modes enum
 */
enum Modes
{
    ISO,
    TOP,
    LEFT,
    BOTTOM,
    RIGHT
};

/*!
 * \brief The Line struct
 */
struct Line
{
    double x1;
    double y1;
    double z1;
    double x2;
    double y2;
    double z2;
    int c;
    Line() : x1( 0.0 ), y1( 0.0 ), z1( 0.0 ), x2( 0.0 ), y2( 0.0 ), z2( 0.0 ), c( 0 ) {}
};

/*!
 * \brief The Config struct
 */
struct Config
{
    QRect crop;
    Square square;
    Square square0;
};

/*!
 * \brief The Image class
 */
class Image
{
    
public:
    void resetCounter(){ counter = 0u; }
    void setFullCounter(){ counter = 3u; }
    bool isCounterFull(){ return counter == 3u; }
    unsigned getSum() const { return std::accumulate(bound_counter.begin(), bound_counter.end(), 0); }
    const QImage& getImageRef() const { return image; }
    QRect& getCropRef(){ return config.crop; }
    std::array<unsigned, 4>& getBoundCounterRef(){ return bound_counter; }
    QImage getImage() const {return image; }
    unsigned getThreshold() const { return threshold; }
    std::array<unsigned, 4> getBoundCounter() const { return bound_counter; }
    QString getFileName() const { return fileName; }
    QRect getCrop() const { return config.crop; }
    const Square& getSquare(int index) const { if(index == 0) return config.square; else return config.square0; }
    Square& getSquareRef(int index) { if(index == 0) return config.square; else return config.square0; }
    unsigned getCounter() const { return counter; }
    void incrementCounter(){ counter++; }
    void setImage(const QImage& _image){ image = _image; }
    void setFileName(const QString& filename){ fileName = filename; }
    void setCrop(const QRect& rect){ config.crop = rect; }
    void setSquare(int index, const Square& sq){ if(index == 1) config.square0 = sq; else config.square = sq; }
    void setConfig(const Config& conf){ config = conf; }
    void setThreshold(unsigned th){ threshold = th; }
    bool isImageNull(){ return image.isNull(); }
    void cropImage(){ image = image.copy(config.crop); }
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

/*!
 * \brief The Display struct
 */
struct Display
{
    Image im;
    QPair<QPoint, QPoint> origin;
};

/*!
 * \brief The ModelingData class
 */
class ModelingData {
    bool is_ext = false;
    QString filename = "";
    double pressure = 0;
    double die_size;
    double mem_size;
    double mem_thickness;
    double spacer_height;
    double ym;
    double pr;
    unsigned int ray_number;
    unsigned int wavelength;
    unsigned int rows;
    unsigned int hole_size;
    unsigned int space_size;
    double camx;
    double camy;
    double x_angle = 0;
    double y_angle = 0;
    
    
public:
    QString getFilename() const
    {
        return filename;
    }
    
    void setFilename(const QString &value)
    {
        filename = value;
    }
    
    double getDie_size() const
    {
        return die_size;
    }
    
    void setDie_size(double value)
    {
        die_size = value;
    }
    
    double getMem_size() const
    {
        return mem_size;
    }
    
    void setMem_size(double value)
    {
        mem_size = value;
    }
    
    double getMem_thickness() const
    {
        return mem_thickness;
    }
    
    void setMem_thickness(double value)
    {
        mem_thickness = value;
    }
    
    double getYm() const
    {
        return ym;
    }
    
    void setYm(double value)
    {
        ym = value;
    }
    
    double getPr() const
    {
        return pr;
    }
    
    void setPr(double value)
    {
        pr = value;
    }
    
    unsigned int getRay_number() const
    {
        return ray_number;
    }
    
    void setRay_number(unsigned int value)
    {
        ray_number = value;
    }
    
    unsigned int getWavelength() const
    {
        return wavelength;
    }
    
    void setWavelength(unsigned int value)
    {
        wavelength = value;
    }
    
    unsigned int getRows() const
    {
        return rows;
    }
    
    void setRows(unsigned int value)
    {
        rows = value;
    }
    
    unsigned int getHole_size() const
    {
        return hole_size;
    }
    
    void setHole_size(unsigned int value)
    {
        hole_size = value;
    }
    
    unsigned int getSpace_size() const
    {
        return space_size;
    }
    
    void setSpace_size(unsigned int value)
    {
        space_size = value;
    }
    
    double getCamx() const
    {
        return camx;
    }
    
    void setCamx(double value)
    {
        camx = value;
    }
    
    double getCamy() const
    {
        return camy;
    }
    
    void setCamy(double value)
    {
        camy = value;
    }
    
    double getX_angle() const
    {
        return x_angle;
    }
    
    void setX_angle(double value)
    {
        x_angle = value;
    }
    
    double getY_angle() const
    {
        return y_angle;
    }
    
    void setY_angle(double value)
    {
        y_angle = value;
    }
    
    bool getIs_ext() const
    {
        return is_ext;
    }
    
    void setIs_ext(bool value)
    {
        is_ext = value;
    }
    
    double getPressure() const
    {
        return pressure;
    }
    
    void setPressure(double value)
    {
        pressure = value;
    }
    
    double getSpacer_height() const
    {
        return spacer_height;
    }
    
    void setSpacer_height(double value)
    {
        spacer_height = value;
    }
    
};

#endif // HELPERS_HPP





