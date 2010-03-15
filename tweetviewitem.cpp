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
#include <QUrl>
#include "sceneitems/pixmapbuttonitem.h"
#include "sceneitems/gradientrectitem.h"
#include "sceneitems/netpixmapitem.h"
#include "sceneitems/statustextitem.h"
#include "tweetviewitem.h"
#include "mainwindow.h"
#include "qtwit/qtwitstatus.h"

TweetViewItem::TweetViewItem(int index, TweetListView *view)
    : QGraphicsItem(view), d(new TweetViewItemData)
{
    d->listView = view;
    d->index = index;

    d->gradRectItem = new GradientRectItem(200, GradientRectItem::Blue, this);

    QGraphicsPixmapItem *avatarBoxItem = new QGraphicsPixmapItem(QPixmap(":/images/avatar_box.png"), d->gradRectItem);
    avatarBoxItem->setPos(7, 7);

    QGraphicsRectItem *whiteBorderItem = new QGraphicsRectItem(0, 0, 49, 49, avatarBoxItem);
    whiteBorderItem->setPen(QPen(Qt::white));
    whiteBorderItem->setBrush(QBrush(Qt::NoBrush));
    whiteBorderItem->setPos(10, 10);

    d->avatarItem = new NetPixmapItem(MainWindow::networkAccessManager(), whiteBorderItem);
    d->avatarItem->setPos(1, 1);

    d->nameItem = new QGraphicsTextItem(d->gradRectItem);
    d->nameItem->setDefaultTextColor(QColor("#018ad9"));
    d->nameItem->setFont(QFont("Segoe UI", 11));
    d->nameItem->setPos(84, 0);

    d->textItem = new StatusTextItem(d->gradRectItem);
    d->textItem->setPos(84, 24);

    d->replyItem = new PixmapButtonItem(d->gradRectItem);
    d->replyItem->setDefaultPixmap(QPixmap(":/images/button_reply.png"));
    d->replyItem->setHoverPixmap(QPixmap(":/images/button_reply_hover.png"));
    d->replyItem->setClickedPixmap(QPixmap(":/images/button_reply_click.png"));
    d->replyItem->setPos(10, 80);
    d->replyItem->setToolTip(QObject::tr("Reply to this status"));

    d->retweetItem = new PixmapButtonItem(d->gradRectItem);
    d->retweetItem->setDefaultPixmap(QPixmap(":/images/button_retweet.png"));
    d->retweetItem->setHoverPixmap(QPixmap(":/images/button_retweet_hover.png"));
    d->retweetItem->setClickedPixmap(QPixmap(":/images/button_retweet_click.png"));
    d->retweetItem->setPos(43, 80);
    d->retweetItem->setToolTip(QObject::tr("Retweet this status"));

    d->favoritedItem = new PixmapButtonItem(d->gradRectItem);
    d->favoritedItem->setDefaultPixmap(QPixmap(":/images/button_favorited.png"));
    d->favoritedItem->setHoverPixmap(QPixmap(":/images/button_favorited_hover.png"));
    d->favoritedItem->setClickedPixmap(QPixmap(":/images/button_favorited_click.png"));
    d->favoritedItem->setToolTip(QObject::tr("Favorite this status"));

    d->lineItem = new QGraphicsLineItem(d->gradRectItem);
    d->lineItem->setPen(QPen(QColor("#DDDDDD")));

    setData();
}

int TweetViewItem::index() const
{
    return d->index;
}

void TweetViewItem::setIndex(int index)
{
    d->index = index;
    itemChanged();
}

int TweetViewItem::width() const
{
    return d->width;
}

void TweetViewItem::setWidth(int w)
{
    d->width = w;

    d->gradRectItem->setWidth(w);
    d->textItem->setTextWidth(w - 84 -10);
    d->favoritedItem->setPos(w - 50, 80);
    d->lineItem->setLine(1, 99, w - 1, 99);
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
    return d->listView;
}

QSizeF TweetViewItem::size() const
{
    return QSizeF(d->width, 101.0f);
}

QRectF TweetViewItem::boundingRect() const
{
    return QRectF();
}

void TweetViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}

void TweetViewItem::setData()
{
    QTwitStatus s = d->listView->model()->data(d->index).value<QTwitStatus>();
    d->avatarItem->setPixmapUrl(QUrl(s.profileImageUrl()));
    d->nameItem->setPlainText(s.screenName());
    d->textItem->setHtml(s.text());

    if (s.isRead())
        d->gradRectItem->setGradient(GradientRectItem::White);
    else
        d->gradRectItem->setGradient(GradientRectItem::Blue);
}
