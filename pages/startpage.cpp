#include "startpage.hpp"
#include "ui_startpage.h"
#include "../modelingwizard.hpp"
namespace Pages{

/*!
 * \brief StartPage::StartPage
 * \param parent
 */
StartPage::StartPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::StartPage)
{
    ui->setupUi(this);
    setTitle("Set plate deflection modeling type");
    ui->lineEdit->hide();
    ui->pushButton->hide();
}

/*!
 * \brief StartPage::~StartPage
 */
StartPage::~StartPage()
{
    delete ui;
}

/*!
 * \brief StartPage::isExt
 * \return 
 */
bool StartPage::isExt() const
{
    return ui->radioButton_2->isChecked();
}

/*!
 * \brief StartPage::nextId
 * \return 
 */
int StartPage::nextId() const
{
    ModelingWizard * mw = dynamic_cast<ModelingWizard*>(wizard());
    if(isExt()){
        mw->getDataRef().setIs_ext(true);
        mw->getDataRef().setFilename(ui->lineEdit->text());
        return ModelingWizard::HOLES_PAGE;
    }else{
        mw->getDataRef().setIs_ext(false);
        return ModelingWizard::SIZE_PAGE;
    }
}

/*!
 * \brief StartPage::on_pushButton_clicked
 */
void StartPage::on_pushButton_clicked()
{
    QString name = QFileDialog::getOpenFileName(this, "Open data file", ".", "Text files (*.txt)");
    if(!name.isEmpty()){
        ui->lineEdit->setText(name);
    }
}
}
