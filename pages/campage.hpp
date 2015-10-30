#ifndef CAMPAGE_HPP
#define CAMPAGE_HPP

#include <QWidget>
#include <QWizardPage>

namespace Ui {
class CamPage;
}

/*!
 * \brief The CamPage class
 */
class CamPage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit CamPage(QWidget *parent = 0);
    ~CamPage();
    
    int nextId() const;
private:
    Ui::CamPage *ui;
};

#endif // CAMPAGE_HPP
