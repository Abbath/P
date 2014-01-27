#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
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
    ~MainWindow();
private:
    Ui::MainWindow *ui;
public slots:
    void Update(Display dis);
    void Error(QString a, QString b);
signals:
    void openImage(QStringList v);
    void align();
    void reset();
    void autorun();
    void run();
    void prev();
    void next();
    void loadConf(QString name);
    void saveConf(QString name, bool def);
    void loadData(QString name);
    void saveData(QString name);
    void hesDeadJim();
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
};

#endif // MAINWINDOW_HPP
