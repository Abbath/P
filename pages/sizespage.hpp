#ifndef SIZESPAGE_HPP
#define SIZESPAGE_HPP

#include <QWidget>
#include <QWizardPage>

namespace Ui {
class SizesPage;
}

/*!
 * \brief The SizesPage class
 */
class SizesPage : public QWizardPage
{
    Q_OBJECT
    
public:
    explicit SizesPage(QWidget *parent = 0);
    ~SizesPage();
    int nextId() const;
private:
    Ui::SizesPage *ui;
};

#endif // SIZESPAGE_HPP
