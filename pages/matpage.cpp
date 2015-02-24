#include "matpage.hpp"
#include "ui_matpage.h"
#include "../modelingwizard.hpp"

/*!
 * \brief MatPage::MatPage
 * \param parent
 */
MatPage::MatPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MatPage)
{
    ui->setupUi(this);
    setTitle("Set material properties");
}

/*!
 * \brief MatPage::~MatPage
 */
MatPage::~MatPage()
{
    delete ui;
}

/*!
 * \brief MatPage::nextId
 * \return 
 */
int MatPage::nextId() const
{
    ModelingWizard * mw = dynamic_cast<ModelingWizard*>(wizard());
    mw->getDataRef().setYm(ui->Ym->value()*1e9);
    mw->getDataRef().setPr(ui->Pr->value());
    return ModelingWizard::MISC_PAGE;
}
