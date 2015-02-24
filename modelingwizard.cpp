#include "modelingwizard.hpp"

/*!
 * \brief ModelingWizard::ModelingWizard
 * \param parent
 */
ModelingWizard::ModelingWizard(QWidget* parent) : QWizard(parent)
{
    setPage(START_PAGE, new Pages::StartPage);
    setPage(SIZE_PAGE, new SizesPage);
    setPage(MATERIAL_PAGE, new MatPage);
    setPage(MISC_PAGE, new MiscPage);
    setPage(HOLES_PAGE, new HolesPage);
    setPage(CAM_PAGE, new CamPage);
    setStartId(START_PAGE);
}

/*!
 * \brief ModelingWizard::~ModelingWizard
 */
ModelingWizard::~ModelingWizard()
{
    
}

/*!
 * \brief ModelingWizard::getDataRef
 * \return 
 */
ModelingData &ModelingWizard::getDataRef()
{
    return data;
}

/*!
 * \brief ModelingWizard::setData
 * \param value
 */
void ModelingWizard::setData(const ModelingData &value)
{
    data = value;
}

/*!
 * \brief ModelingWizard::getIs_integrated
 * \return 
 */
bool ModelingWizard::getIs_integrated() const
{
    return is_integrated;
}

/*!
 * \brief ModelingWizard::setIs_integrated
 * \param value
 */
void ModelingWizard::setIs_integrated(bool value)
{
    is_integrated = value;
    if(value){
        setStartId(SIZE_PAGE);
    }
}
