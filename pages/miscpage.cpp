#include "miscpage.hpp"
#include "ui_miscpage.h"
#include "../modelingwizard.hpp"

MiscPage::MiscPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MiscPage)
{
    ui->setupUi(this);
    setTitle("Set pressure, ray number and light wavelength");
}

MiscPage::~MiscPage()
{
    delete ui;
}

int MiscPage::nextId() const
{
    ModelingWizard * mw = dynamic_cast<ModelingWizard*>(wizard());
    mw->getDataRef().setRay_number(ui->rays_number->value());
    mw->getDataRef().setWavelength(ui->wavelength->value());
    mw->getDataRef().setPressure(ui->pressure->value()*1000/25);
    return ModelingWizard::HOLES_PAGE;
}
