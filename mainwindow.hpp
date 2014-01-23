#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QFileDialog>
#include <helpers.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
public slots:
    void Update(Display dis);
signals:
    void openImage(QStringList v);
private slots:
    void on_actionOpen_Image_s_triggered();
};

#endif // MAINWINDOW_HPP
