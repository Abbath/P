#include "matpage.hpp"
#include "ui_matpage.h"
#include "modelingwizard.hpp"

MatPage::MatPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MatPage)
{
    ui->setupUi(this);
    setTitle("Set material properties");
}

MatPage::~MatPage()
{
    delete ui;
}

int MatPage::nextId() const
{
    return ModelingWizard::MISC_PAGE;
}
