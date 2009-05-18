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
	:	QGraphicsScene(parent), m_imageDownloader(0)
{
}

void QTwitScene::setImageDownloader(ImageDownloader *imgDown)
{	
	m_imageDownloader = imgDown;
	connect(m_imageDownloader, SIGNAL(finished()), this, SLOT(finishedDownloadImages()));
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
	Q_ASSERT(m_imageDownloader != 0);

	QList<QUrl> urlsImages;

	//if number of statuses is bigger than text items create new 
	if(m_statuses.count() > m_gradRectItems.count()){
		int numCreated = m_statuses.count() - m_gradRectItems.count();
		float posY = 100.0f * m_gradRectItems.count();

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

			//QGraphicsPixmapItem* pixmapItem = addPixmap(QPixmap());
			//pixmapItem->setPos(0, posY);
			//m_pixmapItems << pixmapItem;

			//QGraphicsRectItem* rectItem = new QGraphicsRectItem;
			//addItem(rectItem);
			//rectItem->setPos(48, posY);
			//rectItem->setRect(0, 0, 0, 50);
			//rectItem->setPen(QPen(Qt::NoPen));
			//m_rectItems << rectItem;

			//if(m_rectItems.count() % 2)
				//rectItem->setBrush(QBrush(QColor("#F2E3C6")));
			//else
				//rectItem->setBrush(QBrush(QColor("#E0E4CC")));

			//QGraphicsTextItem* textItem = new QGraphicsTextItem(rectItem);
			//textItem->setOpenExternalLinks(true);
			//textItem->setTextInteractionFlags(Qt::TextBrowserInteraction);
			//m_textItems << textItem;

			//PixmapButtonItem *replyItem = new PixmapButtonItem(rectItem);
			//replyItem->setDefaultPixmap(QPixmap(":/images/button_reply.png"));
			//replyItem->setHoverPixmap(QPixmap(":/images/button_reply_hover.png"));
			//replyItem->setClickedPixmap(QPixmap(":/images/button_reply_click.png"));
			//replyItem->setIndex(m_textItems.count() - 1);
			//connect(replyItem, SIGNAL(clicked(int)), this, SLOT(replyClicked(int)));
			//m_replyItems << replyItem;

			//PixmapButtonItem *retweetItem = new PixmapButtonItem(rectItem);
			//retweetItem->setDefaultPixmap(QPixmap(":/images/button_retweet.png"));
			//retweetItem->setHoverPixmap(QPixmap(":/images/button_retweet_hover.png"));
			//retweetItem->setClickedPixmap(QPixmap(":/images/button_retweet_click.png"));
			//retweetItem->setIndex(m_textItems.count() - 1);
			//connect(retweetItem, SIGNAL(clicked(int)), this, SLOT(retweetClicked(int)));
			//m_retweetItems << retweetItem;

			//PixmapButtonItem *favoritedItem = new PixmapButtonItem(rectItem);
			//favoritedItem->setDefaultPixmap(QPixmap(":/images/button_favorited.png"));
			//favoritedItem->setHoverPixmap(QPixmap(":/images/button_favorited_hover.png"));
			//favoritedItem->setClickedPixmap(QPixmap(":/images/button_favorited_click.png"));
			//favoritedItem->setIndex(m_textItems.count() - 1);
			//connect(favoritedItem, SIGNAL(clicked(int)), this, SLOT(favoritedClicked(int)));
			//m_favoritedItems << favoritedItem;

			posY += 100.0f;
		}
	}

	//check
	if(m_statuses.count() < m_gradRectItems.count())
		qDebug() << "This shouldn't be";

	QListIterator<QTwitStatus> iterStatus(m_statuses);
	while(iterStatus.hasNext()){
		QTwitStatus ts = iterStatus.next();
		urlsImages << ts.profileImageUrl();
	}

	if(!urlsImages.isEmpty())
		m_imageDownloader->downloadImages(urlsImages);
}

void QTwitScene::finishedDownloadImages()
{
	QHash<QString, QImage> images = m_imageDownloader->getImages();

    /*
	QListIterator<QTwitStatus> iterStatus(m_statuses);
	QListIterator<QGraphicsPixmapItem*> iterPixmapItem(m_pixmapItems);
	QListIterator<QGraphicsTextItem*> iterTextItem(m_textItems);

	while(iterStatus.hasNext()){
		QTwitStatus ts = iterStatus.next();
		QGraphicsPixmapItem* pixmapItem = iterPixmapItem.next();
		QGraphicsTextItem* textItem = iterTextItem.next();

		QImage img = images.value(ts.profileImageUrl());

		pixmapItem->setPixmap(QPixmap::fromImage(img));
		QString nameHtml = QString("<b>%1: </b>").arg(ts.name());
		QString textHtml = replaceLinksWithHref(ts.text());
		textItem->setHtml(nameHtml + textHtml);
	}
    */

	QList<QGraphicsView*> graphicsViews = views();
	QGraphicsView* twitView = graphicsViews.at(0);
	
	setSceneRect(0, 0, twitView->viewport()->width(), boundingHeight());
	resizeItems(twitView->viewport()->width() - 1);
}

void QTwitScene::resizeItems(int w)
{
    QListIterator<GradientRectItem*> iterGradRectItem(m_gradRectItems);
    while (iterGradRectItem.hasNext()) {
        iterGradRectItem.next()->setWidth(w);
    }
    /*
	QListIterator<QGraphicsTextItem*> iterTextItem(m_textItems);
	while(iterTextItem.hasNext()){
		QGraphicsTextItem* textItem = iterTextItem.next();
		textItem->setTextWidth(w);
	}

	QListIterator<QGraphicsRectItem*> iterRectItem(m_rectItems);
	QListIterator<PixmapButtonItem*> iterReplyItem(m_replyItems);
	QListIterator<PixmapButtonItem*> iterRetweetItem(m_retweetItems);
	QListIterator<PixmapButtonItem*> iterFavoritedItem(m_favoritedItems);
	while(iterRectItem.hasNext()){
		QGraphicsRectItem* rectItem = iterRectItem.next();
		PixmapButtonItem* replyItem = iterReplyItem.next();
		PixmapButtonItem* retweetItem = iterRetweetItem.next();
		PixmapButtonItem* favoritedItem = iterFavoritedItem.next();
		rectItem->setRect(0, 0, w, 48);
		replyItem->setPos(w, 0);
		retweetItem->setPos(w, 16);
		favoritedItem->setPos(w, 32);
	}
    */
}

float QTwitScene::boundingHeight() const
{
	return 100.0f * m_gradRectItems.count();
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