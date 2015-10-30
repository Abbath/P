#ifndef IMAGESTORAGE_HPP
#define IMAGESTORAGE_HPP

#include <QObject>
#include <QtCore>
#include <memory>
#include <mutex>
#include "image.hpp"

class ImageStorage : public QObject
{
    Q_OBJECT
public:
    ~ImageStorage();
    static ImageStorage& getInstance();
    void resize(int size);
    void clear();
    Image& getCurrImageRef();
    unsigned int &getCurrNumRef();
signals:
    
public slots:
    
private:
    static std::unique_ptr<ImageStorage> instance;
    static std::once_flag onceFlag;
    QVector<Image> images; 
    unsigned int current = 0;
    explicit ImageStorage(QObject *parent = 0);
    ImageStorage(const ImageStorage& is);
    ImageStorage& operator=(const ImageStorage& rhs);
};

#endif // IMAGESTORAGE_HPP
