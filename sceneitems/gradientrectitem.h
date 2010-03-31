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
 
#ifndef GRADIENTRECTITEM_H
#define GRADIENTRECTITEM_H

#include <QGraphicsRectItem>

class QPropertyAnimation;
class QParallelAnimationGroup;

/*!
    Simple rectangle item with linear gradient
 */
class GradientRectItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
    Q_PROPERTY(QColor firstGradientColor READ firstGradientColor WRITE setFirstGradientColor)
    Q_PROPERTY(QColor secondGradientColor READ secondGradientColor WRITE setSecondGradientColor)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    enum Gradient {
        Green,
        Blue,
        Grey,
        Pink,
        White
    };

	GradientRectItem(QGraphicsItem * parent = 0);
    GradientRectItem(qreal width, GradientRectItem::Gradient gradient, QGraphicsItem * parent = 0);
    void setWidth(qreal width);
    qreal width() const;
    void setGradient(GradientRectItem::Gradient gradient);
    void setGradientAnim(GradientRectItem::Gradient gradient);
    void startFadeOutAnim();
    void startMoveAnimY(qreal dy);

    QColor firstGradientColor() const;
    void setFirstGradientColor(const QColor& color);
    QColor secondGradientColor() const;
    void setSecondGradientColor(const QColor& color);

signals:
    void fadeInOutFinished();

private:
    void setTwoColorGradient();

    QColor m_firstGradColor;
    QColor m_secondGradColor;
    QPropertyAnimation *m_gradient1Animation;
    QPropertyAnimation *m_gradient2Animation;
    QParallelAnimationGroup *m_parallelAnimation;
    QPropertyAnimation *m_fadeinoutAnimation;
    QPropertyAnimation *m_moveAnimation;
};

#endif //GRADIENTRECTITEM_H
