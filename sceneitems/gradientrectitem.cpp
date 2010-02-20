/* Copyright (c) 2009, Antonie Jovanoski
 *	
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#include <QLinearGradient>
#include <QPen>
#include <QPropertyAnimation>
#include "gradientrectitem.h"

GradientRectItem::GradientRectItem(QGraphicsItem * parent)
    :	QGraphicsRectItem(parent), m_gradientAnimation(new QPropertyAnimation(this, "gradientColor", this))
{
    setPen(QPen(Qt::NoPen));
}

GradientRectItem::GradientRectItem(qreal width, GradientRectItem::Gradient gradient, QGraphicsItem * parent)
    :   QGraphicsRectItem(parent), m_gradientAnimation(new QPropertyAnimation(this, "gradientColor", this))
{
    setWidth(width);
    setPen(QPen(Qt::NoPen));
    setGradient(gradient);
}

void GradientRectItem::setWidth(qreal width)
{
    QRectF rectItem = rect();
    rectItem.setWidth(width);
    rectItem.setHeight(100.0);    //fixed height
    rectItem.setX(0);
    rectItem.setY(0);
    
    setRect(rectItem);
}

qreal GradientRectItem::width() const
{
    return rect().width();
}

void GradientRectItem::setGradient(GradientRectItem::Gradient gradient)
{
    switch (gradient) {
        case Grey:
            setGradientColor(QColor(238, 238, 238, 127));
            break;
        case Pink:
            setGradientColor(QColor(255, 223, 223, 127));
            break;
        case Blue:
            setGradientColor(QColor(220, 238, 243, 127));
            break;
        case TestBlue:
            setGradientColor(QColor(0, 190, 243, 127));
    }
}

void GradientRectItem::setGradientAnim(GradientRectItem::Gradient gradient)
{
    m_gradientAnimation->setDuration(500);

    switch (gradient) {
    case Grey:
       m_gradientAnimation->setEndValue(QColor(238, 238, 238, 127));
       break;
    case Pink:
       m_gradientAnimation->setEndValue(QColor(255, 223, 223, 127));
       break;
    case Blue:
       m_gradientAnimation->setEndValue(QColor(220, 238, 243, 127));
       break;
    case TestBlue:
       m_gradientAnimation->setEndValue(QColor(0, 190, 243, 127));
    }

    m_gradientAnimation->start();
}

QColor GradientRectItem::gradientColor() const
{
    return m_linearGradColor;
}

void GradientRectItem::setGradientColor(const QColor &color)
{
    m_linearGradColor = color;

    QLinearGradient linearGradient(0, 0, 0, 1);
    linearGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    linearGradient.setColorAt(0, color);
    linearGradient.setColorAt(1, QColor(255, 255, 255, 255));

    setBrush(linearGradient);
}
