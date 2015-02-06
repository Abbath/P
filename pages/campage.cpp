#include "campage.hpp"
#include "ui_campage.h"

CamPage::CamPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::CamPage)
{
    ui->setupUi(this);
    setTitle("Set camera angles");
    registerField("CamX", ui->camx);
}

CamPage::~CamPage()
{
    delete ui;
}

int CamPage::nextId() const
{
    return -1;
}
