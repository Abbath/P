#include "imagearea.hpp"
#include "ui_imagearea.h"

#include <QtConcurrent/QtConcurrent>



ImageArea::ImageArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageArea)
{
    ui->setupUi(this);
    loadConf(true);
}

void ImageArea::paintEvent(QPaintEvent *e){
    grabMouse();
    QPainter painter(this);
    if(d3 && !image.isNull()){
        std::sort(lines.begin(),lines.end(), [](Line a, Line b) { return a.z1 < b.z1; });
        QImage pix(this->width(),this->height(), image.format());
        pix.fill(Qt::black);
        QPainter p;
        p.begin(&pix);
        for(Line l : lines){
            if(l.c >= tre()){
                p.setPen(qRgb(0,255,0));
            }else{
                p.setPen(qRgb(l.c,l.c,l.c));
            }
            p.drawLine(l.x1+300,l.y1+500,l.x2+300,l.y2+500);
        }
        painter.drawImage(0,0,pix);
        e->accept();
        releaseMouse();
        return;
    }
    painter.drawImage(0,0,image);
    if(counter > 0){
        for(unsigned i = 0; i < counter; ++i){
            painter.setPen(Qt::red);
            painter.drawLine(square[i].x()-4,square[i].y(),square[i].x()+4,square[i].y());
            painter.drawLine(square[i].x(),square[i].y()-4,square[i].x(),square[i].y()+4);
            painter.setPen(Qt::white);
        }
    }
    if(rect){
        painter.setPen(Qt::green);
        painter.drawRect(crop[0].x(),crop[0].y(),crop[1].x()-crop[0].x(),crop[1].y()-crop[0].y());
        painter.setPen(Qt::white);
    }else{
        if(zoom_b){
            QImage zoomed = image.copy(zoom.x()-10,zoom.y()-10,20,20).scaled(41,41);
            zoomed.setPixel(20,20,qRgb(0,255,0));
            painter.drawImage(zoom.x(),zoom.y(),zoomed);
        }
    }
    painter.drawText(30,30,QString::number(image.width())+QString(", ")+QString::number(image.height())+", "+QString::number(threshold)+", "+QString::number(sum));
    painter.drawText(30,50,
                     QString("left: ")+QString::number(bound_counter[2])+
            QString(" up: ")+QString::number(bound_counter[1])+
            QString(" right: ")+QString::number(bound_counter[0])+
            QString(" down: ")+QString::number(bound_counter[3])+
            QString(" ave: ")+QString::number((bound_counter[2]+bound_counter[1]+bound_counter[0]+bound_counter[3])/4.)+
            QString(" sum: ")+QString::number(bound_counter[2]+bound_counter[1]+bound_counter[0]+bound_counter[3]) );
    e->accept();
    releaseMouse();
}

void ImageArea::keyPressEvent(QKeyEvent *e){
    if(e->key() == Qt::Key_O && e->modifiers() == Qt::ControlModifier){
        openImage();
    }
    if(e->key() == Qt::Key_R && e->modifiers() == Qt::ControlModifier){
        reset();
    }
    if(e->key() == Qt::Key_E && e->modifiers() == Qt::ControlModifier){
        d3 = !d3;
    }

    repaint();
}

void ImageArea::mousePressEvent(QMouseEvent *e){
    if(e->button() == Qt::RightButton && !image.isNull() ){
        rect = true;
        crop[0].setX(e->x());
        crop[0].setY(e->y());
        repaint();
    }
    if(e->button() == Qt::LeftButton && !image.isNull()){
        zoom_b = true;
        zoom.setX(e->x());
        zoom.setY(e->y());
        repaint();
    }

}

void ImageArea::mouseMoveEvent(QMouseEvent *e){
    if(!image.isNull()){
        if(rect){
            crop[1].setX(e->x());
            crop[1].setY(e->y());

        }else{
            zoom.setX(e->x());
            zoom.setY(e->y());
        }
    }
    repaint();
}

void ImageArea::mouseReleaseEvent(QMouseEvent *e){
    if(e->button() == Qt::RightButton && !image.isNull()){
        if(crop[0].x() > crop[1].x()){
            int tmp = crop[0].x();
            crop[0].setX(crop[1].x());
            crop[1].setX(tmp);
        }
        if(crop[0].y() > crop[1].y()){
            int tmp = crop[0].y();
            crop[0].setY(crop[1].y());
            crop[1].setY(tmp);
        }
        image = image.copy(crop[0].x(),crop[0].y(),crop[1].x()-crop[0].x(),crop[1].y()-crop[0].y());
        rect = false;
    }
    if(e->button() == Qt::LeftButton && !image.isNull()){
        if(counter < 3){
            square[counter].setX(e->x());
            square[counter].setY(e->y());
            counter++;
        }else{
            counter = 0;
        }
        zoom_b = false;
    }
    repaint();
    e->accept();
}

void ImageArea::wheelEvent(QWheelEvent *e){
    qint32 a = threshold + e->delta()/80;
    if( a > 255 ) a = 255;
    if( a < 0 ) a = 0;
    threshold = quint8(a);
    if(!image.isNull()){
        run();
    }
}

void ImageArea::openImage()
{
    fileName = QFileDialog::getOpenFileName( this, tr("Open data file"), "", tr("Image files (*.bmp)"));
    loadImage();
    repaint();
}

int ImageArea::openVideo()
{
    fileNameV = QFileDialog::getOpenFileName( this, tr("Open data file"), "", tr("Video files (*.avi)"));
    this->setCursor(Qt::WaitCursor);
    QFuture<int> fn = QtConcurrent::run(&conv, &Converter::processVideo, fileNameV);
    int frame_num = fn.result();
    this->setCursor(Qt::ArrowCursor);
    return frame_num;
}

void ImageArea::reset()
{
    loadImage();
    repaint();
}

void ImageArea::loadImage(){
    if(image.load(fileName)){
        for(int i = 0; i < image.width(); ++i){
            for(int j = 0; j < image.height(); ++j){
                int gray = qGray(image.pixel(i,j));
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
        }
        sharpen();
        counter = 0;
    }else{
        QMessageBox::warning(this, tr("Error"), tr("Can not load an image"));
    }
}

void ImageArea::run()
{
    qDebug() <<counter << image.isNull();
    QImage timage = image;
    for(unsigned& x : bound_counter){
        x = 0;
    }
    if(d3 && !image.isNull()){
        QFuture<QVector<Line> > tmp = QtConcurrent::run(&conv, &Converter::convert, image, mode);
        lines = tmp.result();
        repaint();
        return;
    }
    if( counter == 3 && !image.isNull()){
        unsigned x1 = square[1].x();
        unsigned x2 = image.width()-1;
        unsigned y1 = square[1].y();
        unsigned y2 = square[2].y();
        bound_counter[0] = searchTheLight(x1,y1,x2,y2);
        x1 = 0;
        x2 = square[0].x();
        y1 = square[0].y();
        y2 = square[2].y();
        bound_counter[1] = searchTheLight(x1,y1,x2,y2);
        x1 = square[0].x();
        x2 = square[1].x();
        y1 = 0;
        y2 = square[0].y();
        bound_counter[2] = searchTheLight(x1,y1,x2,y2);
        x1 = square[0].x();
        x2 = square[1].x();
        y1 = square[2].y();
        y2 = image.height()-1;
        bound_counter[3] = searchTheLight(x1,y1,x2,y2);
    }else{
        QMessageBox::critical(this,"No points or image","Put 3 points or open image");
    }
    repaint();
    image = timage;
}

unsigned ImageArea::searchTheLight(unsigned x1, unsigned y1, unsigned x2, unsigned y2){
    unsigned counter=0;
    for (unsigned  i = x1 + 1; i != x2; x1 < x2 ? ++i : --i ) {
        for(unsigned j = y1 + 1; j != y2; y1 < y2 ? ++j : --j) {
            if(qGray(image.pixel(i, j)) >= tre()) {
                counter++;
            }
            if(qGray(image.pixel(i,j)) <= tre() && (qGray(image.pixel(i + 1, j)) > tre() ||
                                                    qGray(image.pixel(i, j + 1)) > tre() ||
                                                    qGray(image.pixel(i - 1, j)) > tre() ||
                                                    qGray(image.pixel(i, j - 1)) > tre() )) {
                image.setPixel(i, j, qRgb(0,255,0));
            }
        }
    }
    return counter;
}

void ImageArea::switchMode()
{
    d3 = !d3;
    repaint();
}

void ImageArea::saveImage()
{
    if(!image.isNull()){
        image.save(QFileDialog::getSaveFileName(this,tr("Save image"), "", tr("Image files (*.bmp)")));
    }
}

void ImageArea::saveConf(bool def)
{
    static bool fp = true;
    static QString filename = QString("default.conf");
    if(!def){
        filename = QFileDialog::getSaveFileName(this, tr("Save config"), "", tr("Config files (*.conf)"));
        if(fp){
            QFile file(filename);
            if(file.open(QFile::WriteOnly)){
                QTextStream str(&file);
                str << crop[0].x() << " " << crop[0].y() << "\n";
                str << crop[1].x() << " " << crop[1].y() << "\n";
                str << square[0].x() << " " << square[0].y() << "\n";
                str << square[1].x() << " " << square[1].y() << "\n";
                str << square[2].x() << " " << square[2].y() << "\n";
                QMessageBox::information(this, tr("Next step"), tr("Put 3 points then press Save again"));
            }else{
                QMessageBox::warning(this, tr("Error"), tr("Can not open a file"));
            }
            fp = false;
        }else{
            QFile file(filename);
            if(file.open(QFile::Append)){
                QTextStream str(&file);
                str << square[0].x() << " " << square[0].y() << "\n";
                str << square[1].x() << " " << square[1].y() << "\n";
                str << square[2].x() << " " << square[2].y() << "\n";
            }else{
                QMessageBox::warning(this, tr("Error"), tr("Can not open a file"));
            }
            fp = true;
        }
    }else{
        QFile file(filename);
        if(file.open(QFile::WriteOnly)){
            QTextStream str(&file);
            str << conf.crop[0].x() << " " << conf.crop[0].y() << "\n";
            str << conf.crop[1].x() << " " << conf.crop[1].y() << "\n";
            str << conf.square[0].x() << " " << conf.square[0].y() << "\n";
            str << conf.square[1].x() << " " << conf.square[1].y() << "\n";
            str << conf.square[2].x() << " " << conf.square[2].y() << "\n";
            str << conf.square0[0].x() << " " << conf.square0[0].y() << "\n";
            str << conf.square0[1].x() << " " << conf.square0[1].y() << "\n";
            str << conf.square0[2].x() << " " << conf.square0[2].y() << "\n";
        }
    }

}

void ImageArea::loadConf(bool def)
{
    QString filename = "default.conf";
    if(!def){
        filename = QFileDialog::getOpenFileName(this, tr("Open config"), "", tr("Config files (*.conf)"));
    }
    QFile file(filename);
    if(file.open(QFile::ReadOnly)){
        QTextStream str(&file);
        str >> conf.crop[0].rx() >> conf.crop[0].ry();
        str >> conf.crop[1].rx() >> conf.crop[1].ry();
        str >> conf.square[0].rx() >> conf.square[0].ry();
        str >> conf.square[1].rx() >> conf.square[1].ry();
        str >> conf.square[2].rx() >> conf.square[2].ry();
        str >> conf.square0[0].rx() >> conf.square0[0].ry();
        str >> conf.square0[1].rx() >> conf.square0[1].ry();
        str >> conf.square0[2].rx() >> conf.square0[2].ry();
        if(str.status() & QTextStream::ReadCorruptData || str.status() & QTextStream::ReadPastEnd){
            QMessageBox::warning(this, "Error", "Can not read config!");
            return;
        }
    }else{
        repaint();
    }
    counter = 3;
}

void ImageArea::align()
{
    QMatrix matrix;
    matrix.shear(4*(square[1].x() - square[2].x())/(double)image.width(), 4*(square[0].y() - square[1].y())/(double)image.height());
    image = image.transformed(matrix);
    counter = 0;
    repaint();
}

void ImageArea::autorun()
{
    crop[0] = conf.crop[0];
    crop[1] = conf.crop[1];
    square[0] = conf.square[0];
    square[1] = conf.square[1];
    square[2] = conf.square[2];
    image = image.copy(crop[0].x(),crop[0].y(),crop[1].x()-crop[0].x(),crop[1].y()-crop[0].y());
    counter = 3;
    align();
    counter = 3;
    square[0] = conf.square0[0];
    square[1] = conf.square0[1];
    square[2] = conf.square0[2];
    run();
    sum = QtConcurrent::run(&conv, &Converter::calculate, res, pres, std::accumulate(&bound_counter[0], bound_counter+4,0)).result();
    repaint();
}

void ImageArea::calibrate()
{
    loadConf(false);
    QStringList names = QFileDialog::getOpenFileNames(this,tr("Open images"), "", tr("Images (*.bmp)"));
    res.clear();
    pres.clear();
    for(QStringList::iterator it = names.begin(); it != names.end(); ++it){
        image.load(*it);
        for(int i = 0; i < image.width(); ++i){
            for(int j = 0; j < image.height(); ++j){
                int gray = qGray(image.pixel(i,j));
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
        }
        sharpen();
        int a = (*it).lastIndexOf('.');
        (*it).chop((*it).size() - a);
        a = (*it).lastIndexOf('/');
        (*it) = (*it).right((*it).size() - a - 1 );
        int n = (*it).toInt();
        autorun();
        res.push_back(bound_counter[0]+bound_counter[1]+bound_counter[2]+bound_counter[3]);
        pres.push_back(n);
    }

}

void ImageArea::getFrame(int n)
{
    fileName = QString("frame_")+QString::number(n)+QString(".bmp");
    loadImage();
    repaint();
}

void ImageArea::loadData()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Save data"), "", tr("Data (*.dat)"));
    QFile file(filename);
    int r = 0;
    double p = 0;
    res.clear();
    pres.clear();
    if(file.open(QFile::ReadOnly)){
        QTextStream str(&file);
        while(!str.atEnd()){
            str >> r >> p;
            res.push_back(r);
            pres.push_back(p);
        }
    }else{
        repaint();
    }
}

void ImageArea::saveData()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save data"), "", tr("Data (*.dat)"));
    QFile file(filename);
    if(file.open(QFile::WriteOnly)){
        QTextStream str(&file);
        for(int i = 0; i < res.size(); ++i){
            str << res[i] << " " << pres[i] << " \n";
        }
    }else{
        repaint();
    }
}

void  ImageArea::sharpen(){
    QImage oldImage = image;
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
    repaint();
}

ImageArea::~ImageArea()
{
    delete ui;
}
