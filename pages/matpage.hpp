#ifndef MATPAGE_HPP
#define MATPAGE_HPP

#include <QWidget>
#include <QWizardPage>

namespace Ui {
class MatPage;
}

/*!
 * \brief The MatPage class
 */
class MatPage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit MatPage(QWidget *parent = 0);
    ~MatPage();
    
    int nextId() const;
private:
    Ui::MatPage *ui;
};

#endif // MATPAGE_HPP
