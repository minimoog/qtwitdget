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
#include "tweetlistview.h"

class GradientRectItem;
class NetPixmapItem;
class QGraphicsTextItem;
class StatusTextItem;
class PixmapButtonItem;
class QGraphicsLineItem;

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

    int m_index;
    TweetListView* m_listView;
    int m_width;

    GradientRectItem* m_gradRectItem;
    NetPixmapItem* m_avatarItem;
    QGraphicsTextItem* m_nameItem;
    StatusTextItem* m_textItem;
    PixmapButtonItem* m_replyButtonItem;
    PixmapButtonItem* m_retweetItem;
    PixmapButtonItem* m_favoritedItem;
    QGraphicsLineItem* m_lineItem;
};

#endif // TWEETVIEWITEM_H
