#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QtMultimedia/QMediaPlayer>
#include <QVideoWidget>
#include <helpers.hpp>
#include <processor.hpp>
#include <qwt_plot.h>
#include <qwt.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_zoomer.h>

namespace Ui {
class MainWindow;
}

/*!
 * \brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void setProcessor(Processor * _p) { p = _p;}
    ~MainWindow();
private:
    QGraphicsScene *scene = nullptr;
    QwtPlotZoomer *zoom = nullptr;
    QwtPlotCurve *curve = nullptr;
    QwtPlotZoomer *zoom0 = nullptr;
    QwtPlotCurve *curve0 = nullptr;
    QVideoWidget * w = nullptr;
    QMediaPlayer * player = nullptr;
    Ui::MainWindow *ui = nullptr;
    Processor *p = nullptr;
    void disableUi(bool b = true);
signals:
    void stop();
public slots:
    void Update(Display dis);
    void imageAreaUpdated(Display dis);
    void Error(QString a, QString b);
    void plot(QVector<double> res);
    void plot(QVector<double> res0, QVector<double> res);
private slots:
    void on_actionOpen_Image_s_triggered();
    void on_actionAlign_triggered();
    void on_actionRun_triggered();
    void on_actionReset_triggered();
    void on_actionAutorun_triggered();
    void on_actionPrev_triggered();
    void on_actionNext_triggered();
    void on_actionExit_triggered();
    void on_actionLoad_triggered();
    void on_actionSave_triggered();
    void on_actionLoad_2_triggered();
    void on_actionSave_2_triggered();
    void on_actionSave_as_Default_triggered();
    void on_action3D_triggered(bool checked);
    void on_actionCalibrate_triggered();
    void on_actionOpen_Video_triggered();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_clicked(bool checked);
    void on_horizontalSlider_valueChanged(int value);
    void on_actionStop_triggered();
    void on_actionAbout_triggered();
    void on_actionHelp_triggered();
};

#endif // MAINWINDOW_HPP
