#ifndef MISCPAGE_HPP
#define MISCPAGE_HPP

#include <QWidget>
#include <QWizardPage>

namespace Ui {
class MiscPage;
}

class MiscPage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit MiscPage(QWidget *parent = 0);
    ~MiscPage();
    
    int nextId() const;
private:
    Ui::MiscPage *ui;
};

#endif // MISCPAGE_HPP
