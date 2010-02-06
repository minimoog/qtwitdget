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

#include <QResizeEvent>
#include "qtwitview.h"
#include "qtwitscene.h"

QTwitView::QTwitView(QWidget *parent) 
	: QGraphicsView(parent)
{
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	QScrollBar *scrollBar = verticalScrollBar();
	connect(scrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarPos(int)));

    //do translation with translating one root item
    setTransformationAnchor(QGraphicsView::NoAnchor);
}

void QTwitView::scrollBarPos(int pos)
{
	if(pos == verticalScrollBar()->maximum() && verticalScrollBar()->isEnabled())
		emit scrollBarMaxPos(true);
	else
		emit scrollBarMaxPos(false);
}

void QTwitView::resizeEvent(QResizeEvent *e)
{
	QSize s = e->size();
	QTwitScene* twitScene = qobject_cast<QTwitScene*>(scene());
	if(twitScene){
		twitScene->resizeItems(s.width() - 1);
		twitScene->setSceneRect(0, 0, s.width(), twitScene->boundingHeight());
	}

	QWidget::resizeEvent(e);
}
