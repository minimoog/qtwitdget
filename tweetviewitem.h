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
#include "listviewiteminterface.h"

class GradientRectItem;
class NetPixmapItem;
class QGraphicsTextItem;
class StatusTextItem;
class PixmapButtonItem;
class QGraphicsLineItem;

class TweetViewItemData : public QSharedData
{
public:
    TweetViewItemData() : width(0), gradRectItem(0), avatarItem(0),
    nameItem(0), textItem(0), replyItem(0), retweetItem(0),
    favoritedItem(0), lineItem(0)
    {}

    // ### Copy Constructor?

    int width;
    GradientRectItem* gradRectItem;
    NetPixmapItem* avatarItem;
    QGraphicsTextItem* nameItem;
    StatusTextItem* textItem;
    PixmapButtonItem* replyItem;
    PixmapButtonItem* retweetItem;
    PixmapButtonItem* favoritedItem;
    QGraphicsLineItem* lineItem;
};

/*!
    Item for the MVC (TweetListView, TweetListModel
 */
class TweetViewItem : public QObject, public ListViewItemInterface
{
    Q_OBJECT

public:
    TweetViewItem(int index, ListViewInterface* view);
    int width() const;
    void setWidth(int w);
    QSizeF size() const;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void itemChanged(const QList<QByteArray> &roles = QList<QByteArray>());
    QVariant data() const;

signals:
    void replyDeleteClicked(int index);
    void retweetClicked(int index);
    void favoritedClicked(int index);
    
private slots:
    void replyDeleteButtonClicked();
    void retweetButtonClicked();
    void favoritedButtonClicked();

private:
    void setData();

    QSharedDataPointer<TweetViewItemData> d;
};

#endif // TWEETVIEWITEM_H
