#include "modelingcore.hpp"

ModelingCore::ModelingCore(QObject *parent) :
    QObject(parent)
{
    stop.store(true);
    clean_space_f = (die_size - mem_size) / 2;
    clean_space_b = (die_size - mem_size) / 2 + mem_size;
}

void ModelingCore::run()
{
    stop.store(false);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_real_distribution<double> distribution((die_size - mem_size)/2.0, (die_size - mem_size)/2.0+mem_size);
    QPixmap pix(die_size*scale, die_size*scale), pix0(die_size*scale, die_size*scale);
    QPainter painter(&pix);
    QPainter painter0(&pix0);
    painter.setBrush(Qt::SolidPattern);
    painter0.setPen(QColor(0, 255, 0));
    painter.setPen(QColor(0, 255, 0));
    painter.drawRect((die_size-mem_size)/2*scale, (die_size-mem_size)/2*scale, mem_size*scale, mem_size*scale);
    QColor color = waveLengthToRGB(wavelength);
    painter.setPen(color);
    if(is_data_external){
        std::string s = filename.toStdString();
        data = readFile(s);
    }
    int hole_n = int(mem_size*scale) / hole_space_size;
    
    some_strange_size = int(mem_size*scale) % hole_space_size;
    
    if(some_strange_size == 0){
        some_strange_size = space_size/2;
    }else if(some_strange_size > hole_size){
        some_strange_size = (int(mem_size*scale) - (hole_space_size * hole_n + hole_size)) / 2;
    }else if(some_strange_size < hole_size){
        some_strange_size = some_strange_size / 2;
    }
    int counter = 0;    
    //#pragma omp parallel for shared(counter)
    for(int i = 0; i < ray_number;  ++ i){ 
        if(!stop.load()){
            Point p;
            p.x = distribution(generator);
            p.y = distribution(generator);
            p.z = 0;
            auto res = test(p);
            int x2, y2;
            //            int x1 = static_cast<int>(p.x*scale);
            //            int y1 = static_cast<int>(p.y*scale);
            
            x2 = (p.x - std::get<0>(res)) * scale;
            y2 = (p.y - std::get<1>(res)) * scale;
            
            if(x2 > 0 && y2 > 0 && x2 < die_size*scale && y2 < die_size*scale){
                //                if(!((x2 > (die_size-mem_size)/2*scale && x2 < ((die_size-mem_size)/2+mem_size)*scale) && (y2 > (die_size-mem_size)/2*scale && y2 < ((die_size-mem_size)/2+mem_size)*scale))){
                if(check_holes(x2, y2)){
                    painter.fillRect(x2, y2, 30, 30, color);
                }
                //}
            }
        }
        counter ++ ;
        
        if(counter >= 100 && !(counter%(ray_number/100))){
            emit lil(counter/(ray_number/100));
        }
    }
    painter.setPen(Qt::black);
    painter.fillRect(int((die_size-mem_size)/2*scale),int((die_size-mem_size)/2*scale), int(mem_size*scale), int(mem_size*scale), Qt::SolidPattern );
    int new_size = pix.height()*423/15000;
    im = pix.scaled(new_size, new_size).toImage();
    im = rotate(im);
    im.save(QString::number((int)pressure*25/1000) + ".png");
    emit lil(100);
    emit sendImage(im);
}

std::vector<Point> ModelingCore::readFile(std::string filename){
    std::ifstream in(filename.c_str());
    std::vector<Point> res;
    double z;
    Point tmp;
    double tc, tx, ty;
    while(in >> tmp.x >> tmp.y >> z >> tc >> tx >> ty >> tmp.z){
        if(std::fabs(z - 0.4e-3) <= std::numeric_limits<double>::epsilon()){
            //tmp.x +=5e-3;
            //tmp.y +=5e-3;
            // tmp.z - =0.4e-3;
            res.push_back(tmp);
        }
    }
    return res;
}

double ModelingCore::dist(const Point& a, const Point& b){
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double ModelingCore::square(const Point &a, const Point &b, const Point &c)
{
    return 0.5*((a.x - c.x)*(b.y - c.y) - (b.x - c.x)*(a.y - c.y));
}

std::tuple<Point, Point, Point> ModelingCore::closest_external(const Point& p, const std::vector<Point>& v){
    std::vector<double> dists;
    dists.reserve(v.size());
    for(auto& x : v){
        dists.push_back(dist(p, x));
    }
    auto a_it = std::min_element(dists.begin(), dists.end());
    Point a = v[a_it - dists.begin()];
    dists.erase(a_it);
    
    auto b_it = std::min_element(dists.begin(), dists.end());
    Point b = v[b_it - dists.begin()];
    dists.erase(b_it);
    
    auto c_it = std::min_element(dists.begin(), dists.end());
    Point c = v[c_it - dists.begin()];
    dists.erase(c_it);
    
    if(!check_triangle(p, std::make_tuple(a, b, c))){
        std::cout << "He's dead, Jim." << std::endl;
    }
    return std::make_tuple(a, b, c);
}

std::tuple<Point, Point, Point> ModelingCore::closest_internal(const Point &p)
{
    Point p1, p2, p3;
    //auto place = die_size/2;
    //    if(p.x > place && p.y > place){
    p1.x = p.x - 1e-5;
    p1.y = p.y - 1e-5;
    
    p2.x = p.x + 1e-5;
    p2.y = p.y;    
    
    p3.x = p.x;
    p3.y = p.y + 1e-5;
    //    }
    //    if(p.x < place && p.y < place){
    //        p1.x = p.x + 1e-5;
    //        p1.y = p.y + 1e-5;
    
    //        p2.x = p.x - 1e-5;
    //        p2.y = p.y;    
    
    //        p3.x = p.x;
    //        p3.y = p.y - 1e-5;
    //    }
    //    if(p.x > place && p.y < place){
    //        p1.x = p.x + 1e-5;
    //        p1.y = p.y - 1e-5;
    
    //        p2.x = p.x - 1e-5;
    //        p2.y = p.y;    
    
    //        p3.x = p.x;
    //        p3.y = p.y + 1e-5;
    //    }
    //    if(p.x < place && p.y > place){
    //        p1.x = p.x - 1e-5;
    //        p1.y = p.y + 1e-5;
    
    //        p2.x = p.x + 1e-5;
    //        p2.y = p.y;    
    
    //        p3.x = p.x;
    //        p3.y = p.y - 1e-5;
    //    }
    auto displacement = (die_size - mem_size)/2.0;
    p1.z = w(p1.x - displacement, p1.y - displacement);
    p2.z = w(p2.x - displacement, p2.y - displacement);
    p3.z = w(p3.x - displacement, p3.y - displacement);
    
    return std::make_tuple(p1, p2, p3);
    
}

Vector ModelingCore::normal(std::tuple<Point, Point, Point> points){
    Point a = std::get<0>(points);
    Point b = std::get<1>(points);
    Point c = std::get<2>(points);
    auto A = a.y * (b.z - c.z) + b.y * (c.z - a.z) + c.y * (a.z - b.z);
    auto B = a.z * (b.x - c.x) + b.z * (c.x - a.x) + c.z * (a.x - b.x);
    auto C = a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
    auto len = sqrt(A*A + B*B + C*C);
    Vector res{A, B, C};
    res.x = A / len;
    res.y = B / len;
    res.z = C / len;
    if(res.z < 0){
        res.x =  - res.x;
        res.y =  - res.y;
        res.z =  - res.z;
    }
    return res;
}

double ModelingCore::angle(const Vector& a, const Vector& b){
    auto nom = a.x*b.x + a.y*b.y + a.z*b.z;
    auto den = sqrt(a.x*a.x + a.y*a.y + a.z*a.z) * sqrt(b.x*b.x + b.y*b.y + b.z*b.z);
    return acos(nom / den);
}

Vector ModelingCore::reflect(const Vector& v, const Vector& n){
    Vector res;
    auto dot = v.x*n.x + v.y*n.y + v.z*n.z;
    res.x = 2*dot*n.x - v.x;
    res.y = 2*dot*n.y - v.y;
    res.z = 2*dot*n.z - v.z;
    return res;
}

double ModelingCore::z(double x, double y, std::tuple<Point, Point , Point> closest){
    Point a = std::get<0>(closest);
    Point b = std::get<1>(closest);
    Point c = std::get<2>(closest);
    auto nom =  - ((x - a.x) * ( (b.y - a.y) * (c.z - a.z) - (b.z - a.z) * (c.y - a.y) ) - (y - a.y) * ((b.x - a.x) * (c.z - a.z) - (c.x - a.x) * (b.z - a.z)));
    auto den = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
    return (nom / den) + a.z;
}

void ModelingCore::calculateStress(std::tuple<Point, Point, Point> three)
{
    auto p1 = std::get<0>(three);
    auto p2 = std::get<1>(three);
    auto p3 = std::get<2>(three);
    
    auto displacement = (die_size - mem_size)/2.0;
    
    std::pair<double, double> st1 = stress(p1.x - displacement, p1.y - displacement, p1.z);
    std::pair<double, double> st2 = stress(p2.x - displacement, p2.y - displacement, p2.z);
    std::pair<double, double> st3 = stress(p3.x - displacement, p3.y - displacement, p3.z);
    
    if (st1.first > 7000*scale || st1.second > 7000*scale || st2.first > 7000*scale || st2.second > 7000*scale || st3.first > 7000*scale || st3.second > 7000*scale){
        emit error("Membrane is broken");
        stop.store(true);
    }    
}

std::tuple<double, double> ModelingCore::test(const Point& p/*, const std::vector<Point>& data */){
    std::tuple<Point, Point, Point> three;
    if(is_data_external){
        three = closest_external(p, data);
    }else{
        three = closest_internal(p);
        //calculateStress(three);
    }
    auto norm = normal(three);
    
    double A = x_angle;
    double B = y_angle;
    double C =  - 1.0;
    double l = sqrt(A*A + B*B + C*C);
    
    double X = A / l;
    double Y = B / l;
    double Z = C / l;
    
    auto r = reflect({X, Y, Z}, norm);
    auto ang = angle(r, {r.x, r.y, 0});
    auto len = (spacer_height - w(p.x, p.y)) / tan(ang);
    auto ratio = len/sqrt(r.x*r.x + r.y*r.y);
    auto x = r.x * ratio;
    auto y = r.y * ratio;
    return std::make_tuple(x, y);
}

bool ModelingCore::check_holes(int x, int y){
    int hole_space_size = hole_size + space_size;
    
    if((x > (die_size-mem_size)/2*scale && x < ((die_size-mem_size)/2+mem_size)*scale) && (y > (die_size-mem_size)/2*scale && y < ((die_size-mem_size)/2+mem_size)*scale)){
        return false;
    }
    if((x > ((die_size-mem_size)/2+mem_size)*scale) && (y > (die_size-mem_size)/2*scale && y < ((die_size-mem_size)/2+mem_size)*scale)){
        if(((int)(x - ((die_size-mem_size)/2+mem_size)*scale) % hole_space_size) > space_size && ((int)(y - (die_size-mem_size)/2*scale + some_strange_size) % hole_space_size) < hole_size){
            if((x - ((die_size-mem_size)/2+mem_size)*scale) < hole_space_size*holes_rows_number){
                return true;
            }
        }
    }
    if((x < (die_size-mem_size)/2*scale) && (y > (die_size-mem_size)/2*scale && y < ((die_size-mem_size)/2+mem_size)*scale)){
        if(((int)((die_size-mem_size)/2*scale-x) % hole_space_size) > space_size && ((int)(y - (die_size-mem_size)/2*scale + some_strange_size) % hole_space_size) < hole_size){
            if(((die_size-mem_size)/2*scale-x) < hole_space_size*holes_rows_number){
                return true;
            }
        }
    }
    if((y > ((die_size-mem_size)/2+mem_size)*scale) && (x > (die_size-mem_size)/2*scale && x < ((die_size-mem_size)/2+mem_size)*scale)){
        if(((int)(y - ((die_size-mem_size)/2+mem_size)*scale) % hole_space_size) > space_size && ((int)(x - (die_size-mem_size)/2*scale + some_strange_size) % hole_space_size) < hole_size){
            if((y - ((die_size-mem_size)/2+mem_size)*scale) < hole_space_size*holes_rows_number){
                return true;
            }   
        }
    }
    if((y < (die_size-mem_size)/2*scale) && (x > (die_size-mem_size)/2*scale && x < ((die_size-mem_size)/2+mem_size)*scale)){
        if(((int)((die_size-mem_size)/2*scale-y) % hole_space_size) > space_size && ((int)(x - (die_size-mem_size)/2*scale + some_strange_size) % hole_space_size) < hole_size){
            if(((die_size-mem_size)/2*scale-y) < hole_space_size*holes_rows_number){
                return true;
            }
        }
    }
    return false;
}

bool ModelingCore::check_triangle(const Point &p, const std::tuple<Point, Point, Point> &closest)
{
    Point a = std::get<0>(closest);
    Point b = std::get<1>(closest);
    Point c = std::get<2>(closest);
    auto sabc = square(a, b, c);
    auto spbc = square(p, b, c);
    auto sapc = square(a, p, c);
    auto sabp = square(a, b, p);
    return fabs(sabc - spbc - sapc - sabp) <= std::numeric_limits<double>::epsilon();
}

Grid ModelingCore::build_grid()
{
    Grid res;
    double top = hole_space_size;
    double left = 0, right = hole_size;
    Point a, b, c, d;
    for(int i = 0; i < 6;  ++ i){
        for(int j = 0; j < 13;  ++ j){
            a = {left/scale, top/scale, 2.1e-3};
            b = {right/scale, top/scale, 2.1e-3};
            c = {(left - 3.25)/scale, (top + 3.25)/scale, 2e-3};
            d = {(right + 3.25)/scale, (top + 3.25)/scale, 2e-3};  
            auto t = std::make_tuple(a, b, c, d);
            res.push_back(t);
            left  += hole_space_size;
            right  += hole_space_size;
        }
        top  += hole_space_size;
        left = 0;
        right = hole_size;
    }
    
    top = (int)( - mem_size*scale) + hole_size;
    left = (int)(mem_size*scale) + space_size;
    right = (int)(mem_size*scale) + 400;
    for(int i = 0; i < 13;  ++ i){
        for(int j = 0; j < 6;  ++ j){
            a = {left/scale, top/scale, 2.1e-3};
            b = {right/scale, top/scale, 2.1e-3};
            c = {(left - 3.25)/scale, (top + 3.25)/scale, 2e-3};
            d = {(right + 3.25)/scale, (top + 3.25)/scale, 2e-3};  
            auto t = std::make_tuple(a, b, c, d);
            res.push_back(t);
            left  += hole_space_size;
            right  += hole_space_size;
        }
        top  += hole_space_size;
        left = (int)(mem_size*scale) + space_size;
        right = (int)(mem_size*scale) + hole_space_size;
    }
    
    top = (int)( - (mem_size + 2.4e-3)*scale) + hole_size;
    left = 0, right = hole_size;
    for(int i = 0; i < 6;  ++ i){
        for(int j = 0; j < 13;  ++ j){
            a = {left/scale, top/scale, 2.1e-3};
            b = {right/scale, top/scale, 2.1e-3};
            c = {(left - 3.25)/scale, (top + 3.25)/scale, 2e-3};
            d = {(right + 3.25)/scale, (top + 3.25)/scale, 2e-3};  
            auto t = std::make_tuple(a, b, c, d);
            res.push_back(t);
            left  += hole_space_size;
            right  += hole_space_size;
        }
        top  += hole_space_size;
        left = 0;
        right = hole_size;
    }
    
    top = (int)( - mem_size*scale) + hole_size;
    left =  - hole_space_size*holes_rows_number;
    right =  - hole_space_size*holes_rows_number + hole_size;
    for(int i = 0; i < 13;  ++ i){
        for(int j = 0; j < 6;  ++ j){
            a = {left/scale, top/scale, 2.1e-3};
            b = {right/scale, top/scale, 2.1e-3};
            c = {(left - 3.25)/scale, (top + 3.25)/scale, 2e-3};
            d = {(right + 3.25)/scale, (top + 3.25)/scale, 2e-3};  
            auto t = std::make_tuple(a, b, c, d);
            res.push_back(t);
            left  += hole_space_size;
            right  += hole_space_size;
        }
        top  += hole_space_size;
        left =  - hole_space_size*holes_rows_number;
        right =  - hole_space_size*holes_rows_number + hole_size;
    }
    
    return res;
}

QImage ModelingCore::rotate(const QImage &im)
{
    QImage res(im.width(), im.height(), im.format());
    res.fill(Qt::black);
    for(int i = 0; i < im.width();  ++ i){
        for(int j = 0; j < im.height();  ++ j){
            double alpha = cam_angle_y;
            Point p{double(i), double(j), 0};
            int h = im.height() / 2;
            p.y = ((p.y-h) *cos(alpha) - p.z*sin(alpha)) + h;
            if(p.y < im.height() && p.y >= 0){
                res.setPixel(p.x, p.y, im.pixel(i, j));
            }
        }
    }
    return res;
}

std::pair<bool, Vector> ModelingCore::check_grid(const Point& origin, const Vector& v/*, int x*/, int y, int x0, int y0)
{ 
    double yy = y/scale;
    double xx0 = x0/scale;
    double yy0 = y0/scale;
    Grid grid = build_grid();
    for(auto x : grid){
        Point a = std::get<0>(x) + origin;
        Point b = std::get<1>(x) + origin;
        Point c = std::get<2>(x) + origin;
        if(xx0 > a.x && xx0 < b.x){
            if(yy < c.y && yy0 > a.y){
                auto norm = normal(std::make_tuple(a, b, c));
                if(norm.z > 0){
                    norm.x = - norm.x;
                    norm.y = - norm.y;
                    norm.z = - norm.z;
                }
                auto r = reflect(v, norm);
                auto ang = angle(r, {r.x, r.y, 0}); 
                double range = M_PI/3.0*0.25;
                if(ang < (M_PI/3.0 + range) && ang > (M_PI/3.0 - range)){
                    std::cout << "Yep, it's wood." << std::endl;
                }
                std::cout << ang << std::endl;
                return std::make_pair(true, r);
            }
        }
    }
    return std::make_pair(false, Vector{0, 0, 0});
}

QColor ModelingCore::waveLengthToRGB(double wavelength)
{
    double gamma = 0.8;
    double factor;
    double R, G, B;
    
    if(wavelength >= 380 && wavelength < 440){
        R =  - (wavelength - 440) / (440 - 380);
        G = 0.0;
        B = 1.0;
    }else if(wavelength >= 440 && wavelength < 490){
        R = 0.0;
        G = (wavelength - 440) / (490 - 440);
        B = 1.0;
    }else if(wavelength >= 490 && wavelength < 510){
        R = 0.0;
        G = 1.0;
        B = - (wavelength - 510) / (510 - 490);
    }else if(wavelength >= 510 && wavelength < 580){
        R = (wavelength - 510) / (580 - 510);
        G = 1.0;
        B = 0.0;
    }else if(wavelength >= 580 && wavelength < 645){
        R = 1.0;
        G = - (wavelength - 645) / (645 - 580);
        B = 0.0;
    }else if(wavelength >= 645 && wavelength < 781){
        R = 1.0;
        G = 0.0;
        B = 0.0;
    }else{
        R = G = B = 0.0;
    }
    
    if(wavelength >= 380 && wavelength < 420){
        factor = 0.3 + 0.7 * (wavelength - 380) / (420 - 380);
    }else if(wavelength >= 420 && wavelength < 701){
        factor = 1.0;
    }else if(wavelength >= 701 && wavelength < 781){
        factor = 0.3 + 0.7 * (780 - wavelength) / (780 - 700);
    }else{
        factor = 0.0;
    }
    
    R = R == 0 ? 0.0 : round(255 * pow(R * factor, gamma));
    G = G == 0 ? 0.0 : round(255 * pow(G * factor, gamma));
    B = B == 0 ? 0.0 : round(255 * pow(B * factor, gamma));
    
    return QColor(R, G, B);
}

void ModelingCore::Stop()
{
    stop.store(!stop.load());
}
int ModelingCore::getSpace_size() const
{
    return space_size;
}

void ModelingCore::setSpace_size(int value)
{
    space_size = value;
}

void ModelingCore::setHole_size(int value)
{
    hole_size = value;
}

void ModelingCore::setData(const ModelingData &data)
{
    pressure = data.getPressure();
    is_data_external = data.getIs_ext();
    die_size = data.getDie_size();
    mem_size = data.getMem_size();
    thickness = data.getMem_thickness();
    holes_rows_number = data.getRows();
    hole_size = data.getHole_size();
    hole_space_size = data.getSpace_size();
    spacer_height = data.getSpacer_height();
    ray_number = data.getRay_number();
    filename = data.getFilename();
    ym = data.getYm();
    pr = data.getPr();
    wavelength = data.getWavelength();
    cam_angle_x = data.getCamx();
    cam_angle_y = data.getCamy();
    x_angle = data.getX_angle();
    y_angle = data.getY_angle();
}

int ModelingCore::getHoles_rows_number() const
{
    return holes_rows_number;
}

void ModelingCore::setHoles_rows_number(int value)
{
    holes_rows_number = value;
}

double ModelingCore::getPr() const
{
    return pr;
}

void ModelingCore::setPr(double value)
{
    pr = value;
}

double ModelingCore::getYm() const
{
    return ym;
}

void ModelingCore::setYm(double value)
{
    ym = value;
}

int ModelingCore::getN() const
{
    return N;
}

void ModelingCore::setN(int value)
{
    N = value;
}

double ModelingCore::getCam_angle_y() const
{
    return cam_angle_y;
}

void ModelingCore::setCam_angle_y(double value)
{
    cam_angle_y = value;
}

double ModelingCore::getCam_angle_x() const
{
    return cam_angle_x;
}

void ModelingCore::setCam_angle_x(double value)
{
    cam_angle_x = value;
}

bool ModelingCore::getIs_data_external() const
{
    return is_data_external;
}

void ModelingCore::setIs_data_external(bool value)
{
    is_data_external = value;
}


double ModelingCore::w(double x, double y)
{
    auto q0 = pressure;
    auto a = mem_size;
    auto b = mem_size;
    auto h = thickness;
    auto E = ym;
    auto mu = pr;
    auto pi = 3.14159265;
    auto D = pow(h, 3.0)*E/(12*(1 - pow(mu, 2.0)));
    auto res = 0.0;
    for(int m = 1; m < N;  ++ m){
        for(int n = 1; n < N;  ++ n){
            auto m1 = 16*q0/((2*m - 1)*(2*n - 1)*pow(pi, 6)*D);
            auto m2 = pow((pow((2*m - 1), 2)/pow(a, 2) + pow((2*n - 1), 2)/pow(b, 2)), - 2);
            auto m3 = sin((2*m - 1)*pi*x/a);
            auto m4 = sin((2*n - 1)*pi*y/b);
            res  += m1*m2*m3*m4;
        }
    }
    //auto dist = sqrt(pow(x*1000-2.5,2.0) + pow(y*1000-2.5, 2.0));
    //res /= exp(2*dist)-1;
    //std::cout << x << " " << y << " " << res << '\n';
    //res /= 10;
    return res + (thickness/2);
    
    
}

double ModelingCore::w2(double x, double y){
    auto p = pressure;
    auto u = mem_size/2;
    auto v = mem_size/2;
    auto b = mem_size/2;
    auto d = mem_size/2;
    auto l_x = mem_size;
    auto l_y = mem_size;
    auto pi = 3.1459265359;
    auto t = thickness;
    auto E = ym;
    auto mu = pr;
    auto k = pow(t, 3.0)*E/(12*(1-pow(mu, 2.0)));
    double w = 0.0;
    for(int i = 1; i <= N; ++i){
        for(int j = 1; j <= N; ++j){
            auto a_ij = 16*p/(pi*pi*i*j)*sin(i*pi*u/l_x)*sin(i*pi*b/l_x)*sin(j*pi*v/l_y)*sin(j*pi*d/l_y);
            auto w_ij = a_ij/(k*pow(pi, 4.0)*pow((pow(i/l_x, 2.0)+pow(j/l_y, 2.0)), 2.0));
            w += w_ij*sin(i*pi*x/l_x)*sin(j*pi*y/l_y);
        }
    }
    std::cerr << w << '\n';
    return w + (thickness/2);
}

std::pair<double, double> ModelingCore::stress(double x, double y, double z){
    auto q0 = pressure;
    auto a = mem_size;
    auto b = mem_size;
    auto h = thickness;
    auto mu = pr;
    auto pi = 3.14159265;
    auto res1 = 0.0, res2 = 0.0;
    for(int m = 1; m < N;  ++ m){
        for(int n = 1; n < N;  ++ n){
            auto m1 = 16*q0/((2*m - 1)*(2*n - 1)*pow(pi, 4.0));
            auto m2 = pow((2*m - 1), 2.0)/pow(a, 2.0) + mu*pow((2*n - 1), 2.0)/pow(b, 2.0);
            auto m3 = pow(pow((2*m - 1), 2.0)/pow(a, 2.0) + pow((2*n - 1), 2.0)/pow(b, 2.0),  - 2.0);
            auto m4 = sin((2*m - 1)*pi*x/a);
            auto m5 = sin((2*n - 1)*pi*y/b);
            res1  += m1*m2*m3*m4*m5; 
        }
    }
    
    for(int m = 1; m < N;  ++ m){
        for(int n = 1; n < N;  ++ n){
            auto m1 = 16*q0/((2*m - 1)*(2*n - 1)*pow(pi, 4.0));
            auto m2 = pow((2*n - 1), 2.0)/pow(b, 2.0) + mu*pow((2*m - 1), 2.0)/pow(a, 2.0);
            auto m3 = pow(pow((2*m - 1), 2.0)/pow(a, 2.0) + pow((2*n - 1), 2.0)/pow(b, 2.0),  - 2.0);
            auto m4 = sin((2*m - 1)*pi*x/a);
            auto m5 = sin((2*n - 1)*pi*y/b);
            res2  += m1*m2*m3*m4*m5; 
        }
    }   
    res1 *= 12*z/pow(h, 3.0);
    res2 *= 12*z/pow(h, 3.0);    
    return std::make_pair(res1, res2);
}

double ModelingCore::getY_angle() const
{
    return y_angle;
}

void ModelingCore::setY_angle(double value)
{
    y_angle = value;
}

double ModelingCore::getX_angle() const
{
    return x_angle;
}

void ModelingCore::setX_angle(double value)
{
    x_angle = value;
}

double ModelingCore::getWavelength() const
{
    return wavelength;
}

void ModelingCore::setWavelength(double value)
{
    wavelength = value;
}

QImage ModelingCore::getIm() const
{
    return im;
}

void ModelingCore::setIm(const QImage &value)
{
    im = value;
}

double ModelingCore::getThickness() const
{
    return thickness;
}

void ModelingCore::setThickness(double value)
{
    thickness = value;
}

double ModelingCore::getSpacer_height() const
{
    return spacer_height;
}

void ModelingCore::setSpacer_height(double value)
{
    spacer_height = value;
}

double ModelingCore::getPressure() const
{
    return pressure;
}

void ModelingCore::setPressure(double value)
{
    pressure = value;
}


double ModelingCore::getMargin() const
{
    return margin;
}

void ModelingCore::setMargin(double value)
{
    margin = value;
}


double ModelingCore::getMem_size() const
{
    return mem_size;
}

void ModelingCore::setMem_size(double value)
{
    mem_size = value;
}

double ModelingCore::getDie_size() const
{
    return die_size;
}

void ModelingCore::setDie_size(double value)
{
    die_size = value;
}

int ModelingCore::getRay_number() const
{
    return ray_number;
}

void ModelingCore::setRay_number(int value)
{
    ray_number = value;
}

