#ifndef MODELINGDATA_HPP
#define MODELINGDATA_HPP

#include <QtCore>

/*!
 * \brief The ModelingData class
 */
class ModelingData{
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
    
    ~ModelingData();
    explicit ModelingData();
};

#endif // MODELINGDATA_HPP
