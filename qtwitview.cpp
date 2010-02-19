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
#include <QPropertyAnimation>
#include "qtwitview.h"
#include "qtwitscene.h"

QTwitView::QTwitView(QWidget *parent) 
    : QGraphicsView(parent),
      m_scrbarAnimation(new QPropertyAnimation(this, "valueVertScrollBar", this))
{
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	QScrollBar *scrollBar = verticalScrollBar();
	connect(scrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarPos(int)));

    //do translation with translating one root item
    //setTransformationAnchor(QGraphicsView::NoAnchor);

    m_scrbarAnimation->setDuration(500);
    m_scrbarAnimation->setEasingCurve(QEasingCurve::InOutExpo);
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

void QTwitView::setValueVertScrollbar(int value)
{
    verticalScrollBar()->setValue(value);
}

int QTwitView::valueVertScrollbar() const
{
    return verticalScrollBar()->value();
}

void QTwitView::moveToPoint(qreal y)
{
    qreal maxYMovement = sceneRect().height() - viewport()->height();

    if (maxYMovement > 0) {
        int vBarNextPosition = 0;
        int vBarMin = verticalScrollBar()->minimum();
        int vBarMax = verticalScrollBar()->maximum();

        if (y > maxYMovement) {
            vBarNextPosition = vBarMax;
        } else {
            //WHY -18????? FIX THIS !!!!!!!
            vBarNextPosition = vBarMin + (vBarMax - vBarMin) * y / maxYMovement - 18;
        }

        m_scrbarAnimation->setEndValue(vBarNextPosition);
        m_scrbarAnimation->start();
    }
}
