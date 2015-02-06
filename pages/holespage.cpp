#include "holespage.hpp"
#include "ui_holespage.h"
#include "modelingwizard.hpp"

HolesPage::HolesPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::HolesPage)
{
    ui->setupUi(this);
    setTitle("Set holes parameters");
}

HolesPage::~HolesPage()
{
    delete ui;
}

int HolesPage::nextId() const
{
    return ModelingWizard::CAM_PAGE;
}
