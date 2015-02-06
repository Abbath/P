#include "modelingwizard.hpp"

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
ModelingData &ModelingWizard::getDataRef()
{
    return data;
}

void ModelingWizard::setData(const ModelingData &value)
{
    data = value;
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

