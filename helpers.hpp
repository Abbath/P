#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <QtCore>
#include <QtGui>
#include <QPointF>
#include <array>
#include <numeric>
#include "config.hpp"
#include "image.hpp"
#include "modelingdata.hpp"

/*!
 * \brief The Display struct
 */
struct Display
{
    Image im;
    QPair<QPoint, QPoint> origin;
};



#endif // HELPERS_HPP









