#include "modelingwizard.hpp"
#include "pages/startpage.hpp"
#include "pages/sizespage.hpp"
#include "pages/matpage.hpp"
#include "pages/miscpage.hpp"
#include "pages/holespage.hpp"
#include "pages/campage.hpp"

ModelingWizard::ModelingWizard(QWidget* parent)
{
    setPage(START_PAGE, new StartPage);
    setPage(SIZE_PAGE, new SizesPage);
    setPage(MATERIAL_PAGE, new MatPage);
    setPage(MISC_PAGE, new MiscPage);
    setPage(HOLES_PAGE, new HolesPage);
    setPage(CAM_PAGE, new CamPage);
    setStartId(START_PAGE);
}

ModelingWizard::~ModelingWizard()
{
    
}

QWizardPage *ModelingWizard::createPage()
{
    QWizardPage *page = new QWizardPage;
    QLabel *label = new QLabel("Test", page);
    QRadioButton *rb = new QRadioButton;
    QVBoxLayout* vl = new QVBoxLayout(page);
    vl->addWidget(label);
    page->setLayout(vl);
    //page->registerField("Label", label);
    return page;
}

