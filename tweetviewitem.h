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

#ifndef TWEETVIEWITEM_H
#define TWEETVIEWITEM_H

#include <QGraphicsItem>
#include <QSharedData>
#include "tweetlistview.h"

class GradientRectItem;
class NetPixmapItem;
class QGraphicsTextItem;
class StatusTextItem;
class PixmapButtonItem;
class QGraphicsLineItem;

class TweetViewItemData : public QSharedData
{
public:
    TweetViewItemData() : index(0), width(0), listView(0), gradRectItem(0),
    avatarItem(0), nameItem(0), textItem(0), replyItem(0), retweetItem(0),
    favoritedItem(0), lineItem(0)
    {}

    // ### Copy Constructor?

    int index;
    int width;
    TweetListView* listView;
    GradientRectItem* gradRectItem;
    NetPixmapItem* avatarItem;
    QGraphicsTextItem* nameItem;
    StatusTextItem* textItem;
    PixmapButtonItem* replyItem;
    PixmapButtonItem* retweetItem;
    PixmapButtonItem* favoritedItem;
    QGraphicsLineItem* lineItem;
};


class TweetViewItem : public QGraphicsItem
{

public:
    TweetViewItem(int index, TweetListView * view);
    int index() const;
    void setIndex(int index);
    int width() const;
    void setWidth(int w);
    QSizeF size() const;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void itemChanged(const QList<QByteArray> &roles = QList<QByteArray>());
    QVariant data() const;
    TweetListView* view() const;
    
private:
    void setData();

    QSharedDataPointer<TweetViewItemData> d;
};

#endif // TWEETVIEWITEM_H
