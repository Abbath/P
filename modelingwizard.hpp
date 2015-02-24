#ifndef MODELINGWIZARD_HPP
#define MODELINGWIZARD_HPP

#include <QWizard>
#include <QtWidgets>
#include <iostream>
#include "pages/startpage.hpp"
#include "pages/sizespage.hpp"
#include "pages/matpage.hpp"
#include "pages/miscpage.hpp"
#include "pages/holespage.hpp"
#include "pages/campage.hpp"
#include "helpers.hpp"

/*!
 * \brief The ModelingWizard class
 */
class ModelingWizard : public QWizard
{
    Q_OBJECT
public:
    enum {START_PAGE, SIZE_PAGE, MATERIAL_PAGE, MISC_PAGE, HOLES_PAGE, CAM_PAGE};
    ModelingWizard(QWidget* parent = 0);
    ~ModelingWizard();
    ModelingData& getDataRef();
    void setData(const ModelingData &value);
    
    bool getIs_integrated() const;
    void setIs_integrated(bool value);
    
private:
    ModelingData data;
    bool is_integrated = false;
};

#endif // MODELINGWIZARD_HPP
