#include "miscpage.hpp"
#include "ui_miscpage.h"
#include "modelingwizard.hpp"

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
    return ModelingWizard::HOLES_PAGE;
}
