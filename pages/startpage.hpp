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
    bool isExt();
    int nextId() const;
private:
    Ui::StartPage *ui;
    
};

#endif // STARTPAGE_HPP
