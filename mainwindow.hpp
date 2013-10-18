#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>
#include <helpers.hpp>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *e);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void on_actionReset_triggered();
    void on_actionRun_triggered();
    void on_actionAlign_triggered();
    void on_action3D_triggered();
    void on_actionSave_triggered();
    void on_actionLoad_triggered();
    void on_actionCalibrate_triggered();
    void on_actionAutorun_triggered();

    void on_actionOpen_Video_triggered();

    void on_horizontalSlider_valueChanged(int value);

    void on_actionExit_triggered();

    void on_actionSave_as_Default_triggered();

    void on_actionSave_2_triggered();

    void on_actionLoad_2_triggered();

    void on_actionAbout_triggered();



    void on_actionBounds_triggered(bool checked);

private:
    Ui::MainWindow *ui;
    };

#endif // MAINWINDOW_HPP
