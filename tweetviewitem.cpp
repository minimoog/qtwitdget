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

#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QPen>
#include "sceneitems/pixmapbuttonitem.h"
#include "sceneitems/gradientrectitem.h"
#include "sceneitems/netpixmapitem.h"
#include "sceneitems/statustextitem.h"
#include "tweetviewitem.h"
#include "mainwindow.h"

TweetViewItem::TweetViewItem(int index, TweetListView *view)
    : QGraphicsItem(view), m_listView(view), m_index(index)
{
    m_gradRectItem = new GradientRectItem(200, GradientRectItem::Blue, this);

    QGraphicsPixmapItem *avatarBoxItem = new QGraphicsPixmapItem(QPixmap(":/images/avatar_box.png"), m_gradRectItem);
    avatarBoxItem->setPos(7, 7);

    QGraphicsRectItem *whiteBorderItem = new QGraphicsRectItem(0, 0, 49, 49, avatarBoxItem);
    whiteBorderItem->setPen(QPen(Qt::white));
    whiteBorderItem->setBrush(QBrush(Qt::NoBrush));
    whiteBorderItem->setPos(10, 10);

    NetPixmapItem *m_avatarItem = new NetPixmapItem(MainWindow::networkAccessManager(), whiteBorderItem);
    m_avatarItem->setPos(1, 1);

    QGraphicsTextItem *m_nameItem = new QGraphicsTextItem(m_gradRectItem);
    m_nameItem->setDefaultTextColor(QColor("#018ad9"));
    m_nameItem->setFont(QFont("Segoe UI", 11));
    m_nameItem->setPos(84, 0);

    StatusTextItem *m_textItem = new StatusTextItem(m_gradRectItem);
    m_textItem->setPos(84, 24);

    PixmapButtonItem *m_replyItem = new PixmapButtonItem(m_gradRectItem);
    m_replyItem->setDefaultPixmap(QPixmap(":/images/button_reply.png"));
    m_replyItem->setHoverPixmap(QPixmap(":/images/button_reply_hover.png"));
    m_replyItem->setClickedPixmap(QPixmap(":/images/button_reply_click.png"));
    m_replyItem->setPos(10, 80);
    m_replyItem->setToolTip(QObject::tr("Reply to this status"));

    PixmapButtonItem *m_retweetItem = new PixmapButtonItem(m_gradRectItem);
    m_retweetItem->setDefaultPixmap(QPixmap(":/images/button_retweet.png"));
    m_retweetItem->setHoverPixmap(QPixmap(":/images/button_retweet_hover.png"));
    m_retweetItem->setClickedPixmap(QPixmap(":/images/button_retweet_click.png"));
    m_retweetItem->setPos(43, 80);
    m_retweetItem->setToolTip(QObject::tr("Retweet this status"));

    PixmapButtonItem *m_favoritedItem = new PixmapButtonItem(m_gradRectItem);
    m_favoritedItem->setDefaultPixmap(QPixmap(":/images/button_favorited.png"));
    m_favoritedItem->setHoverPixmap(QPixmap(":/images/button_favorited_hover.png"));
    m_favoritedItem->setClickedPixmap(QPixmap(":/images/button_favorited_click.png"));
    m_favoritedItem->setToolTip(QObject::tr("Favorite this status"));

    QGraphicsLineItem *m_lineItem = new QGraphicsLineItem(m_gradRectItem);
    m_lineItem->setPen(QPen(QColor("#DDDDDD")));
}

int TweetViewItem::index() const
{
    return m_index;
}

void TweetViewItem::setIndex(int index)
{
    m_index = index;
    itemChanged();
}

int TweetViewItem::width() const
{
    return m_width;
}

void TweetViewItem::setWidth(int w)
{
    m_width = w;

    m_gradRectItem->setWidth(w);
    m_textItem->setTextWidth(w - 84 -10);
    m_favoritedItem->setPos(w - 50, 80);
    m_lineItem->setLine(1, 99, w - 1, 99);
}

void TweetViewItem::itemChanged(const QList<QByteArray> &roles)
{
    // ### TODO
}

QVariant TweetViewItem::data() const
{
    return QVariant();
}

TweetListView* TweetViewItem::view() const
{
    return m_listView;
}

QSizeF TweetViewItem::size() const
{
    return QSizeF(m_width, 101.0f);
}

QRectF TweetViewItem::boundingRect() const
{
    return QRectF();
}

void TweetViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}
