/* Copyright (c) 2010, Antonie Jovanoski
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

#include "tweetlistview.h"
#include "tweetviewitem.h"

TweetListView::TweetListView(QGraphicsItem * parent)
    : QGraphicsItem(parent), m_model(0)
{
}

TweetListModel* TweetListView::model() const
{
    return m_model;
}

void TweetListView::setModel(TweetListModel *model)
{
    if (model == m_model)
        return;

    if (m_model) {
        disconnect(m_model, SIGNAL(itemsChanged(int,int,QList<QByteArray>)),
                   this, SLOT(itemsChanged(int,int,QList<QByteArray>)));
        disconnect(m_model, SIGNAL(itemsInserted(int,int)), this, SLOT(itemsInserted(int,int)));
        disconnect(m_model, SIGNAL(itemsRemoved(int,int)), this, SLOT(itemsRemoved(int,int)));
        disconnect(m_model, SIGNAL(itemsMoved(int,int,int)), this, SLOT(itemsMoved(int,int,int)));
        // ### signal: destroyed()  ????
    }

    m_model = model;

    if (m_model) {
        connect(m_model, SIGNAL(itemsChanged(int,int,QList<QByteArray>)),
                this, SLOT(itemsChanged(int,int,QList<QByteArray>)));
        connect(m_model, SIGNAL(itemsInserted(int,int)), this, SLOT(itemsInserted(int,int)));
        connect(m_model, SIGNAL(itemsRemoved(int,int)), this, SLOT(itemsRemoved(int,int)));
        connect(m_model, SIGNAL(itemsMoved(int,int,int)), this, SLOT(itemsMoved(int,int,int)));
        // ### signal: destroyed()  ????
    }
}

int TweetListView::itemAt(const QPointF &position) const
{
    // ### TODO
    return 0;
}

QSizeF TweetListView::itemGeometry(int index) const
{
    // ### TODO
    return QSizeF();
}

QGraphicsItem* TweetListView::itemForIndex(int index) const
{
    if (index >=0 && index < m_model->count()) {
        return m_items.at(index);
    }

    return 0;
}

void TweetListView::itemsInserted(int index, int count)
{
    //find from position which item to move
    //  !!! only vertical !!!

    //find from which position to move/insert

    QPointF posFrom(0, 0);

    if (m_items.count())
        posFrom = m_items.at(index)->mapToParent(QPointF(0, 0));

    qreal ymovement = 0.0;

    //insert items;
    for (int i = 0; i < count; ++i) {
        TweetViewItem *item = new TweetViewItem(index + i, this);
        m_items.insert(index + i, item);
        item->setPos(posFrom.x(), posFrom.y() + ymovement);
        ymovement += item->size().height();
    }

    //move rest of items down
    for (int i = index + count; i < m_items.count(); ++i) {
        m_items.at(i)->moveBy(0, ymovement);
        m_items.at(i)->setIndex(i);
    }
}

void TweetListView::itemsRemoved(int index, int count)
{
    QPointF posTo = m_items.at(index)->mapToParent(QPointF(0, 0));
    QPointF posFrom = m_items.at(index + count)->mapToParent(QPointF(0, 0));

    //remove items
    for (int i = 0; i < count; ++i) {
        TweetViewItem *item = m_items.takeAt(index + i);
        delete item;
    }

    //move rest up
    for (int i = index; i < m_items.count(); ++i) {
        m_items.at(i)->moveBy(posTo.x(), posFrom.y() - posTo.y());
        m_items.at(i)->setIndex(i);
    }
}

void TweetListView::itemsMoved(int from, int to, int count)
{
    // ### TODO
}

void TweetListView::itemsChanged(int index, int count, const QList<QByteArray> &roles)
{
    for (int i = 0; i < count; ++i) {
        m_items.at(index + i)->itemChanged(roles);
    }
}

QRectF TweetListView::boundingRect() const
{
    // nothing to do
    return QRectF();
}

void TweetListView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // nothing to paint, just a holder for list items
}
