#include "imageprocessor.hpp"
#include <QtConcurrent/QtConcurrent>

ImageProcessor::ImageProcessor(QObject* parent) : QObject(parent)
{
}

void ImageProcessor::openImage(const QStringList &names)
{
    fileNameV.clear();
    fileNames = names;//QFileDialog::getOpenFileNames( this, tr("Open data file"), "", tr("Image files (*.bmp)"));
    if(images){
        delete[] images;
    }
    if(!fileNames.isEmpty()){
        images = new Image[fileNames.length()];
        for(curr = 0; curr < static_cast<unsigned>(fileNames.length()); ++curr){
            loadImage();
        }
        curr = 0;
        emit needRepaint();
    }
}

int ImageProcessor::openVideo(const QString &name)
{
    fileNames.clear();
    fileNameV.clear();
    fileNameV = name;//QFileDialog::getOpenFileName( this, tr("Open video file"), "", tr("Video files (*.avi)"));
    if(!fileNameV.isEmpty()){
        fn = QtConcurrent::run(&conv, &Converter::processVideo, fileNameV);
        QTimer::singleShot(300, this, SLOT(checkVideoProcess()) );
        return 0;
    }else{
        return 0;
    }
}

void ImageProcessor::reset()
{
    loadImage();
    origin[0] = {0, 0};
    origin[1] = {0, 0};
    for(int i = 0; i < 4 ; ++i){
        lasts[i].clear();
        hull[i].clear();
    }
    images[curr].r = false;
    emit needRepaint();
}

void ImageProcessor::loadImage(){
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
        images[curr].l = true;
        emit imageChanged(fileNames[curr]);
    }else{
        emit somethingWentWrong("Error", "Can not load an image(s)");
    }
}

void ImageProcessor::run()
{
    Image * image = &images[curr];
    QImage timage = image->image;
    for(unsigned& x : image->bound_counter){
        x = 0;
    }
    if(d3 && !image->image.isNull()){
        QFuture<QVector<Line> > tmp = QtConcurrent::run(&conv, &Converter::convert, image->image, mode);
        lines = tmp.result();
        emit needRepaint();
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
        emit somethingWentWrong("No points or image","Put 3 points or open image");
    }
    if(!vid)emit needRepaint();
    image->image = timage;
    
}

void ImageProcessor::prev()
{
    if(!curr){
        return;
    }else{
        curr--;
        emit needRepaint();
        emit imageUpdated(fileNames[curr]);
    }
}

void ImageProcessor::next()
{
    if(curr == (unsigned)fileNames.length()-1){
        return;
    }else{
        curr++;
        emit needRepaint();
        emit imageUpdated(fileNames[curr]);
    }
}

void ImageProcessor::checkVideoProcess()
{
    if(fn.isFinished()){
        emit somethingWentWrong("It's done!","Video has been opened!");
        //this->setCursor(Qt::ArrowCursor);
        frame_num = fn.result();
        emit giveFramesNumber(frame_num);
    }
    else {
        QTimer::singleShot(300, this, SLOT(checkVideoProcess()) );
    }
}

unsigned ImageProcessor::searchTheLight(unsigned x1, unsigned y1, unsigned x2, unsigned y2/*, int k*/){
    QImage * image = &images[curr].image;
    unsigned counter=0;
    for (unsigned  i = x1 + 1; i != x2; x1 < x2 ? ++i : --i ) {
        for(unsigned j = y1 + 1; j != y2; y1 < y2 ? ++j : --j) {
            if(qGray(image->pixel(i, j)) >= tre()) {
                counter++;
            }
        }
    }
    return counter;
}

void ImageProcessor::switchMode()
{
    d3 = !d3;
    run();
}

void ImageProcessor::saveImage(const QString & name)
{
    if(!images[curr].image.isNull()){
        images[curr].image.save(name);
    }
}

void ImageProcessor::saveConf(const QString& name, bool def)
{
    Image * image = &images[curr];
    static bool fp = true;
    static QString filename = QString("default.conf");
    if(!def){
        if(fp){
            filename = name;
            QFile file(filename);
            if(file.open(QFile::WriteOnly)){
                QTextStream str(&file);
                str << image->crop[0].x() << " " << image->crop[0].y() << "\n";
                str << image->crop[1].x() << " " << image->crop[1].y() << "\n";
                str << image->square[0].x() << " " << image->square[0].y() << "\n";
                str << image->square[1].x() << " " << image->square[1].y() << "\n";
                str << image->square[2].x() << " " << image->square[2].y() << "\n";
                emit somethingWentWrong("Next step","Put 3 points then press Save again");
            }else{
                emit somethingWentWrong("Error","Can not open a file");
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
                emit somethingWentWrong("Error","Can not open a file");
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
            emit somethingWentWrong("Error","Can not open a file");
        }
    }
    
}

void ImageProcessor::loadConf(const QString &name)
{
    Image * image = &images[curr];
    QString filename = name;//"default.conf";
   /* if(!def){
        filename = name;//QFileDialog::getOpenFileName(this, tr("Open config"), "", tr("Config files (*.conf)"));
    }*/
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
            emit somethingWentWrong("Error", "Can not read config!");
            return;
        }
    }else{
        emit somethingWentWrong("Error","Can not open a file");
        emit needRepaint();
    }
    image->conf = conf;
    image->counter = 3;
    
}

void ImageProcessor::align()
{
    Image * image = &images[curr];
    QMatrix matrix;
    matrix.shear(4*(image->square[1].x() - image->square[2].x())/(double)image->image.width(), 4*(image->square[0].y() - image->square[1].y())/(double)image->image.height());
    image->image = image->image.transformed(matrix);
    image->counter = 0;
    if(!vid)emit needRepaint();
}

void ImageProcessor::autorun()
{
    if(!fileNameV.isEmpty()){
        vid = true;
        fileNames.clear();
        delete[] images;
        images = new Image[frame_num];
        QList<QFuture<double>> sums;
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
            //image->sum = QtConcurrent::run(&conv, &Converter::calculate, res, pres, std::accumulate(&image->bound_counter[0], image->bound_counter+4,0)/1000.).result();
            image->sum = conv.calculate(res, pres, std::accumulate(&image->bound_counter[0], image->bound_counter+4,0)/1000.);
            vres.push_back(images[curr].sum);
            vres0.push_back(std::accumulate(&image->bound_counter[0], image->bound_counter+4,0));
            //image->image.save(QString::number(image->sum) + QString(".bmp"));
            if(!cont){
                cont = true;
                delete[] images;
                vres.clear();
                vres0.clear();
                break;
            }
        }
        vid = false;
    }else{
        Image * image = &images[curr];
        if(!image->r && image->l){
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
            image->r = true;
            emit needRepaint();
        }else{
            emit somethingWentWrong("Fail", "Image already processed or not loaded");
        }
    }
}

void ImageProcessor::calibrate(const QString &name, const QStringList &names)
{
    loadConf(name);
    QStringList fnames = names;//QFileDialog::getOpenFileNames(this,tr("Open images"), "", tr("Images (*.bmp)"));
    delete[] images;
    images = new Image[1];
    res.clear();
    pres.clear();
    for (int i = 0; i < 4; ++i) {
        res4[i].clear();
    }
    curr = 0;
    fileNames.append(fnames[0]);
    for(QStringList::iterator it = fnames.begin(); it != fnames.end(); ++it){
        fileNames[0] = *it;
        images[0].image.load(*it);
        images[0].l = true;
        images[0].r = false;
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
        for (int i = 0; i < 4; ++i) {
            res4[i].push_back(images[0].bound_counter[i]/1000.);
        }
        res.push_back((images[0].bound_counter[0] + images[0].bound_counter[1] + images[0].bound_counter[2] + images[0].bound_counter[3])/1000.);
        pres.push_back(n);
    }
}

void ImageProcessor::saveResults(const QString &name)
{
    QFile file(name);//QFileDialog::getSaveFileName(this,tr("Save results"), "", tr("Data (*.dat)")));
    if(file.open(QFile::WriteOnly)){
        QTextStream str(&file);
        for(int i = 0; i < vres.size(); ++i){
            str << vres0[i] << " " << vres[i] << "\n";
        }
    }
}

void ImageProcessor::getFrame(int n)
{
    fileNames.clear();
    fileNames.push_back(QString("frame_")+QString::number(n)+QString(".bmp"));
    delete[] images;
    images = new Image[1];
    curr = 0;
    loadImage();
    emit needRepaint();
}

void ImageProcessor::loadData(const QString &name)
{
    QString filename = name;//QFileDialog::getOpenFileName(this,tr("Load data"), "", tr("Data (*.dat)"));
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
        emit needRepaint();
    }
}

void ImageProcessor::saveData(const QString &name)
{
    QString filename = name;//QFileDialog::getSaveFileName(this,tr("Save data"), "", tr("Data (*.dat)"));
    QFile file(filename);
    if(file.open(QFile::WriteOnly)){
        QTextStream str(&file);
        for(int i = 0; i < res.size(); ++i){
            str << res4[0][i] << " " << res4[1][i] << " " << res4[2][i] << " " << res4[3][i] << " ";
            str << res[i] << " " << pres[i] << " \n";
        }
    }else{
        emit needRepaint();
    }
}

void  ImageProcessor::sharpen(){
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
    emit needRepaint();
}

