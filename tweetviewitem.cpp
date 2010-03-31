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

/*!
    Replaces parsed url with href
    \param text
    \return text with replaced url/links
 */
static QString replaceLinksWithHref(const QString &text)
{
    QRegExp rx("\\(?\\bhttp://[-A-Za-z0-9+&@#/%?=~_()|!:,.;]*[-A-Za-z0-9+&@#/%=~_()|]", Qt::CaseInsensitive);

    //make a copy
    QString textWithHref(text);

    //replace url links with href
    int pos = 0;
    while(pos >= 0){
        pos = rx.indexIn(textWithHref, pos);

        if(pos >= 0){
            int length = rx.matchedLength();
            QString matchedUrl = textWithHref.mid(pos, length);
            QString href = "<a href=\"" + matchedUrl + "\">" + matchedUrl + "</a>";
            textWithHref.replace(pos, length, href);
            int advance = href.size() - matchedUrl.size(); //how much to advanced after replacement
            pos += (length + advance);
        }
    }

    return textWithHref;
}

/*!
    Constructor
    \param index Index of the item in the model
    \param view View
 */
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
    d->replyItem->setPos(10, 80);
    connect(d->replyItem, SIGNAL(clicked(qint64)), this, SLOT(replyDeleteButtonClicked()));

    d->retweetItem = new PixmapButtonItem(d->gradRectItem);
    d->retweetItem->setDefaultPixmap(QPixmap(":/images/button_retweet.png"));
    d->retweetItem->setHoverPixmap(QPixmap(":/images/button_retweet_hover.png"));
    d->retweetItem->setClickedPixmap(QPixmap(":/images/button_retweet_click.png"));
    d->retweetItem->setPos(43, 80);
    d->retweetItem->setToolTip(QObject::tr("Retweet this status"));
    connect(d->retweetItem, SIGNAL(clicked(qint64)), this, SLOT(retweetButtonClicked()));

    d->favoritedItem = new PixmapButtonItem(d->gradRectItem);
    d->favoritedItem->setDefaultPixmap(QPixmap(":/images/button_favorited.png"));
    d->favoritedItem->setHoverPixmap(QPixmap(":/images/button_favorited_hover.png"));
    d->favoritedItem->setClickedPixmap(QPixmap(":/images/button_favorited_click.png"));
    d->favoritedItem->setToolTip(QObject::tr("Favorite this status"));
    connect(d->favoritedItem, SIGNAL(clicked(qint64)), this, SLOT(favoritedButtonClicked()));

    d->lineItem = new QGraphicsLineItem(d->gradRectItem);
    d->lineItem->setPen(QPen(QColor("#DDDDDD")));

    setData();

    //connect model to button signals
    // ### TODO: SignalMapper
    TweetListModel *model = d->listView->model();
    connect(this, SIGNAL(replyDeleteClicked(int)), model, SLOT(replyDeleteClicked(int)));
    connect(this, SIGNAL(retweetClicked(int)), model, SLOT(retweetClicked(int)));
    connect(this, SIGNAL(favoritedClicked(int)), model, SLOT(favoritedClicked(int)));
}

/*!
    \returns index of the item
 */
int TweetViewItem::index() const
{
    return d->index;
}

/*!
    \param index Sets index of the item
 */
void TweetViewItem::setIndex(int index)
{
    d->index = index;
    itemChanged();
}

/*!
    \returns width of the item
 */
int TweetViewItem::width() const
{
    return d->width;
}

/*!
    Sets the width of the item
    \param w width
 */
void TweetViewItem::setWidth(int w)
{
    d->width = w;

    d->gradRectItem->setWidth(w);
    d->textItem->setTextWidth(w - 84 -10);
    d->favoritedItem->setPos(w - 50, 80);
    d->lineItem->setLine(1, 99, w - 1, 99);
}

/*!
    \param roles Roles in with item changed
 */
void TweetViewItem::itemChanged(const QList<QByteArray> &roles)
{
    QHash<QByteArray, QVariant> hash = d->listView->model()->data(d->index, roles);

    for (int i = 0; i < roles.count(); ++i) {
        if (roles.at(i) == "isRead") {
            bool isRead = hash.value("isRead", QVariant(false)).toBool();

            if (isRead)
                d->gradRectItem->setGradient(GradientRectItem::White);
        }
    }
}

/*!
    \returns data of the item
 */
QVariant TweetViewItem::data() const
{
    return QVariant();
}

/*!
    \returns List view
 */
TweetListView* TweetViewItem::view() const
{
    return d->listView;
}

/*!
    \returns items geometry
 */
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
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void TweetViewItem::setData()
{
    QTwitStatus s = d->listView->model()->data(d->index).value<QTwitStatus>();
    d->avatarItem->setPixmapUrl(QUrl(s.profileImageUrl()));
    d->nameItem->setPlainText(s.screenName());
    d->textItem->setHtml(replaceLinksWithHref(s.text()));

    if (s.userId() == view()->model()->userid()) {
        d->replyItem->setDefaultPixmap(QPixmap(":/images/button_delete.png"));
        d->replyItem->setHoverPixmap(QPixmap(":/images/button_delete_hover.png"));
        d->replyItem->setClickedPixmap(QPixmap(":/images/button_delete_click.png"));
        d->replyItem->setToolTip(QObject::tr("Delete this status"));
    } else {
        d->replyItem->setDefaultPixmap(QPixmap(":/images/button_reply.png"));
        d->replyItem->setHoverPixmap(QPixmap(":/images/button_reply_hover.png"));
        d->replyItem->setClickedPixmap(QPixmap(":/images/button_reply_click.png"));
        d->replyItem->setToolTip(QObject::tr("Reply to this status"));
    }

    if (s.isRead())
        d->gradRectItem->setGradient(GradientRectItem::White);
    else
        d->gradRectItem->setGradient(GradientRectItem::Blue);
}

void TweetViewItem::replyDeleteButtonClicked()
{
    emit replyDeleteClicked(d->index);
}

void TweetViewItem::retweetButtonClicked()
{
    emit retweetClicked(d->index);
}

void TweetViewItem::favoritedButtonClicked()
{
    emit favoritedClicked(d->index);
}
