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
#include <QParallelAnimationGroup>
#include "gradientrectitem.h"

GradientRectItem::GradientRectItem(QGraphicsItem * parent)
    :	QGraphicsRectItem(parent), m_gradient1Animation(new QPropertyAnimation(this, "firstGradientColor", this)),
    m_gradient2Animation(new QPropertyAnimation(this, "secondGradientColor", this)),
    m_parallelAnimation(new QParallelAnimationGroup(this)),
    m_fadeinoutAnimation(new QPropertyAnimation(this, "opacity", this)),
    m_moveAnimation(new QPropertyAnimation(this, "pos", this))
{
    setPen(QPen(Qt::NoPen));
    m_parallelAnimation->addAnimation(m_gradient1Animation);
    m_parallelAnimation->addAnimation(m_gradient2Animation);
    connect(m_fadeinoutAnimation, SIGNAL(finished()), this, SIGNAL(fadeInOutFinished()));
}

GradientRectItem::GradientRectItem(qreal width, GradientRectItem::Gradient gradient, QGraphicsItem * parent)
    :   QGraphicsRectItem(parent), m_gradient1Animation(new QPropertyAnimation(this, "firstGradientColor", this)),
    m_gradient2Animation(new QPropertyAnimation(this, "secondGradientColor", this)),
    m_parallelAnimation(new QParallelAnimationGroup (this)),
    m_fadeinoutAnimation(new QPropertyAnimation(this, "opacity", this)),
    m_moveAnimation(new QPropertyAnimation(this, "pos", this))
{
    setWidth(width);
    setPen(QPen(Qt::NoPen));
    setGradient(gradient);

    m_parallelAnimation->addAnimation(m_gradient1Animation);
    m_parallelAnimation->addAnimation(m_gradient2Animation);
    connect(m_fadeinoutAnimation, SIGNAL(finished()), this, SIGNAL(fadeInOutFinished()));
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
        case Green:
            setFirstGradientColor(QColor("#DDF5D3"));
            setSecondGradientColor(QColor("#E5FFDA"));
            break;
        case Blue:
            setFirstGradientColor(QColor("#DDF0F5"));
            setSecondGradientColor(QColor("#E5FAFF"));
            break;
        case Grey:
            setFirstGradientColor(QColor("#E3E3E3"));
            setSecondGradientColor(QColor("#EBEBEB"));
            break;
        case Pink:
            setFirstGradientColor(QColor("#F5E1E1"));
            setSecondGradientColor(QColor("#FFE9E9"));
            break;
        case White:
            setFirstGradientColor(QColor("#FFFFFF"));
            setSecondGradientColor(QColor("#FFFFFF"));
            break;
    }

    setTwoColorGradient();
}

void GradientRectItem::setGradientAnim(GradientRectItem::Gradient gradient)
{
    m_gradient1Animation->setDuration(200);
    m_gradient2Animation->setDuration(200);

    switch (gradient) {
    case Green:
       m_gradient1Animation->setEndValue(QColor("#DDF5D3"));
       m_gradient2Animation->setEndValue(QColor("#E5FFDA"));
       break;
    case Blue:
       m_gradient1Animation->setEndValue(QColor("#DDF0F5"));
       m_gradient2Animation->setEndValue(QColor("#E5FAFF"));
       break;
    case Grey:
       m_gradient1Animation->setEndValue(QColor("#E3E3E3"));
       m_gradient2Animation->setEndValue(QColor("#EBEBEB"));
       break;
    case White:
       m_gradient1Animation->setEndValue(QColor("#FFFFFF"));
       m_gradient2Animation->setEndValue(QColor("#FFFFFF"));
    }

    //m_gradient1Animation->start();
    m_parallelAnimation->start();
}

QColor GradientRectItem::firstGradientColor() const
{
    return m_firstGradColor;
}

void GradientRectItem::setFirstGradientColor(const QColor &color)
{
    m_firstGradColor = color;
    setTwoColorGradient();
}

void GradientRectItem::setSecondGradientColor(const QColor &color)
{
    m_secondGradColor  = color;
    setTwoColorGradient();
}

QColor GradientRectItem::secondGradientColor() const
{
    return m_secondGradColor;
}

void GradientRectItem::setTwoColorGradient()
{
    QLinearGradient linearGradient(0, 0, 0, 1);
    linearGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    linearGradient.setColorAt(0, m_firstGradColor);
    linearGradient.setColorAt(1, m_secondGradColor);

    setBrush(linearGradient);
}

void GradientRectItem::startFadeOutAnim()
{
    m_fadeinoutAnimation->setDuration(200);
    m_fadeinoutAnimation->setStartValue(1.0);
    m_fadeinoutAnimation->setEndValue(0.0);
    m_fadeinoutAnimation->start();
}

void GradientRectItem::startMoveAnimY(qreal dy)
{
    m_moveAnimation->setDuration(200);
    m_moveAnimation->setEndValue(QPointF(0, pos().y() + dy));
    m_moveAnimation->start();
}
