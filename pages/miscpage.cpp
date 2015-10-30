#include "miscpage.hpp"
#include "ui_miscpage.h"
#include "../modelingwizard.hpp"

/*!
 * \brief MiscPage::MiscPage
 * \param parent
 */
MiscPage::MiscPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MiscPage)
{
    ui->setupUi(this);
    setTitle("Set pressure, ray number and light wavelength");
}

/*!
 * \brief MiscPage::~MiscPage
 */
MiscPage::~MiscPage()
{
    delete ui;
}

/*!
 * \brief MiscPage::nextId
 * \return 
 */
int MiscPage::nextId() const
{
    ModelingWizard * mw = dynamic_cast<ModelingWizard*>(wizard());
    mw->getDataRef().setRay_number(ui->rays_number->value());
    mw->getDataRef().setWavelength(ui->wavelength->value());
    mw->getDataRef().setPressure(ui->pressure->value()*1000/25);
    return ModelingWizard::HOLES_PAGE;
}
