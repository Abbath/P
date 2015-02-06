#ifndef HOLESPAGE_HPP
#define HOLESPAGE_HPP

#include <QWidget>
#include <QWizardPage>

namespace Ui {
class HolesPage;
}

class HolesPage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit HolesPage(QWidget *parent = 0);
    ~HolesPage();
    
    int nextId() const;
private:
    Ui::HolesPage *ui;
};

#endif // HOLESPAGE_HPP
