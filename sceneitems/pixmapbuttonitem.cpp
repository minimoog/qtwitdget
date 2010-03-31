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

#include <QtDebug>
#include <QGraphicsSceneHoverEvent>
#include "pixmapbuttonitem.h"

/*!
    Constructor
    \param Graphics item parent
 */
PixmapButtonItem::PixmapButtonItem(QGraphicsItem *parent)
	:	QGraphicsPixmapItem(parent)
{
	setAcceptHoverEvents(true);
	setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
}

/*!
    Sets hover pixmap
    \param pixmap hover pixmap
 */
void PixmapButtonItem::setHoverPixmap(const QPixmap &pixmap)
{
	m_hoverPixmap = pixmap;
}

/*!
    Sets click pixmap when item is clicked
    \param pixmap Click pixmap
 */
void PixmapButtonItem::setClickedPixmap(const QPixmap &pixmap)
{
	m_clickedPixmap = pixmap;
}

/*!
    \return Hover pixmap
 */
QPixmap PixmapButtonItem::hoverPixmap() const
{
	return m_hoverPixmap;
}

/*!
    \return Clicked pixmap
 */
QPixmap PixmapButtonItem::clickedPixmap() const
{
	return m_clickedPixmap;
}

/*!
    Sets default pixmap
    \param pixmap Default pixmap
 */
void PixmapButtonItem::setDefaultPixmap(const QPixmap &pixmap)
{
	m_defaultPixmap = pixmap;
	setPixmap(m_defaultPixmap);
}

/*!
    \return Default pixmap
 */
QPixmap PixmapButtonItem::defaultPixmap() const
{
	return m_defaultPixmap;
}

/*!
    \reimp
 */
void PixmapButtonItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
	setPixmap(m_hoverPixmap);

	//QGraphicsItem::prepareGeometryChange () ???

	QGraphicsPixmapItem::hoverEnterEvent(e);
}

/*!
    \reimp
 */
void PixmapButtonItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
	setPixmap(m_defaultPixmap);

	QGraphicsPixmapItem::hoverLeaveEvent(e);
}

/*!
    \reimp
 */
void PixmapButtonItem::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
	if(e->button() == Qt::LeftButton){
		setPixmap(m_clickedPixmap);
		emit clicked(m_id);
		e->accept();
	}
}

/*!
    \reimp
 */
void PixmapButtonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
	if(e->button() == Qt::LeftButton){
		setPixmap(m_defaultPixmap);
	}
}
