#include "sizespage.hpp"
#include "ui_sizespage.h"
#include "modelingwizard.hpp"

SizesPage::SizesPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::SizesPage)
{
    ui->setupUi(this);
    setTitle("Set sizes of the sensor");
}

SizesPage::~SizesPage()
{
    delete ui;
}

int SizesPage::nextId() const
{
    return ModelingWizard::MATERIAL_PAGE;
}
