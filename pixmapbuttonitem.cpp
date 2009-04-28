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
 * Contact e-mail: Antonie Jovanoski <minimoog77@gmail.com>
 */

#include <QtDebug>
#include <QGraphicsSceneHoverEvent>
#include "pixmapbuttonitem.h"

PixmapButtonItem::PixmapButtonItem(QGraphicsItem *parent)
	:	QGraphicsPixmapItem(parent)
{
	setAcceptHoverEvents(true);
	setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
	setToolTip(tr("PixmapButtonItem"));
}

void PixmapButtonItem::setHoverPixmap(const QPixmap &pixmap)
{
	m_hoverPixmap = pixmap;
}

void PixmapButtonItem::setClickedPixmap(const QPixmap &pixmap)
{
	m_clickedPixmap = pixmap;
}

QPixmap PixmapButtonItem::hoverPixmap() const
{
	return m_hoverPixmap;
}

QPixmap PixmapButtonItem::clickedPixmap() const
{
	return m_clickedPixmap;
}

void PixmapButtonItem::setDefaultPixmap(const QPixmap &pixmap)
{
	m_defaultPixmap = pixmap;
	setPixmap(m_defaultPixmap);
}

QPixmap PixmapButtonItem::defaultPixmap() const
{
	return m_defaultPixmap;
}


void PixmapButtonItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
	setPixmap(m_hoverPixmap);

	//QGraphicsItem::prepareGeometryChange () ???

	QGraphicsPixmapItem::hoverEnterEvent(e);
}

void PixmapButtonItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
	setPixmap(m_defaultPixmap);

	QGraphicsPixmapItem::hoverLeaveEvent(e);
}
