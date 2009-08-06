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

/*
 * This work is licensed under the Creative Commons Attribution-Noncommercial-Share 
 * Alike 3.0 Unported License. To view a copy of this license, 
 * visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to 
 * Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 *
 * Graphic design by Goce Mitevski 
 */

#include <QtDebug>
#include <QScrollBar>
#include <QGraphicsView>
#include <QRegExp>
#include "qtwit/qtwitstatus.h"
#include "qtwitscene.h"

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

QTwitScene::QTwitScene(QObject *parent)
	:	QGraphicsScene(parent), m_netManager(0)
{
}

void QTwitScene::setNetworkAccessManager(QNetworkAccessManager * netManager)
{
    m_netManager = netManager;
}

void QTwitScene::setStatuses(const QList<QTwitStatus>& statuses)
{
	m_statuses = statuses;

	updateStatusWidgets();
}

void QTwitScene::setUserid(int id)
{
	m_userid = id;
}

void QTwitScene::updateStatusWidgets()
{
	QList<QUrl> urlsImages;

	//if number of statuses is bigger than text items create new 
	if(m_statuses.count() > m_gradRectItems.count()){
		int numCreated = m_statuses.count() - m_gradRectItems.count();
		float posY = 101.0f * m_gradRectItems.count();

		for(int i = 0; i < numCreated; ++i){
            GradientRectItem *rectItem = new GradientRectItem(200, GradientRectItem::Pink);
            addItem(rectItem);
            rectItem->setPos(0, posY);
            
            switch (m_gradRectItems.count() % 3) {
                case 0:
                    rectItem->setGradient(GradientRectItem::Grey);
                    break;
                case 1:
                    rectItem->setGradient(GradientRectItem::Pink);
                    break;
                case 2:
                    rectItem->setGradient(GradientRectItem::Blue);
                    break;
            }

            m_gradRectItems << rectItem;

            QGraphicsPixmapItem *avatarBoxItem = new QGraphicsPixmapItem(QPixmap(":/images/avatar_box.png"), rectItem);
            avatarBoxItem->setPos(7, 7);

            QGraphicsRectItem *whiteBorderItem = new QGraphicsRectItem(0, 0, 49, 49, avatarBoxItem);
            whiteBorderItem->setPen(QPen(Qt::white));
            whiteBorderItem->setBrush(QBrush(Qt::NoBrush));
            whiteBorderItem->setPos(10, 10);

            NetPixmapItem *avatarItem = new NetPixmapItem(m_netManager, whiteBorderItem);
            avatarItem->setPos(1, 1);
            m_avatarItems << avatarItem;

            QGraphicsTextItem *nameItem = new QGraphicsTextItem(rectItem);
            nameItem->setDefaultTextColor(QColor("#018ad9"));
            nameItem->setFont(QFont("Segoe UI", 11));
            nameItem->setPos(84, 0);
            m_nameItems << nameItem;

            QGraphicsTextItem *textItem = new QGraphicsTextItem(rectItem);
            textItem->setDefaultTextColor(QColor("#222222"));
            textItem->setFont(QFont("Segoe UI", 9));
            textItem->setPos(84, 24);
            textItem->setOpenExternalLinks(true);
            textItem->setTextInteractionFlags(Qt::TextBrowserInteraction);
            m_textItems << textItem;

			PixmapButtonItem *replyItem = new PixmapButtonItem(rectItem);
			replyItem->setDefaultPixmap(QPixmap(":/images/button_reply.png"));
			replyItem->setHoverPixmap(QPixmap(":/images/button_reply_hover.png"));
			replyItem->setClickedPixmap(QPixmap(":/images/button_reply_click.png"));
		    replyItem->setIndex(m_textItems.count() - 1);
            replyItem->setPos(10, 80);
			connect(replyItem, SIGNAL(clicked(int)), this, SLOT(replyClicked(int)));
            m_replyButtonItems << replyItem;

			PixmapButtonItem *retweetItem = new PixmapButtonItem(rectItem);
			retweetItem->setDefaultPixmap(QPixmap(":/images/button_retweet.png"));
			retweetItem->setHoverPixmap(QPixmap(":/images/button_retweet_hover.png"));
			retweetItem->setClickedPixmap(QPixmap(":/images/button_retweet_click.png"));
			retweetItem->setIndex(m_textItems.count() - 1);
            retweetItem->setPos(43, 80);
			connect(retweetItem, SIGNAL(clicked(int)), this, SLOT(retweetClicked(int)));

			PixmapButtonItem *favoritedItem = new PixmapButtonItem(rectItem);
			favoritedItem->setDefaultPixmap(QPixmap(":/images/button_favorited.png"));
			favoritedItem->setHoverPixmap(QPixmap(":/images/button_favorited_hover.png"));
			favoritedItem->setClickedPixmap(QPixmap(":/images/button_favorited_click.png"));
			favoritedItem->setIndex(m_textItems.count() - 1);
			connect(favoritedItem, SIGNAL(clicked(int)), this, SLOT(favoritedClicked(int)));
			m_favoritedItems << favoritedItem;

            QGraphicsLineItem *lineItem = new QGraphicsLineItem(rectItem);
            lineItem->setPen(QPen(QColor("#DDDDDD")));
            m_lineItems << lineItem;
            
			posY += 101.0f;
		}
	}

	//check
	if(m_statuses.count() < m_gradRectItems.count())
		qDebug() << "This shouldn't be";

    refreshStatutes();
}

void QTwitScene::refreshStatutes()
{
    QListIterator<NetPixmapItem*> iterAvatarItem(m_avatarItems);
    QListIterator<QGraphicsTextItem*> iterNameItem(m_nameItems);
    QListIterator<QTwitStatus> iterStatus(m_statuses);
    QListIterator<QGraphicsTextItem*> iterTextItem(m_textItems);
    QListIterator<PixmapButtonItem*> iterFavoritedItem(m_favoritedItems);
    QListIterator<PixmapButtonItem*> iterReplyButtonItem(m_replyButtonItems);
    while (iterStatus.hasNext()) {
        QTwitStatus ts = iterStatus.next();
        NetPixmapItem* avatarItem = iterAvatarItem.next();
        QGraphicsTextItem* nameItem = iterNameItem.next();
        PixmapButtonItem* favoritedItem = iterFavoritedItem.next();
        PixmapButtonItem* replyButtonItem = iterReplyButtonItem.next();

        avatarItem->setPixmapUrl(QUrl(ts.profileImageUrl()));
        nameItem->setPlainText(ts.screenName());

        QString textHtml = replaceLinksWithHref(ts.text());
        iterTextItem.next()->setHtml(textHtml);

        if (ts.userId() == m_userid) {
            replyButtonItem->setDefaultPixmap(QPixmap(":/images/button_delete.png"));
            replyButtonItem->setHoverPixmap(QPixmap(":/images/button_delete_hover.png"));
            replyButtonItem->setClickedPixmap(QPixmap(":/images/button_delete_click.png"));
        } else {
            replyButtonItem->setDefaultPixmap(QPixmap(":/images/button_reply.png"));
            replyButtonItem->setHoverPixmap(QPixmap(":/images/button_reply_hover.png"));
            replyButtonItem->setClickedPixmap(QPixmap(":/images/button_reply_click.png"));
        }

        if (ts.favorited()) {
            favoritedItem->setDefaultPixmap(QPixmap(":/images/button_unfavorited.png"));
            favoritedItem->setHoverPixmap(QPixmap(":/images/button_unfavorited_hover.png"));
            favoritedItem->setClickedPixmap(QPixmap(":/images/button_unfavorited_click.png"));
        } else {
            favoritedItem->setDefaultPixmap(QPixmap(":/images/button_favorited.png"));
            favoritedItem->setHoverPixmap(QPixmap(":/images/button_favorited_hover.png"));
            favoritedItem->setClickedPixmap(QPixmap(":/images/button_favorited_click.png"));
        }
    } 
   
	QList<QGraphicsView*> graphicsViews = views();
	QGraphicsView* twitView = graphicsViews.at(0);
	
	setSceneRect(0, 0, twitView->viewport()->width(), boundingHeight());
	resizeItems(twitView->viewport()->width() - 1);
}

void QTwitScene::resizeItems(int w)
{
    QListIterator<GradientRectItem*> iterGradRectItem(m_gradRectItems);
    QListIterator<QGraphicsTextItem*> iterTextItem(m_textItems);
    QListIterator<PixmapButtonItem*> iterFavoritedItem(m_favoritedItems);
    QListIterator<QGraphicsLineItem*> iterLineItem(m_lineItems);
    while (iterGradRectItem.hasNext()) {
        iterGradRectItem.next()->setWidth(w);
        iterTextItem.next()->setTextWidth(w - 84 - 10);
        iterFavoritedItem.next()->setPos(w - 50, 80);
        iterLineItem.next()->setLine(1, 99, w - 1, 99);
    }
}

float QTwitScene::boundingHeight() const
{
	return 101.0f * m_gradRectItems.count();
}

void QTwitScene::replyClicked(int i)
{
	emit requestReply(m_statuses.at(i).id(), m_statuses.at(i).screenName());
}

void QTwitScene::retweetClicked(int i)
{
	emit requestRetweet(m_statuses.at(i).text(), m_statuses.at(i).screenName());
}

void QTwitScene::favoritedClicked(int i)
{
	emit requestFavorited(m_statuses.at(i).id());
}

void QTwitScene::deleteClicked(int i)
{

}