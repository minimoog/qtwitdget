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

#ifndef STATUSTEXTITEM_H
#define STATUSTEXTITEM_H

#include <QGraphicsTextItem>

class StatusTextItem : public QGraphicsTextItem
{
public:
	StatusTextItem(QGraphicsItem * parent = 0);
	StatusTextItem(const QString& text, QGraphicsItem * parent = 0);
	
	virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
};

#endif //STATUSTEXTITEM_H