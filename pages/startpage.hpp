#ifndef STARTPAGE_HPP
#define STARTPAGE_HPP

#include <QWidget>
#include <QWizardPage>

namespace Ui {
class StartPage;
}

class StartPage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit StartPage(QWidget *parent = 0);
    ~StartPage();
    bool isExt() const;
    int nextId() const;
private slots:
    void on_pushButton_clicked();
    
private:
    Ui::StartPage *ui;
    
};

#endif // STARTPAGE_HPP
