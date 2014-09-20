#include "dialog.hpp"
#include "ui_dialog.h"

/*!
 * \brief Dialog::Dialog
 * \param parent
 */
Dialog::Dialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

/*!
 * \brief Dialog::~Dialog
 */
Dialog::~Dialog()
{
    delete ui;
}
