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

#include <QFont>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include "statustextitem.h"
 
StatusTextItem::StatusTextItem(QGraphicsItem * parent)
	:	QGraphicsTextItem(parent)
{
	setDefaultTextColor(QColor("#222222"));
	setFont(QFont("Segoe UI", 9));
	setOpenExternalLinks(true);
	setTextInteractionFlags(Qt::TextBrowserInteraction);
}

StatusTextItem::StatusTextItem(const QString& text, QGraphicsItem * parent)
	:	QGraphicsTextItem(text, parent)
{
	setDefaultTextColor(QColor("#222222"));
	setFont(QFont("Segoe UI", 9));
	setOpenExternalLinks(true);
	setTextInteractionFlags(Qt::TextBrowserInteraction);
}

void StatusTextItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	QStyleOptionGraphicsItem *o = const_cast<QStyleOptionGraphicsItem*>(option);
	o->state &= ~QStyle::State_Selected;
	QGraphicsTextItem::paint(painter, o, widget);
}