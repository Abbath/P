#include "modelingwizard.hpp"

ModelingWizard::ModelingWizard(QWidget* parent)
{
    setPage(START_PAGE, new Pages::StartPage);
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
bool ModelingWizard::getIs_integrated() const
{
    return is_integrated;
}

void ModelingWizard::setIs_integrated(bool value)
{
    is_integrated = value;
    if(value){
        setStartId(SIZE_PAGE);
    }
}
