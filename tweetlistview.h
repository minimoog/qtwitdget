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

#ifndef TWEETLISTVIEW_H
#define TWEETLISTVIEW_H

#include <QGraphicsItem>
#include "tweetlistmodel.h"
//#include "tweetviewitem.h"

class TweetViewItem;

class TweetListView : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    TweetListView(QGraphicsItem * parent = 0);
    TweetListModel* model() const;
    void setModel(TweetListModel *model);
    int itemAt(const QPointF &position) const;
    QSizeF itemGeometry(int index) const;
    QGraphicsItem* itemForIndex(int index) const;
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public slots:
    void resizeWidth(int w);

private slots:
    void itemsInserted(int index, int count);
    void itemsRemoved(int index, int count);
    void itemsMoved(int from, int to, int count);
    void itemsChanged(int index, int count, const QList<QByteArray> &roles);

private:
    TweetListModel *m_model;
    QList<TweetViewItem*> m_items;
};

#endif // TWEETLISTVIEW_H
