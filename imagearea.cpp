#include "imagearea.hpp"
#include "ui_imagearea.h"

#include <QtConcurrent/QtConcurrent>

ImageArea::ImageArea(QWidget *parent) :
    QWidget(parent),
    images{new Image[1]},
    ui(new Ui::ImageArea)
{
    ui->setupUi(this);
    loadConf(true);
}

void ImageArea::paintEvent(QPaintEvent *e){
    Image * image = &images[curr];
    grabMouse();
    QPainter painter(this);
    if(d3 && !image->image.isNull()){
        std::sort(lines.begin(),lines.end(), [](Line a, Line b) { return a.z1 < b.z1; });
        QImage pix(this->width(),this->height(), image->image.format());
        pix.fill(Qt::black);
        QPainter p;
        p.begin(&pix);
        for(Line l : lines){
            if(l.c >= tre()){
                p.setPen(qRgb(0, 255, 0));
            }else{
                p.setPen(qRgb(l.c, l.c, l.c));
            }
            p.drawLine(l.x1 + 300, l.y1 + 500, l.x2 + 300,l.y2 + 500);
        }
        painter.drawImage(0, 0, pix);
        e->accept();
        releaseMouse();
        return;
    }
    painter.drawImage(origin[1].x(), origin[1].y(), image->image);
    if(image->counter > 0){
        for(unsigned i = 0; i < image->counter; ++i){
            painter.setPen(Qt::red);
            painter.drawLine(image->square[i].x() - 4, image->square[i].y(), image->square[i].x() + 4, image->square[i].y());
            painter.drawLine(image->square[i].x(), image->square[i].y() - 4, image->square[i].x(), image->square[i].y() + 4);
            painter.drawText(image->square[i].x() + 3, image->square[i].y() - 3, QString::number(i+1));
            painter.setPen(Qt::white);
        }
    }
    if(rect){
        painter.setPen(Qt::green);
        painter.drawRect(image->crop[0].x(), image->crop[0].y(), image->crop[1].x() - image->crop[0].x(), image->crop[1].y() - image->crop[0].y());
        painter.setPen(Qt::white);
    }else{
        if(zoom_b){
            QImage zoomed = image->image.copy(zoom.x() - origin[1].x() - 10, zoom.y() - origin[1].y() - 10, 20, 20).scaled(41, 41);
            zoomed.setPixel(20, 20, qRgb(0, 255, 0));
            painter.drawImage(zoom.x(), zoom.y(), zoomed);
        }
    }
    emit giveImage(*image);
    /*painter.drawText(30,30, QString("threshold: ") +QString::number(threshold) + ", pressure: " + QString::number(sum));
    painter.drawText(30, 50,
                     QString("left: ")+QString::number(bound_counter[2])+
            QString(" up: ")+QString::number(bound_counter[1])+
            QString(" right: ")+QString::number(bound_counter[0])+
            QString(" down: ")+QString::number(bound_counter[3])+
            QString(" ave: ")+QString::number((bound_counter[2]+bound_counter[1]+bound_counter[0]+bound_counter[3])/4.)+
            QString(" sum: ")+QString::number(bound_counter[2]+bound_counter[1]+bound_counter[0]+bound_counter[3]) );
    */
    for(int i = 0 ; i <  4; ++i){
        auto dbs = conv.dbscan(lasts[i]);
        for(auto it = lasts[i].begin(); it != lasts[i].end(); ++it){
            if(dbs[i] == -1){
               painter.setPen(Qt::red);
               painter.drawPoint(*it);
            }else if(dbs[i] != 1){
                painter.setPen(Qt::blue);
                painter.drawPoint(*it);
            }else{
                painter.setPen(Qt::green);
                painter.drawPoint(*it);
            }
        }
        if(!hull[i].isEmpty()){
            painter.setPen(Qt::green);
            //painter.drawPolygon(hull[i].data(),hull[i].size());
        }
    }
    if(image->sum <= GY){
        painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
        painter.setPen(Qt::green);
    }else if(image->sum > GY && image->sum < YR){
        painter.setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
        painter.setPen(Qt::yellow);
    }else if(image->sum >= YR){
        painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        painter.setPen(Qt::red);
    }
    painter.drawEllipse(this->width() - 100, this->height() - 100, 80, 80);

    e->accept();
    releaseMouse();

}

void ImageArea::mousePressEvent(QMouseEvent *e){
    Image * image = &images[curr];
    if(e->button() == Qt::RightButton && !image->image.isNull() ){
        rect = true;
        image->crop[0].setX(e->x()/* + origin[1].x()*/);
        image->crop[0].setY(e->y() /*+ origin[1].y()*/);
        repaint();
    }
    if(e->button() == Qt::LeftButton && !image->image.isNull()){
        zoom_b = true;
        zoom.setX(e->x()/*+ origin[1].x()*/);
        zoom.setY(e->y()/*+ origin[1].y()*/);
        repaint();
    }
    if(e->button() == Qt::MiddleButton && !image->image.isNull()){
        origin[0].setX(origin[0].x() + e->x());
        origin[0].setY(origin[0].y() + e->y());
    }

}

void ImageArea::mouseMoveEvent(QMouseEvent *e){
    Image * image = &images[curr];
    if(!image->image.isNull()){
        if(rect){
            image->crop[1].setX(e->x());
            image->crop[1].setY(e->y());

        }else{
            zoom.setX(e->x() /*+ origin[1].x()*/);
            zoom.setY(e->y() /*+ origin[1].y()*/);
        }
    }if(e->buttons() & Qt::MiddleButton && !image->image.isNull()){
        origin[1].setX(e->x() - origin[0].x());
        origin[1].setY(e->y() - origin[0].y());
    }
    repaint();
}

void ImageArea::mouseReleaseEvent(QMouseEvent *e){
    Image * image = &images[curr];
    if(e->button() == Qt::RightButton && !image->image.isNull()){
        if(image->crop[0].x() > image->crop[1].x()){
            int tmp = image->crop[0].x();
            image->crop[0].setX(image->crop[1].x());
            image->crop[1].setX(tmp);
        }
        if(image->crop[0].y() > image->crop[1].y()){
            int tmp = image->crop[0].y();
            image->crop[0].setY(image->crop[1].y());
            image->crop[1].setY(tmp);
        }
        image->image = image->image.copy(image->crop[0].x()- origin[1].x(),image->crop[0].y()-origin[1].y(),image->crop[1].x() - image->crop[0].x(),image->crop[1].y() - image->crop[0].y());
        origin[0].setX(0);
        origin[0].setY(0);
        origin[1].setX(0);
        origin[1].setY(0);
        rect = false;
    }
    if(e->button() == Qt::LeftButton && !image->image.isNull()){
        if(image->counter < 3){
            image->square[image->counter].setX(e->x());
            image->square[image->counter].setY(e->y());
            image->counter++;
        }else{
            image->counter = 0;
        }
        zoom_b = false;
    }
    if(e->button() == Qt::MiddleButton && !image->image.isNull()){
        origin[0].setX(origin[0].x()- e->x());
        origin[0].setY(origin[0].y()- e->y());
    }
    repaint();
    e->accept();
}

void ImageArea::wheelEvent(QWheelEvent *e){
    Image * image = &images[curr];
    qint32 a = image->threshold + e->delta()/80;
    if( a > 255 ) a = 255;
    if( a < 0 ) a = 0;
    image->threshold = quint8(a);
    if(!image->image.isNull()){
        run();
    }
}

void ImageArea::openImage()
{
    fileNameV.clear();
    fileNames = QFileDialog::getOpenFileNames( this, tr("Open data file"), "", tr("Image files (*.bmp)"));
    if(images){
        delete[] images;
    }
    images = new Image[fileNames.length()];
    for(curr = 0; curr < static_cast<unsigned>(fileNames.length()); ++curr){
        loadImage();
    }
    curr = 0;
    repaint();
}

int ImageArea::openVideo()
{
    fileNames.clear();
    fileNameV.clear();
    fileNameV = QFileDialog::getOpenFileName( this, tr("Open data file"), "", tr("Video files (*.avi)"));
    this->setCursor(Qt::WaitCursor);
    QFuture<int> fn = QtConcurrent::run(&conv, &Converter::processVideo, fileNameV);
    frame_num = fn.result();
    this->setCursor(Qt::ArrowCursor);
    return frame_num;
}

void ImageArea::reset()
{
    loadImage();
    origin[0] = {0, 0};
    origin[1] = {0, 0};
    for(int i = 0; i < 4 ; ++i){
        lasts[i].clear();
        hull[i].clear();
    }
    repaint();
}

void ImageArea::loadImage(){
    QImage * image = &images[curr].image;
    if(image->load(fileNames[curr])){
        for(int i = 0; i < image->width(); ++i){
            for(int j = 0; j < image->height(); ++j){
                int gray = qGray(image->pixel(i,j));
                image->setPixel(i,j,qRgb(gray,gray,gray));
            }
        }
        sharpen();
        images[curr].counter = 0;
        emit imageChanged(fileNames[curr]);
    }else{
        QMessageBox::critical(this, tr("Error"), tr("Can not load an image(s)"));
    }
}

void ImageArea::run()
{


    Image * image = &images[curr];
    QImage timage = image->image;
    for(unsigned& x : image->bound_counter){
        x = 0;
    }
    if(d3 && !image->image.isNull()){
        QFuture<QVector<Line> > tmp = QtConcurrent::run(&conv, &Converter::convert, image->image, mode);
        lines = tmp.result();
        repaint();
        return;
    }
    if( image->counter == 3 && !image->image.isNull()){
        for (int i = 0; i < 4; ++i) {
            lasts[i].clear();
        }
        unsigned x1 = image->square[1].x()-origin[1].x();
        unsigned x2 = image->image.width()-1;
        unsigned y1 = image->square[1].y()-origin[1].y();
        unsigned y2 = image->square[2].y()-origin[1].y();
        image->bound_counter[0] = searchTheLight(x1,y1,x2,y2);
        x1 = 0;
        x2 = image->square[0].x()-origin[1].x();
        y1 = image->square[0].y()-origin[1].y();
        y2 = image->square[2].y()-origin[1].y();
        image->bound_counter[1] = searchTheLight(x1,y1,x2,y2);
        x1 = image->square[0].x()-origin[1].x();
        x2 = image->square[1].x()-origin[1].x();
        y1 = 0;
        y2 = image->square[0].y()-origin[1].y();
        image->bound_counter[2] = searchTheLight(x1,y1,x2,y2);
        x1 = image->square[0].x()-origin[1].x();
        x2 = image->square[1].x()-origin[1].x();
        y1 = image->square[2].y()-origin[1].y();
        y2 = image->image.height()-1;
        image->bound_counter[3] = searchTheLight(x1,y1,x2,y2);
    }else{
        QMessageBox::critical(this,"No points or image","Put 3 points or open image");
    }
    repaint();
    image->image = timage;

}

void ImageArea::prev()
{
    if(!curr){
        return;
    }else{
        curr--;
        repaint();
    }
}

void ImageArea::next()
{
    if(curr == (unsigned)fileNames.length()-1){
        return;
    }else{
        curr++;
        repaint();
    }
}

unsigned ImageArea::searchTheLight(unsigned x1, unsigned y1, unsigned x2, unsigned y2){
    QImage * image = &images[curr].image;
    unsigned counter=0;
    for (unsigned  i = x1 + 1; i != x2; x1 < x2 ? ++i : --i ) {
        for(unsigned j = y1 + 1; j != y2; y1 < y2 ? ++j : --j) {
            if(qGray(image->pixel(i, j)) >= tre()) {
                counter++;
            }
            /* if(qGray(image->pixel(i,j)) <= tre() && (qGray(image->pixel(i + 1, j)) > tre() ||
                                                     qGray(image->pixel(i, j + 1)) > tre() ||
                                                     qGray(image->pixel(i - 1, j)) > tre() ||
                                                     qGray(image->pixel(i, j - 1)) > tre() )) {
                image->setPixel(i, j, qRgb(0,255,0));
            }*/
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
    if(!images[curr].image.isNull()){
        images[curr].image.save(QFileDialog::getSaveFileName(this,tr("Save image"), "", tr("Image files (*.bmp)")));
    }
}

void ImageArea::saveConf(bool def)
{
    Image * image = &images[curr];
    static bool fp = true;
    static QString filename = QString("default.conf");
    if(!def){
        if(fp){
            filename = QFileDialog::getSaveFileName(this, tr("Save config"), "", tr("Config files (*.conf)"));
            QFile file(filename);
            if(file.open(QFile::WriteOnly)){
                QTextStream str(&file);
                str << image->crop[0].x() << " " << image->crop[0].y() << "\n";
                str << image->crop[1].x() << " " << image->crop[1].y() << "\n";
                str << image->square[0].x() << " " << image->square[0].y() << "\n";
                str << image->square[1].x() << " " << image->square[1].y() << "\n";
                str << image->square[2].x() << " " << image->square[2].y() << "\n";
                QMessageBox::information(this, tr("Next step"), tr("Put 3 points then press Save again"));
            }else{
                QMessageBox::warning(this, tr("Error"), tr("Can not open a file"));
            }
            fp = false;
        }else{
            QFile file(filename);
            if(file.open(QFile::Append)){
                QTextStream str(&file);
                str << image->square[0].x() << " " << image->square[0].y() << "\n";
                str << image->square[1].x() << " " << image->square[1].y() << "\n";
                str << image->square[2].x() << " " << image->square[2].y() << "\n";
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
        }else{
            QMessageBox::warning(this, tr("Error"), tr("Can not open a file"));
        }
    }

}

void ImageArea::loadConf(bool def)
{
    Image * image = &images[curr];
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
        QMessageBox::warning(this, tr("Error"), tr("Can not open a file"));
        repaint();
    }
    image->conf = conf;
    image->counter = 3;

}

void ImageArea::align()
{
    Image * image = &images[curr];
    QMatrix matrix;
    matrix.shear(4*(image->square[1].x() - image->square[2].x())/(double)image->image.width(), 4*(image->square[0].y() - image->square[1].y())/(double)image->image.height());
    image->image = image->image.transformed(matrix);
    image->counter = 0;
    repaint();
}

void ImageArea::autorun()
{
    if(!fileNameV.isEmpty()){
        fileNames.clear();
        delete[] images;
        images = new Image[frame_num];
        for(unsigned i = 0 ; i < frame_num; ++i){
            fileNames.push_back(QString("frame_")+QString::number(i)+QString(".bmp"));
            curr = i;
            loadImage();
            Image * image = &images[curr];
            image->crop[0] = conf.crop[0];
            image->crop[1] = conf.crop[1];
            image->square[0] = conf.square[0];
            image->square[1] = conf.square[1];
            image->square[2] = conf.square[2];
            image->image = image->image.copy(image->crop[0].x(), image->crop[0].y(), image->crop[1].x() - image->crop[0].x(), image->crop[1].y() - image->crop[0].y());
            image->counter = 3;
            align();
            image->counter = 3;
            image->square[0] = conf.square0[0];
            image->square[1] = conf.square0[1];
            image->square[2] = conf.square0[2];
            run();
            image->sum = QtConcurrent::run(&conv, &Converter::calculate, res, pres, std::accumulate(&image->bound_counter[0], image->bound_counter+4,0)/1000.).result();
            repaint();
            vres.push_back(images[curr].sum);
            vres0.push_back(std::accumulate(&image->bound_counter[0], image->bound_counter+4,0));
        }
    }else{
        Image * image = &images[curr];
        image->crop[0] = conf.crop[0];
        image->crop[1] = conf.crop[1];
        image->square[0] = conf.square[0];
        image->square[1] = conf.square[1];
        image->square[2] = conf.square[2];
        image->image = image->image.copy(image->crop[0].x(), image->crop[0].y(), image->crop[1].x() - image->crop[0].x(), image->crop[1].y() - image->crop[0].y());
        image->counter = 3;
        align();
        image->counter = 3;
        image->square[0] = conf.square0[0];
        image->square[1] = conf.square0[1];
        image->square[2] = conf.square0[2];
        run();
        searchShape();
        for(int i = 0 ; i < 4 ; ++i){
            auto dbs = conv.dbscan(lasts[i]);
            for(int i = 0; i < lasts[i].size(); ++i){
                if(dbs[i] == -1 || dbs[i] != 1){
                    lasts[i].remove(i);
                }
            }
            hull[i] = conv.gethull(lasts[i]);
            Comparator c(conv.mid(hull[i]));
            std::sort(hull[i].begin(), hull[i].end(), c);
        }
        /*for(int i = 0; i < 4; ++i){
            image->sums[i] = QtConcurrent::run(&conv, &Converter::calculate, res4[i], pres, image->bound_counter[i]/1000.).result();
        }*/
        image->sum = QtConcurrent::run(&conv, &Converter::calculate, res, pres, std::accumulate(&image->bound_counter[0], image->bound_counter+4,0)/1000.).result();
        //image->sum = std::accumulate(&image->sums[0],image->sums+4,0)/4;
        repaint();
    }
}

void ImageArea::calibrate()
{
    loadConf(false);
    QStringList names = QFileDialog::getOpenFileNames(this,tr("Open images"), "", tr("Images (*.bmp)"));
    delete[] images;
    images = new Image[1];
    res.clear();
    pres.clear();
    for (int i = 0; i < 4; ++i) {
        res4[i].clear();
    }
    curr = 0;
    for(QStringList::iterator it = names.begin(); it != names.end(); ++it){
        images[0].image.load(*it);
        for(int i = 0; i < images[0].image.width(); ++i){
            for(int j = 0; j < images[0].image.height(); ++j){
                int gray = qGray(images[0].image.pixel(i,j));
                images[0].image.setPixel(i, j, qRgb(gray, gray, gray));
            }
        }
        sharpen();
        int a = (*it).lastIndexOf('.');
        (*it).chop((*it).size() - a);
        a = (*it).lastIndexOf('/');
        (*it) = (*it).right((*it).size() - a - 1 );
        int n = (*it).toInt();
        autorun();
        //QVector<double> r4 = {images[0].bound_counter[0]/1000., images[0].bound_counter[1]/1000., images[0].bound_counter[2]/1000., images[0].bound_counter[3]/1000. };
        for (int i = 0; i < 4; ++i) {
            res4[i].push_back(images[0].bound_counter[i]/1000.);
        }
        res.push_back((images[0].bound_counter[0] + images[0].bound_counter[1] + images[0].bound_counter[2] + images[0].bound_counter[3])/1000.);
        pres.push_back(n);
    }
}

void ImageArea::getFrame(int n)
{
    fileNames.clear();
    fileNames.push_back(QString("frame_")+QString::number(n)+QString(".bmp"));
    delete[] images;
    images = new Image[1];
    curr = 0;
    loadImage();
    repaint();
}

void ImageArea::loadData()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Load data"), "", tr("Data (*.dat)"));
    QFile file(filename);
    QVector<double> r4;
    r4.resize(4);
    double r = 0;
    double p = 0;
    res.clear();
    pres.clear();
    if(file.open(QFile::ReadOnly)){
        QTextStream str(&file);
        while(1){
            str >> r4[0] >> r4[1] >> r4[2] >> r4[3];
            str >> r >> p;
            if(str.atEnd()) break;
            res4[0].push_back(r4[0]);
            res4[1].push_back(r4[1]);
            res4[2].push_back(r4[2]);
            res4[3].push_back(r4[3]);
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
            str << res4[0][i] << " " << res4[1][i] << " " << res4[2][i] << " " << res4[3][i] << " ";
            str << res[i] << " " << pres[i] << " \n";
        }
    }else{
        repaint();
    }
}

void ImageArea::searchShape()
{
    Image * image = &images[curr];
    QPoint last;
    lasts[0].clear();
    lasts[1].clear();
    lasts[2].clear();
    lasts[3].clear();
    for(auto i = image->square[0].x(); i != image->square[1].x(); ++i){
        last.setX(0);
        last.setY(0);
        for(auto j = image->square[0].y(); j != 0; --j){
            if(qGray(image->image.pixel(i, j)) >= tre()) {
                last.setX(i);
                last.setY(j);
            }
        }
        if(!(last.x() == 0 && last.y() == 0)){
            lasts[0].push_back(last);
        }
    }
    for(auto i = image->square[1].y(); i != image->square[2].y(); ++i){
        last.setX(0);
        last.setY(0);
        for(auto j = image->square[1].x(); j != image->image.width(); ++j){
            if(qGray(image->image.pixel(j, i)) >= tre()) {
                last.setX(j);
                last.setY(i);
            }
        }
        if(!(last.x() == 0 && last.y() == 0)){
            lasts[1].push_back(last);
        }
    }

    for(auto i = image->square[0].x(); i != image->square[1].x(); ++i){
        last.setX(0);
        last.setY(0);
        for(auto j = image->square[2].y(); j != image->image.height(); ++j){
            if(qGray(image->image.pixel(i, j)) >= tre()) {
                last.setX(i);
                last.setY(j);
            }
        }
        if(!(last.x() == 0 && last.y() == 0)){
            lasts[2].push_back(last);
        }
    }

    for(auto i = image->square[0].y(); i != image->square[2].y(); ++i){
        last.setX(0);
        last.setY(0);
        for(auto j = image->square[0].x(); j != 0; --j){
            if(qGray(image->image.pixel(j, i)) >= tre()) {
                last.setX(j);
                last.setY(i);
            }
        }
        if(!(last.x() == 0 && last.y() == 0)){
            lasts[3].push_back(last);
        }
    }


    /*for(auto j = 0; j < 200; ++j){
        for(auto i = 0 ; i < lasts.size()-2; ++i){
            auto x0 = lasts[i].x();
            auto y0 = lasts[i].y();
            auto x1 = lasts[i+1].x();
            auto y1 = lasts[i+1].y();
            auto x2 = lasts[i+2].x();
            auto y2 = lasts[i+2].y();
            if(abs(x1-x0) == 1 && abs(x1-x2) == 1){
                if(abs(y1 - y0) > 5 && abs(y1 - y2) > 5){
                    lasts[i+1].ry() = (y1+y2)/2;
                }
            }
            if(abs(y1-y1) == 1 && abs(y1-y2) == 1){
                if(abs(x1 - x0) > 5 && abs(x1-x2) > 5){
                    lasts[i+1].rx() = (x1+x2)/2;
                }
            }
        }
    }*/
}

void  ImageArea::sharpen(){
    QImage * image = &images[curr].image;
    QImage oldImage = images[curr].image;
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
    for(int x=kernelSize/2; x<image->width()-(kernelSize/2); x++){
        for(int y=kernelSize/2; y<image->height()-(kernelSize/2); y++){
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
            image->setPixel(x,y, qRgb(r,g,b));
        }
    }
    repaint();
}

ImageArea::~ImageArea()
{
    delete ui;
}
