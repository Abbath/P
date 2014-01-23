#include "processor.hpp"

Processor::Processor(QObject *parent) :
    QObject(parent)
{
}

QImage Processor::loadImage(const std::string &name)
{
    QImage image;
    if(image.load(QString::fromStdString(name))){
        for(int i = 0; i < image.width(); ++i){
            for(int j = 0; j < image.height(); ++j){
                int gray = qGray(image.pixel(i,j));
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
        }
        image = sharpen(image);
        repaint();
        return image;
    }else{
        emit somethingWentWrong("Error", "Can not load an image(s)");
        return image;
    }
}

void Processor::openImage(const QVector<std::string> &names)
{
    qDebug() << "I'm in Processor::openImage\n";
    fileNameV.clear();
    fileNames.clear();
    fileNames.resize(names.size());
    for(int i = 0; i< names.size(); ++i){
        fileNames.push_back(names[i]);
    }
    images.clear();
    if(!fileNames.empty()){
        images.resize(fileNames.size());
        for(curr = 0; curr < static_cast<unsigned>(fileNames.size()); ++curr){
            images[curr].image = loadImage(fileNames[curr]);
            images[curr].counter = 0;
            images[curr].l = true;
        }
        curr = 0;
        repaint();
    }
}

void Processor::setDisplay(Display dis)
{
    images[curr] = dis.im;
    origin[0] = dis.origin[0];
    origin[1] = dis.origin[1];
}

QImage Processor::sharpen(const QImage &im)
{
    QImage image = im;
    QImage oldImage = im;
    int kernel [3][3]= {{0,-1,0},
                        {-1,5,-1},
                        {0,-1,0}};
    /* int kernel  [5][5] ={
                        {0,0,-1,0,0},
                        {0,-1,-2,-1,0},
                        {-1,-2,20,-2,-1},
                        {0,-1,-2,-1,0},
                        {0,0,-1,0,0}};*/
    int kernelSize = 3;
    int sumKernel = 1;
    int r,g,b;
    QColor color;
    for(int x=kernelSize/2; x<image.width()-(kernelSize/2); x++){
        for(int y=kernelSize/2; y<image.height()-(kernelSize/2); y++){
            r = 0;
            g = 0;
            b = 0;
            for(int i = -kernelSize/2; i<= kernelSize/2; i++){
                for(int j = -kernelSize/2; j<= kernelSize/2; j++){
                    color = QColor(oldImage.pixel(x+i, y+j));
                    r += color.red()*kernel[kernelSize/2+i][kernelSize/2+j];
                    g += color.green()*kernel[kernelSize/2+i][kernelSize/2+j];
                    b += color.blue()*kernel[kernelSize/2+i][kernelSize/2+j];
                }
            }
            r = qBound(0, r/sumKernel, 255);
            g = qBound(0, g/sumKernel, 255);
            b = qBound(0, b/sumKernel, 255);
            image.setPixel(x,y, qRgb(r,g,b));
        }
    }
    return image;
}

void Processor::repaint()
{
    Display dis;
    dis.im = images[curr];
    emit Update(dis);
}


