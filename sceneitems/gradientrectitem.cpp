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
#include "gradientrectitem.h"

GradientRectItem::GradientRectItem(QGraphicsItem * parent)
	:	QGraphicsRectItem(parent)
{
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

void GradientRectItem::setGradient(GradientRectItem::Gradient gradient)
{
    QLinearGradient linearGradient(0, 0, 0, 1);

    switch(gradient){
        case Grey:
            linearGradient.setColorAt(0, QColor(238, 238, 238));
            linearGradient.setColorAt(1, QColor(255, 255, 255));
            break;
        case Pink:
            linearGradient.setColorAt(0, QColor(255, 223, 223));
            linearGradient.setColorAt(1, QColor(255, 255, 255));
            break;
        case Blue:
            linearGradient.setColorAt(0, QColor(220, 238, 243));
            linearGradient.setColorAt(1, QColor(255, 255, 255));
    }

    setBrush(linearGradient);
}