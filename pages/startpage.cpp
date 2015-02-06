#include "startpage.hpp"
#include "ui_startpage.h"
#include "modelingwizard.hpp"

StartPage::StartPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::StartPage)
{
    ui->setupUi(this);
    setTitle("Set plate deflection modeling type");
    ui->lineEdit->hide();
    ui->pushButton->hide();
}

StartPage::~StartPage()
{
    delete ui;
}

bool StartPage::isExt()
{
    return ui->radioButton_2->isChecked();
}

int StartPage::nextId() const
{
    if(ui->radioButton_2->isChecked()){
        return ModelingWizard::HOLES_PAGE;
    }else{
        return ModelingWizard::SIZE_PAGE;
    }
}
