#ifndef MODELINGWIZARD_HPP
#define MODELINGWIZARD_HPP

#include <QWizard>
#include <QtWidgets>
#include <iostream>

class ModelingWizard : public QWizard
{
    Q_OBJECT
public:
    enum {START_PAGE, SIZE_PAGE, MATERIAL_PAGE, MISC_PAGE, HOLES_PAGE, CAM_PAGE};
    ModelingWizard(QWidget* parent = 0);
    ~ModelingWizard();
private:
    bool is_ext = false;
    QWizardPage* createPage();
};

#endif // MODELINGWIZARD_HPP
