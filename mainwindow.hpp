#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsScene>
#include <helpers.hpp>
#include <processor.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void setProcessor(Processor * _p) { p = _p;}
    ~MainWindow();
private:
    QGraphicsScene *scene;
    Ui::MainWindow *ui;
    Processor *p;
public slots:
    void Update(Display dis);
    void imageAreaUpdated(Display dis);
    void Error(QString a, QString b);
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
};

#endif // MAINWINDOW_HPP
