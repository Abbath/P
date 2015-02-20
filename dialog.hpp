#ifndef DIALOG_HPP
#define DIALOG_HPP

#include <QDialog>
#include "mainwindow.hpp"
#include "modelingwindow.hpp"
#include "modelingwizard.hpp"
namespace Ui {
class Dialog;
}

/*!
 * \brief The Dialog class
 */
class Dialog : public QWidget
{
    Q_OBJECT
public:
    enum Mode {MODELING, CALIBRATION, MEASUREMENT};
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
   
    Mode getMod() const;
    void setMod(const Mode &value);
    void closeEvent(QCloseEvent *);
    bool getAsk_for_data() const;
    void setAsk_for_data(bool value);
    
private slots:
    void on_pushButton_clicked();
    
    void on_pushButton_4_clicked();
    
    void on_pushButton_2_clicked();
    
    void on_checkBox_toggled(bool checked);
    
    void on_pushButton_3_clicked();
    
public slots:
    void trayHandle(QSystemTrayIcon::ActivationReason reason);
private:
    Ui::Dialog *ui;
    Mode mod = MEASUREMENT;
    bool ask_for_data = false;
    ModelingWindow* w;
    MainWindow * mainw;
};

#endif // DIALOG_HPP
