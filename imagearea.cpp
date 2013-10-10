#include "imagearea.hpp"
#include "ui_imagearea.h"

#include <QtConcurrent/QtConcurrent>
ImageArea::ImageArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageArea)
{
    ui->setupUi(this);
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
        //pix.save("pix.bmp");
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
    painter.drawText(30,30,QString::number(image.width())+QString(", ")+QString::number(image.height())+", "+QString::number(threshold)+ QString(" ") + QString::number(d3));
    painter.drawText(30,50,
                     QString("left: ")+QString::number(bound_counter[2])+
            QString(" up: ")+QString::number(bound_counter[1])+
            QString(" right: ")+QString::number(bound_counter[0])+
            QString(" down: ")+QString::number(bound_counter[3])+
            QString(" ave: ")+QString::number((bound_counter[2]+bound_counter[1]+bound_counter[0]+bound_counter[3])/4.)+
            QString(" sum: ")+QString::number(bound_counter[2]+bound_counter[1]+bound_counter[0]+bound_counter[3]) );
    /*if(!image.isNull()){
        randomgen();
        for(QRect x : randrect){
            if(x.height() == 40){
                //painter.setPen(Qt::yellow);
                //painter.drawRect(x);
            }else{
                painter.setPen(Qt::red);
                painter.drawRect(x.left()-10,x.top()-10,40,40);

            }
            painter.setPen(Qt::white);
        }
    }*/
    e->accept();
    releaseMouse();
}

void ImageArea::keyPressEvent(QKeyEvent *e){
    if(e->key() == Qt::Key_O && e->modifiers() == Qt::ControlModifier){
        open();
    }
    if(e->key() == Qt::Key_R && e->modifiers() == Qt::ControlModifier){
        reset();
    }
    if(e->key() == Qt::Key_E && e->modifiers() == Qt::ControlModifier){
        qDebug() << "We are here!";
        d3 = !d3;
    }

    repaint();
    e->accept();
}

void ImageArea::mousePressEvent(QMouseEvent *e){
    if(e->button() == Qt::RightButton && !image.isNull() ){
        rect = true;
        crop[0].setX(e->x());
        crop[0].setY(e->y());
        repaint();
    }
    if(e->button() == Qt::LeftButton){
        zoom_b = true;
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

void ImageArea::open()
{
    fileName = QFileDialog::getOpenFileName( this, tr("Open data file"), "", tr("Image files (*.bmp)"));
    load();
    repaint();
}

void ImageArea::reset()
{
    load();
    repaint();
}

void ImageArea::load(){
    image.load(fileName);
    for(int i = 0; i < image.width(); ++i){
        for(int j = 0; j < image.height(); ++j){
            int gray = qGray(image.pixel(i,j));
            image.setPixel(i,j,qRgb(gray,gray,gray));
        }
    }
    sharpen();
    counter = 0;
}

void ImageArea::run()
{
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
    image.save("test.bmp");
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
}

void ImageArea::saveImage()
{
    if(!image.isNull()){
        image.save(QFileDialog::getSaveFileName(this,tr("Save image"), "", tr("Image files (*.bmp)")));
    }
}

void ImageArea::saveConf()
{
    static bool fp = true;
    static QString filename = QString("save.conf");
    if(fp){
        filename = QFileDialog::getSaveFileName(this, tr("Save config"), "", tr("Config files (*.conf)"));
        QFile file(filename);
        if(file.open(QFile::WriteOnly)){
            QTextStream str(&file);
            str << crop[0].x() << " " << crop[0].y() << "\n";
            str << crop[1].x() << " " << crop[1].y() << "\n";
            str << square[0].x() << " " << square[0].y() << "\n";
            str << square[1].x() << " " << square[1].y() << "\n";
            str << square[2].x() << " " << square[2].y() << "\n";
            QMessageBox::information(this, tr("Next step"), tr("Put 3 points then press Save again"));
        }
        fp = false;
    }else{
        QFile file(filename);
        if(file.open(QFile::Append)){
            QTextStream str(&file);
            str << square[0].x() << " " << square[0].y() << "\n";
            str << square[1].x() << " " << square[1].y() << "\n";
            str << square[2].x() << " " << square[2].y() << "\n";
        }
        fp = true;
    }

}

void ImageArea::loadConf()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open config"), "", tr("Config files (*.conf)"));
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
    }else{
        repaint();
    }
}

void ImageArea::align()
{
    QMatrix matrix;
    matrix.shear(4*(square[1].x() - square[2].x())/(double)image.width(), 4*(square[0].y() - square[1].y())/(double)image.height());
    image = image.transformed(matrix);
    counter = 0;
    repaint();
    /*qDebug() << "We are aligning"  << crop[0].x() << " " << crop[0].y() << "\n"
             << crop[1].x() << " " << crop[1].y() << "\n"
             << square[0].x() << " " << square[0].y() << "\n"
             << square[1].x() << " " << square[1].y() << "\n"
             << square[2].x() << " " << square[2].y() << "\n";*/
}

void ImageArea::randomgen(){
    randrect.clear();
    srand(time(NULL));
    for(int i = 0; i < 10000; ++i){
        int x = rand() % (image.width() - 40) + 20;
        int y = rand() % (image.height() - 40) + 20;
        int sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
        for(int i = 0; i < 500; ++i){
            int x1 = rand() % 41 - 20;
            int y1 = rand() % 41 - 20;
            if(x1 > 0 && y1 < 0){
                sum1+= qGray(image.pixel(x+x1, y+y1));
            }
            if(x1 > 0 && y1 > 0){
                sum2+= qGray(image.pixel(x+x1, y+y1));
            }
            if(x1 < 0 && y1 > 0){
                sum3+= qGray(image.pixel(x+x1, y+y1));
            }
            if(x1 < 0 && y1 < 0){
                sum4+= qGray(image.pixel(x+x1, y+y1));
            }
            if(sum1 - sum3 > 300 && sum2 - sum3 > 300 && sum4 - sum2 > 300 && sum3 < 200 && sum2 > 600){
                randrect.push_back(QRect(x - 10, y - 10, 20, 20));
                qDebug() << "Yabadabadu!";
            }
            //qDebug() << sum1 << sum2 << sum3 << sum4;
        }
        // randrect.push_back(QRect(x - 20, y - 20, 40, 40));
    }
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
}

void ImageArea::calibrate()
{
    loadConf();
    QStringList names = QFileDialog::getOpenFileNames(this,tr("Open images"), "", tr("Images (*.bmp)"));
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
        res[n] = bound_counter[0]+bound_counter[1]+bound_counter[2]+bound_counter[3];
        qDebug() << n << res[n];
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
