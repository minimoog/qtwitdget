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

#ifndef PIXMAPBUTTONITEM_H
#define PIXMAPBUTTONITEM_H
 
#include <QGraphicsPixmapItem>

class PixmapButtonItem : public  QObject, QGraphicsPixmapItem
{
	Q_OBJECT
public:
	PixmapButtonItem(QGraphicsItem *parent = 0);
	void setHoverPixmap(const QPixmap &pixmap);
	QPixmap hoverPixmap() const;
	void setClickedPixmap(const QPixmap &pixmap);
	QPixmap clickedPixmap() const;
signals:
	void clicked();
protected:
	void hoverEnterEvent(QGraphicsSceneHoverEvent *e);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *e);
private:
	QPixmap m_hoverPixmap;
	QPixmap m_clickedPixmap;
	QPixmap m_defaultPixmap;
};

#endif //PIXMAPBUTONITEM_H