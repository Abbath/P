#ifndef MODELINGWINDOW_HPP
#define MODELINGWINDOW_HPP

#include <QMainWindow>
#include <QLabel>
#include "modelingcore.hpp"
#include "modelingdisplay.hpp"
#include "helpers.hpp"

namespace Ui {
class ModelingWindow;
}

class ModelingWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ModelingWindow(QWidget *parent = 0);
    ~ModelingWindow();
    void setCore(ModelingCore *_p){p = _p;}
    void setData(ModelingData data);
public slots:
    void setImage(QImage image);
private slots:
    void on_pushButton_3_clicked();
    void lil(int i);
    void saved();
    void on_actionAbout_triggered();
    void error(QString s);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_actionStart_triggered();
    void on_actionLoad_data_triggered();
    void on_actionLoad_config_triggered();
    void on_actionSave_config_triggered();
    void on_actionSave_image_triggered();
    
private:
    void saveConfig();
    void loadConfig();
    void writeSettings();
    void readSettings();
    Ui::ModelingWindow *ui;
    ModelingDisplay * ia;
    ModelingCore *p;
    QLabel * label;
};

#endif // MODELINGWINDOW_HPP
