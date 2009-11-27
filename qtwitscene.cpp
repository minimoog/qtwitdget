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

////TODO: QMap with SceneItems and status id

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

void QTwitScene::setUserid(int id)
{
	m_userid = id;
}

SceneItems QTwitScene::createStatusSceneItem(int count)
{
	SceneItems scit;

	GradientRectItem *rectItem;

	switch (count % 3) {
		case 0:
			rectItem = new GradientRectItem(200, GradientRectItem::Grey);
			break;
		case 1:
			rectItem = new GradientRectItem(200, GradientRectItem::Pink);
			break;
		case 2:
			rectItem = new GradientRectItem(200, GradientRectItem::Blue);
			break;
	}

	scit.gradRectItem = rectItem;

	QGraphicsPixmapItem *avatarBoxItem = new QGraphicsPixmapItem(QPixmap(":/images/avatar_box.png"), rectItem);
    avatarBoxItem->setPos(7, 7);

	QGraphicsRectItem *whiteBorderItem = new QGraphicsRectItem(0, 0, 49, 49, avatarBoxItem);
    whiteBorderItem->setPen(QPen(Qt::white));
    whiteBorderItem->setBrush(QBrush(Qt::NoBrush));
    whiteBorderItem->setPos(10, 10);

	NetPixmapItem *avatarItem = new NetPixmapItem(m_netManager, whiteBorderItem);
    avatarItem->setPos(1, 1);
	scit.avatarItem = avatarItem;

    QGraphicsTextItem *nameItem = new QGraphicsTextItem(rectItem);
    nameItem->setDefaultTextColor(QColor("#018ad9"));
    nameItem->setFont(QFont("Segoe UI", 11));
    nameItem->setPos(84, 0);
	scit.nameItem = nameItem;

    StatusTextItem *textItem = new StatusTextItem(rectItem);
    textItem->setPos(84, 24);
	scit.textItem = textItem;

    PixmapButtonItem *replyItem = new PixmapButtonItem(rectItem);
	replyItem->setDefaultPixmap(QPixmap(":/images/button_reply.png"));
	replyItem->setHoverPixmap(QPixmap(":/images/button_reply_hover.png"));
	replyItem->setClickedPixmap(QPixmap(":/images/button_reply_click.png"));
    replyItem->setPos(10, 80);
	scit.replyButtonItem = replyItem;

    PixmapButtonItem *retweetItem = new PixmapButtonItem(rectItem);
	retweetItem->setDefaultPixmap(QPixmap(":/images/button_retweet.png"));
	retweetItem->setHoverPixmap(QPixmap(":/images/button_retweet_hover.png"));
	retweetItem->setClickedPixmap(QPixmap(":/images/button_retweet_click.png"));
    retweetItem->setPos(43, 80);
    scit.retweetItem = retweetItem;

    PixmapButtonItem *favoritedItem = new PixmapButtonItem(rectItem);
	favoritedItem->setDefaultPixmap(QPixmap(":/images/button_favorited.png"));
	favoritedItem->setHoverPixmap(QPixmap(":/images/button_favorited_hover.png"));
	favoritedItem->setClickedPixmap(QPixmap(":/images/button_favorited_click.png"));
	scit.favoritedItem = favoritedItem;

    QGraphicsLineItem *lineItem = new QGraphicsLineItem(rectItem);
    lineItem->setPen(QPen(QColor("#DDDDDD")));
	scit.lineItem = lineItem;

    return scit;
}

qint64 QTwitScene::addStatuses(const QList<QTwitStatus>& statuses)
{
    QMapIterator<qint64, SceneItems> i(m_sceneItems);
    while (i.hasNext()) {
        i.next();
        i.value().gradRectItem->moveBy(0, 101.0f * statuses.size());
    }

    //we need viewport width
    QList<QGraphicsView*> graphicsViews = views();
    QGraphicsView* twitView = graphicsViews.at(0);

    int width = twitView->viewport()->width();

    //prepend new statuses
    for (int i = 0; i < statuses.size(); ++i) {
        SceneItems scit = createStatusSceneItem(m_sceneItems.count());

        addItem(scit.gradRectItem);
        scit.gradRectItem->setPos(0, 101.0f * i);
        scit.avatarItem->setPixmapUrl(QUrl(statuses.at(i).profileImageUrl()));
        scit.nameItem->setPlainText(statuses.at(i).screenName());
        scit.textItem->setHtml(replaceLinksWithHref(statuses.at(i).text()));

        scit.replyButtonItem->setId(statuses.at(i).id());
        scit.retweetItem->setId(statuses.at(i).id());
        scit.favoritedItem->setId(statuses.at(i).id());

        connect(scit.retweetItem, SIGNAL(clicked(qint64)), this, SLOT(retweetClicked(qint64)));
        connect(scit.favoritedItem, SIGNAL(clicked(qint64)), this, SIGNAL(requestFavorited(qint64)));

        if (statuses.at(i).userId() == m_userid) {
            scit.replyButtonItem->setDefaultPixmap(QPixmap(":/images/button_delete.png"));
            scit.replyButtonItem->setHoverPixmap(QPixmap(":/images/button_delete_hover.png"));
            scit.replyButtonItem->setClickedPixmap(QPixmap(":/images/button_delete_click.png"));
            connect(scit.replyButtonItem, SIGNAL(clicked(qint64)), this, SIGNAL(requestDelete(qint64)));
        } else {
            connect(scit.replyButtonItem, SIGNAL(clicked(qint64)), this, SLOT(replyClicked(qint64)));
        }

        if (statuses.at(i).favorited()) {
            scit.favoritedItem->setDefaultPixmap(QPixmap(":/images/button_unfavorited.png"));
            scit.favoritedItem->setHoverPixmap(QPixmap(":/images/button_unfavorited_hover.png"));
            scit.favoritedItem->setClickedPixmap(QPixmap(":/images/button_unfavorited_click.png"));
        }

        scit.statusText = statuses.at(i).text();
        scit.screenName = statuses.at(i).screenName();

        resizeItem(width, scit);
        m_sceneItems.insert(statuses.at(i).id(), scit);
    }

    //remove surplus statutes
    if (m_sceneItems.count() > 50) {    //50 should be global setting
        int nRemove = m_sceneItems.count() - 50;

        QMutableMapIterator<qint64, SceneItems> i(m_sceneItems);
        int removedItems = 0;

        while (i.hasNext() && (removedItems != nRemove)) {
            i.next();
            SceneItems scit = i.value();
            removeItem(scit.gradRectItem);
            delete scit.gradRectItem;
            i.remove();
            ++removedItems;
        }
    }

    setSceneRect(0, 0, width, boundingHeight());

    return m_sceneItems.begin().key();
}

qint64 QTwitScene::appendStatuses(const QList<QTwitStatus>& statuses)
{
    float appendPos = 101.0f * m_sceneItems.count();
    
    //we need viewport width
    QList<QGraphicsView*> graphicsViews = views();
    QGraphicsView* twitView = graphicsViews.at(0);
    int width = twitView->viewport()->width();

    //append new statuses
    for (int i = 0; i < statuses.size(); ++i) {
        SceneItems scit = createStatusSceneItem(m_sceneItems.count());

        addItem(scit.gradRectItem);
        scit.gradRectItem->setPos(0, appendPos + 101.0f * i);
        scit.avatarItem->setPixmapUrl(QUrl(statuses.at(i).profileImageUrl()));
        scit.nameItem->setPlainText(statuses.at(i).screenName());
        scit.textItem->setHtml(replaceLinksWithHref(statuses.at(i).text()));

        scit.replyButtonItem->setId(statuses.at(i).id());
        scit.retweetItem->setId(statuses.at(i).id());
        scit.favoritedItem->setId(statuses.at(i).id());

        connect(scit.retweetItem, SIGNAL(clicked(qint64)), this, SLOT(retweetClicked(qint64)));
        connect(scit.favoritedItem, SIGNAL(clicked(qint64)), this, SIGNAL(requestFavorited(qint64)));

        if (statuses.at(i).userId() == m_userid) {
            scit.replyButtonItem->setDefaultPixmap(QPixmap(":/images/button_delete.png"));
            scit.replyButtonItem->setHoverPixmap(QPixmap(":/images/button_delete_hover.png"));
            scit.replyButtonItem->setClickedPixmap(QPixmap(":/images/button_delete_click.png"));
            connect(scit.replyButtonItem, SIGNAL(clicked(qint64)), this, SIGNAL(requestDelete(qint64)));
        } else {
            connect(scit.replyButtonItem, SIGNAL(clicked(qint64)), this, SIGNAL(replyClicked(qint64)));
        }

        if (statuses.at(i).favorited()) {
            scit.favoritedItem->setDefaultPixmap(QPixmap(":/images/button_unfavorited.png"));
            scit.favoritedItem->setHoverPixmap(QPixmap(":/images/button_unfavorited_hover.png"));
            scit.favoritedItem->setClickedPixmap(QPixmap(":/images/button_unfavorited_click.png"));
        }

        scit.statusText = statuses.at(i).text();
        scit.screenName = statuses.at(i).screenName();

        resizeItem(width, scit);
        m_sceneItems.insert(statuses.at(i).id(), scit);
    }

    setSceneRect(0, 0, width, boundingHeight()); 

    return m_sceneItems.begin().key();
}

void QTwitScene::resizeItems(int w)
{
	QMapIterator<qint64, SceneItems> iterSceneItems(m_sceneItems);
    while (iterSceneItems.hasNext()) {
		iterSceneItems.next();
        SceneItems scit = iterSceneItems.value();

		scit.gradRectItem->setWidth(w);
		scit.textItem->setTextWidth(w - 84 - 10);
		scit.favoritedItem->setPos(w - 50, 80);
		scit.lineItem->setLine(1, 99, w - 1, 99);
    }
}

void QTwitScene::resizeItem(int w, SceneItems& sceneItems)
{
    sceneItems.gradRectItem->setWidth(w);
    sceneItems.textItem->setTextWidth(w - 84 - 10);
    sceneItems.favoritedItem->setPos(w - 50, 80);
    sceneItems.lineItem->setLine(1, 99, w - 1, 99);
}

float QTwitScene::boundingHeight() const
{
	return 101.0f * m_sceneItems.count();
}

void QTwitScene::replyClicked(qint64 id)
{
    emit requestReply(id, m_sceneItems.value(id).screenName);
}

void QTwitScene::retweetClicked(qint64 id)
{
    SceneItems scit = m_sceneItems.value(id);
    emit requestRetweet(scit.statusText, scit.screenName);
}
