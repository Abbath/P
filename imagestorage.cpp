#include "imagestorage.hpp"

ImageStorage::ImageStorage(QObject *parent) : QObject(parent)
{

}

ImageStorage::~ImageStorage()
{
    
}

ImageStorage &ImageStorage::getInstance()
{
    std::call_once(onceFlag, []{ instance.reset(new ImageStorage);});
    return *instance.get();
}

void ImageStorage::resize(int size)
{
    images.resize(size);
}

void ImageStorage::clear()
{
    images.clear();
}

Image &ImageStorage::getCurrImageRef()
{
    return images[current];
}

unsigned int &ImageStorage::getCurrNumRef()
{
    return current;
}

std::unique_ptr<ImageStorage> ImageStorage::instance;
std::once_flag ImageStorage::onceFlag;
