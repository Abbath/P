#include "holespage.hpp"
#include "ui_holespage.h"
#include "../modelingwizard.hpp"

/*!
 * \brief HolesPage::HolesPage
 * \param parent
 */
HolesPage::HolesPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::HolesPage)
{
    ui->setupUi(this);
    setTitle("Set holes parameters");
}

/*!
 * \brief HolesPage::~HolesPage
 */
HolesPage::~HolesPage()
{
    delete ui;
}

/*!
 * \brief HolesPage::nextId
 * \return 
 */
int HolesPage::nextId() const
{
    ModelingWizard * mw = dynamic_cast<ModelingWizard*>(wizard());
    mw->getDataRef().setRows(ui->rows->value());
    mw->getDataRef().setHole_size(ui->hole_size->value());
    mw->getDataRef().setSpace_size(ui->space_size->value());    
    return ModelingWizard::CAM_PAGE;
}
