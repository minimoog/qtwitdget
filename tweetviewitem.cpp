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

#include <QPen>
#include "tweetviewitem.h"

////SET WIDTH!!!

TweetViewItem::TweetViewItem(int index, QtGraphicsListView *view) :
    QtGraphicsListViewItem(index, view)
{
    m_gradRectItem = new GradientRectItem(200, GradientRectItem::Blue, this);

    QGraphicsPixmapItem *avatarBoxItem = new QGraphicsPixmapItem(QPixmap(":/images/avatar_box.png"), m_gradRectItem);
    avatarBoxItem->setPos(7, 7);

    QGraphicsRectItem *whiteBorderItem = new QGraphicsRectItem(0, 0, 49, 49, avatarBoxItem);
    whiteBorderItem->setPen(QPen(Qt::white));
    whiteBorderItem->setBrush(QBrush(Qt::NoBrush));
    whiteBorderItem->setPos(10, 10);

    m_avatarItem = new NetPixmapItem(0 /*m_netManager*/, whiteBorderItem);
    m_avatarItem->setPos(1, 1);

    m_nameItem = new QGraphicsTextItem(m_gradRectItem);
    m_nameItem->setDefaultTextColor(QColor("#018ad9"));
    m_nameItem->setFont(QFont("Segoe UI", 11));
    m_nameItem->setPos(84, 0);

    m_textItem = new StatusTextItem(m_gradRectItem);
    m_textItem->setPos(84, 24);

    m_replyButtonItem = new PixmapButtonItem(m_gradRectItem);
    m_replyButtonItem->setDefaultPixmap(QPixmap(":/images/button_reply.png"));
    m_replyButtonItem->setHoverPixmap(QPixmap(":/images/button_reply_hover.png"));
    m_replyButtonItem->setClickedPixmap(QPixmap(":/images/button_reply_click.png"));
    m_replyButtonItem->setPos(10, 80);
    m_replyButtonItem->setToolTip(tr("Reply to this status"));

    m_retweetButtonItem = new PixmapButtonItem(m_gradRectItem);
    m_retweetButtonItem->setDefaultPixmap(QPixmap(":/images/button_retweet.png"));
    m_retweetButtonItem->setHoverPixmap(QPixmap(":/images/button_retweet_hover.png"));
    m_retweetButtonItem->setClickedPixmap(QPixmap(":/images/button_retweet_click.png"));
    m_retweetButtonItem->setPos(43, 80);
    m_retweetButtonItem->setToolTip(tr("Retweet this status"));

    m_favoriteButtonItem = new PixmapButtonItem(m_gradRectItem);
    m_favoriteButtonItem->setDefaultPixmap(QPixmap(":/images/button_favorited.png"));
    m_favoriteButtonItem->setHoverPixmap(QPixmap(":/images/button_favorited_hover.png"));
    m_favoriteButtonItem->setClickedPixmap(QPixmap(":/images/button_favorited_click.png"));
    m_favoriteButtonItem->setToolTip(tr("Favorite this status"));

    m_lineItem = new QGraphicsLineItem(m_gradRectItem);
    m_lineItem->setPen(QPen(QColor("#DDDDDD")));
}

void TweetViewItem::itemChanged(const QList<QByteArray> &roles)
{
    //id, text, screenName, avatarUrl
    const QHash<QByteArray, QVariant> hash = data(QList<QByteArray>() << "text" << "screenName" << "avatarUrl" );

    const QString text = hash.value("text").toString();
    const QString name = hash.value("screenName").toString();
    const QString avatarUrl = hash.value("avatarUrl").toString();

    m_textItem->setHtml(text);
    m_nameItem->setPlainText(name);
    m_avatarItem->setPixmapUrl(avatarUrl);
}
