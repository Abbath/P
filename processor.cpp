#include "processor.hpp"

/*!
 * \brief Processor::Processor
 * \param parent
 */
Processor::Processor(QObject *parent) :
    QObject(parent)
{
    images.resize(1);
}

/*!
 * \brief Processor::leastsquares
 * \param x
 * \param yy
 * \return 
 */
std::pair<long double, long double> Processor::leastsquares(const QVector<double> &x, const QVector<double> &yy)const
{
    QVector<double> y = yy;
    for (int i = 0; i < y.size(); ++i) {
        /* if(qFuzzyCompare(y[i],0.0)){
            y[i]+= std::numeric_limits<double>::epsilon();
            
        }*/
        y[i]+=1.0;
    }
    long double A,B,a=0,b=0, at=0, tt = 0, bt =0, tmp=0;
    for(int i = 0; i < x.size(); ++i){
        tmp += x[i]*x[i]*y[i];
    }
    
    a = tmp;
    tmp = 0;
    
    for(int i = 0; i < x.size(); ++i){
        tmp += y[i]*log(y[i]);
    }
    
    a *= tmp;
    tmp = 0;
    
    for(int i = 0; i < x.size(); ++i){
        tmp += y[i]*x[i];
    }
    
    at = tmp;
    tmp = 0;
    
    for(int i = 0; i < x.size(); ++i){
        tmp += y[i]*x[i]*log(y[i]);
    }
    
    at *= tmp;
    tmp = 0;
    
    a -= at;
    
    for(int i = 0; i < x.size(); ++i){
        tmp += y[i];
    }
    
    at = tmp;
    tmp = 0;
    
    for(int i = 0; i < x.size(); ++i){
        tmp += x[i]*x[i]*y[i];
    }
    
    tt = at*tmp;
    tmp = 0;
    
    for(int i = 0; i < x.size(); ++i){
        tmp += x[i]*y[i];
    }
    
    tt -= tmp*tmp;
    tmp = 0;
    
    a /= tt;
    
    A = exp(a);
    
    for(int i = 0; i < x.size(); ++i){
        tmp += y[i];
    }
    
    b = tmp;
    tmp = 0;
    
    for(int i = 0; i < x.size(); ++i){
        tmp += x[i]*y[i]*log(y[i]);
    }
    
    b *= tmp;
    tmp = 0;
    
    for(int i = 0; i < x.size(); ++i){
        tmp += y[i]*x[i];
    }
    
    bt = tmp;
    tmp = 0;
    
    for(int i = 0; i < x.size(); ++i){
        tmp += y[i]*log(y[i]);
    }
    
    bt *= tmp;
    tmp = 0;
    
    b -= bt;
    
    b /= tt;
    
    B = b;
    
    return std::make_pair(A,B);
}

/*!
 * \brief Processor::calculate
 * \param res
 * \param pres
 * \param val
 * \return 
 */
double Processor::calculate(const QVector<double> &res, const QVector<double> &pres, double val) const 
{
    
    /*double sum = 0.0,x1 = 0.0,y1 = 0.0,y0=0.0,x0=0.0;
      
    for(auto it = res.begin(); it != res.end(); ++it){
        if(*it > val){
            if(it == res.begin()){
                x0 = *res.begin();
                y0 = *pres.begin();
                x1 = *(res.begin()+1);
                y1 = *(pres.begin()+1);
                break;
            }
            x1 = *it;
            y1 = pres[it - res.begin()];
            x0 = *(it-1);
            y0 = pres[it - res.begin() - 1];
            break;
        }
        if(it + 1 == res.end()){
            x0 = *(it-1);
            y0 = pres[res.size()-2];
            x1 = *it;
            y1 = pres[res.size()-1];
        }
    }
    sum = y0 + (y1 - y0) * (val - x0) / ( x1 - x0);*/
    
    /*double sum=0,l=1;
      
    for(int j = 0;j < res.size(); ++j){
        for(int i = 0; i < res.size(); ++i){
            if( j != i){
                l*= (val - res[i])/(double)(res[j]-res[i]);
            }
        }
        sum += l*pres[j];
        l = 1;
    }*/
    
    auto p = leastsquares(res,pres);
    double sum = p.first * exp(p.second*val)-1.0;
    return sum;
    
}

/*!
 * \brief Processor::loadImage
 * \param name
 * \return 
 */
QImage Processor::loadImage(const QString &name)
{
    QImage image;
    if ( image.load(name) ) {
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

/*!
 * \brief Processor::prev
 */
void Processor::prev()
{
    if(!curr){
        repaint();
    }else{
        curr--;
        repaint();
    }
}

/*!
 * \brief Processor::next
 */
void Processor::next()
{
    if(curr == (unsigned)fileNames.size()-1){
        repaint();
    }else{
        curr++;
        repaint();
    }
}

/*!
 * \brief Processor::openImage
 * \param names
 */
void Processor::openImage(const QStringList &names)
{
    qDebug() << "I'm in Processor::openImage\n";
    fileNameV.clear();
    fileNames.clear();
    fileNames.reserve(names.size());
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

/*!
 * \brief Processor::die
 */
void Processor::die()
{
    this->deleteLater();
}

/*!
 * \brief Processor::setDisplay
 * \param dis
 */
void Processor::setDisplay(const Display &dis)
{
    images[curr] = dis.im;
    origin[0] = dis.origin[0];
    origin[1] = dis.origin[1];
}

/*!
 * \brief Processor::getDisplay
 * \return 
 */
Display Processor::getDisplay()
{
    Display dis;
    dis.im = images[curr];
    dis.origin[0] = origin[0];
    dis.origin[1] = origin[1];
    return dis;
}

/*!
 * \brief Processor::sharpen
 * \param im
 * \return 
 */
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

/*!
 * \brief Processor::repaint
 */
void Processor::repaint()
{
    
    Display dis;
    dis.im = images[curr];
    if(!fileNames.isEmpty()){
        dis.im.fileName = fileNames[curr];
    }
    dis.origin[0] = origin[0];
    dis.origin[1] = origin[1];
    emit Update(dis);
    
}

/*!
 * \brief Processor::searchTheLight
 * \param image
 * \param tre
 * \param x1
 * \param y1
 * \param x2
 * \param y2
 */
unsigned Processor::searchTheLight(const QImage& image, unsigned tre, unsigned x1, unsigned y1, unsigned x2, unsigned y2){
    unsigned counter=0;
    for (unsigned  i = x1 + 1; i != x2; x1 < x2 ? ++i : --i ) {
        for(unsigned j = y1 + 1; j != y2; y1 < y2 ? ++j : --j) {
            if(static_cast<unsigned>(qGray(image.pixel(i, j))) >= tre) {
                counter++;
            }
        }
    }
    return counter;
}

/*!
 * \brief Processor::run
 */
void Processor::run()
{
    CvCapture * capture = cvCaptureFromAVI(fileNameV.toStdString().c_str());
    if(!capture)
    {
        QMessageBox::warning(0, "Error", "cvCaptureFromAVI failed (file not found?)\n");
        return;
    }
    IplImage* frame = NULL;
    vres.clear();
    vres0.clear();
    vid = true;
    fileNames.clear();
    images.clear();
    int i = 0;
    while ((frame = cvQueryFrame(capture))) {
        if(stop){
            stop = false;
            break;
        }
        curr = i++;
        images.resize(i+1);
        QImage timage = IplImage2QImage(frame);
        for(int i = 0; i < timage.width(); ++i){
            for(int j = 0; j < timage.height(); ++j){
                int gray = qGray(timage.pixel(i,j));
                timage.setPixel(i,j,qRgb(gray,gray,gray));
            }
        }
        timage = sharpen(timage).mirrored();
        
        images[curr].image = timage; 
        Image& image = images[curr];
        image.crop = conf.crop;
        for(int i = 0; i < 3; ++i){
            image.square[i] = conf.square[i];
        }
        image.image = image.image.copy(image.crop);
        image.counter = 3;
        align();
        image.counter = 3;
        for(int i = 0; i < 3; ++i){
            image.square[i] = conf.square0[i];
        }
        run(true);
        image.sum = calculate(res, pres, std::accumulate(&image.bound_counter[0], image.bound_counter+4,0)/1000.);
        qDebug() << image.sum;
        vres.push_back(image.sum);
        vres0.push_back(std::accumulate(&image.bound_counter[0], image.bound_counter+4,0));
        QThread::currentThread()->usleep(500);
        
        emit plot(vres);
        
    }
    vid = false;
    cvReleaseCapture(&capture);
    emit plot(res, pres);
}

/*!
 * \brief Processor::calibrate
 * \param name
 * \param named
 * \param names
 */
void Processor::calibrate(const QString &name, const QString &named, const QStringList &names)
{
    loadConf(name);
    images.clear();
    images.resize(1);
    res.clear();
    pres.clear();
    for (int i = 0; i < 4; ++i) {
        res4[i].clear();
    }
    curr = 0;
    fileNames.append(names[0]);
    for(auto it = names.begin(); it != names.end(); ++it){
        images[0].image = loadImage(*it);
        images[0].l = true;
        images[0].r = false;
        images[0].counter = 3;
        QString tmp = *it;
        int a = tmp.lastIndexOf('.');
        tmp.chop(tmp.size() - a);
        a = tmp.lastIndexOf('/');
        tmp = tmp.right(tmp.size() - a - 1 );
        int n = tmp.toInt();
        autorun();
        for (int i = 0; i < 4; ++i) {
            res4[i].push_back(images[0].bound_counter[i]/1000.);
        }
        res.push_back((images[0].bound_counter[0] + images[0].bound_counter[1] + images[0].bound_counter[2] + images[0].bound_counter[3])/1000.);
        pres.push_back(n);
    }
    saveData(named);
}

/*!
 * \brief Processor::loadData
 * \param name
 */
void Processor::loadData(const QString &name)
{
    QString filename = name;
    QFile file(filename);
    QVector<double> r4;
    r4.resize(4);
    res.clear();
    pres.clear();
    if(file.open(QFile::ReadOnly)){
        QTextStream str(&file);
        while(1){
            double r = 0;
            double p = 0;
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
        emit somethingWentWrong("Error", "Can not open data file");
    }
}

/*!
 * \brief Processor::saveData
 * \param name
 */
void Processor::saveData(const QString &name)
{
    QString filename = name;
    QFile file(filename);
    if(file.open(QFile::WriteOnly)){
        QTextStream str(&file);
        for(int i = 0; i < res.size(); ++i){
            str << res4[0][i] << " " << res4[1][i] << " " << res4[2][i] << " " << res4[3][i] << " ";
            str << res[i] << " " << pres[i] << " \n";
        }
    }else{
        emit somethingWentWrong("Error", "Can not open data file");
    }
}

/*!
 * \brief Processor::stopThis
 */
void Processor::stopThis()
{
    stop = true;
}

/*!
 * \brief Processor::saveConf
 * \param name
 * \param def
 */
void Processor::saveConf(const QString& name, bool def)
{
    Image& image = images[curr];
    static bool fp = true;
    static QString filename = QString("default.conf");
    if(!def){
        if(fp){
            filename = name;
            QFile file(filename);
            if(file.open(QFile::WriteOnly)){
                QTextStream str(&file);
                str << image.crop.left() << " " << image.crop.top() << "\n";
                str << image.crop.right() << " " << image.crop.bottom() << "\n";
                str << image.square[0].x() << " " << image.square[0].y() << "\n";
                str << image.square[1].x() << " " << image.square[1].y() << "\n";
                str << image.square[2].x() << " " << image.square[2].y() << "\n";
                emit somethingWentWrong("Next step","Put 3 points then press Save again");
            }else{
                emit somethingWentWrong("Error","Can not open a file");
            }
            fp = false;
        }else{
            QFile file(filename);
            if(file.open(QFile::Append)){
                QTextStream str(&file);
                str << image.square[0].x() << " " << image.square[0].y() << "\n";
                str << image.square[1].x() << " " << image.square[1].y() << "\n";
                str << image.square[2].x() << " " << image.square[2].y() << "\n";
            }else{
                emit somethingWentWrong("Error","Can not open a file");
            }
            fp = true;
            filename = QString("default.conf");
        }
    }else{
        QFile file(filename);
        if(file.open(QFile::WriteOnly)){
            QTextStream str(&file);
            str << conf.crop.left() << " " << conf.crop.top() << "\n";
            str << conf.crop.right() << " " << conf.crop.bottom() << "\n";
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

/*!
 * \brief Processor::loadConf
 * \param name
 */
void Processor::loadConf(const QString &name)
{
    Image& image = images[curr];
    QString filename = name;
    QFile file(filename);
    if(file.open(QFile::ReadOnly)){
        QTextStream str(&file);
        int r,t,l,b;
        str >> l >> t;
        str >> r >> b;
        conf.crop.setRight(r);
        conf.crop.setLeft(l);
        conf.crop.setTop(t);
        conf.crop.setBottom(b);
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
        repaint();
    }
    image.conf = conf;
    image.counter = 3;
}

/*!
 * \brief Processor::align
 * \param vu_flag
 */
void Processor::align()
{
    Image& image = images[curr];
    QMatrix matrix;
    matrix.shear(4*(image.square[1].x() - image.square[2].x())/(double)image.image.width(), 4*(image.square[0].y() - image.square[1].y())/(double)image.image.height());
    image.image = image.image.transformed(matrix);
    image.counter = 0;
    if(!vid)repaint();
}

/*!
 * \brief Processor::run
 * \param vu_flag
 */
void Processor::run(bool vu_flag)
{
    Image& image = images[curr];
    for(unsigned& x : image.bound_counter){
        x = 0;
    }
    if( image.counter == 3 && !image.image.isNull()){
        unsigned x1 = image.square[1].x()-origin[1].x();
        unsigned x2 = image.image.width()-1;
        unsigned y1 = image.square[1].y()-origin[1].y();
        unsigned y2 = image.square[2].y()-origin[1].y();
        image.bound_counter[0] = searchTheLight(image.image,image.threshold,x1,y1,x2,y2);
        x1 = 0;
        x2 = image.square[0].x()-origin[1].x();
        y1 = image.square[0].y()-origin[1].y();
        y2 = image.square[2].y()-origin[1].y();
        image.bound_counter[1] = searchTheLight(image.image,image.threshold,x1,y1,x2,y2);
        x1 = image.square[0].x()-origin[1].x();
        x2 = image.square[1].x()-origin[1].x();
        y1 = 0;
        y2 = image.square[0].y()-origin[1].y();
        image.bound_counter[2] = searchTheLight(image.image,image.threshold,x1,y1,x2,y2);
        x1 = image.square[0].x()-origin[1].x();
        x2 = image.square[1].x()-origin[1].x();
        y1 = image.square[2].y()-origin[1].y();
        y2 = image.image.height()-1;
        image.bound_counter[3] = searchTheLight(image.image,image.threshold,x1,y1,x2,y2);
    }else{
        emit somethingWentWrong("No points or image","Put 3 points or open image");
    }
    if(!vid && vu_flag)repaint();
}

/*!
 * \brief Processor::reset
 */
void Processor::reset()
{
    Image& image = images[curr];
    image.image = loadImage(fileNames[curr]);
    image.counter = 0;
    origin[0] = {0, 0};
    origin[1] = {0, 0};
    image.r = false;
    repaint();
}

/*!
 * \brief Processor::autorun
 * \param vu_flag
 */
void Processor::autorun(bool vu_flag)
{
    if(!fileNameV.isEmpty()){
        vid = true;
        fileNames.clear();
        images.clear();
        images.resize(frame_num);
        QList<QFuture<double>> sums;
        for(unsigned i = 0 ; i < frame_num; ++i){
            fileNames.push_back((QString("frame_")+QString::number(i)+QString(".bmp")));
            curr = i;
            images[curr].image = loadImage(fileNames[curr]);
            Image& image = images[curr];
            image.crop = conf.crop;
            for(int i = 0; i < 3; ++i){
                image.square[i] = conf.square[i];
            }
            image.image = image.image.copy(image.crop);
            image.counter = 3;
            align();
            image.counter = 3;
            for(int i = 0; i < 3; ++i){
                image.square[i] = conf.square0[i];
            }
            run(true);
            image.sum = calculate(res, pres, std::accumulate(&image.bound_counter[0], image.bound_counter+4,0)/1000.);
            vres.push_back(image.sum);
            vres0.push_back(std::accumulate(&image.bound_counter[0], image.bound_counter+4,0));
        }
        vid = false;
    }else{
        Image& image = images[curr];
        if(!image.r && image.l){
            image.crop = conf.crop;
            for(int i = 0; i < 3; ++i){
                image.square[i] = conf.square[i];
            }
            image.image = image.image.copy(image.crop);
            image.counter = 3;
            repaint();
            align();  
            for(int i = 0; i < 3; ++i){
                image.square[i] = conf.square0[i];
            }
            image.counter = 3;
            run(true);
            image.sum = calculate(res, pres, std::accumulate(&image.bound_counter[0], image.bound_counter+4,0)/1000.);
            image.r = true;
            if(vu_flag){
                repaint();
            }
        }else{
            repaint();
            emit somethingWentWrong("Fail", "Image already processed or not loaded");
        }
    }
}

/*!
 * \brief Processor::IplImage2QImage
 * \param iplImage
 * \return 
 */
QImage Processor::IplImage2QImage(const IplImage *iplImage)
{
    int height = iplImage->height;
    int width = iplImage->width;
    
    const uchar *qImageBuffer =(const uchar*)iplImage->imageData;
    QImage img(qImageBuffer, width, height, QImage::Format_RGB888);
    return img.rgbSwapped();
}
