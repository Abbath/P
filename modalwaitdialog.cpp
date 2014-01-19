#include "modalwaitdialog.h"

ModalWaitDialog::ModalWaitDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(Qt::Dialog);
}
