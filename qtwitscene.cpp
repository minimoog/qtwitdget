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
 * Contact e-mail: Antonie Jovanoski <minimoog77@gmail.com>
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
	if(m_statuses.count() > m_textItems.count()){
		int numCreated = m_statuses.count() - m_textItems.count();
		float posY = 50.0f * m_textItems.count();

		for(int i = 0; i < numCreated; ++i){
			QGraphicsPixmapItem* pixmapItem = addPixmap(QPixmap());
			pixmapItem->setPos(0, posY);
			m_pixmapItems << pixmapItem;

			QGraphicsRectItem* rectItem = new QGraphicsRectItem;
			addItem(rectItem);
			rectItem->setPos(48, posY);
			rectItem->setRect(0, 0, 0, 50);
			rectItem->setPen(QPen(Qt::NoPen));
			m_rectItems << rectItem;

			if(m_rectItems.count() % 2)
				rectItem->setBrush(QBrush(QColor("#F2E3C6")));
			else
				rectItem->setBrush(QBrush(QColor("#E0E4CC")));

			QGraphicsTextItem* textItem = new QGraphicsTextItem(rectItem);
			textItem->setOpenExternalLinks(true);
			textItem->setTextInteractionFlags(Qt::TextBrowserInteraction);
			m_textItems << textItem;

			posY += 50.0f;
		}
	}

	//check
	if(m_statuses.count() < m_textItems.count())
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

	QListIterator<QTwitStatus> iterStatus(m_statuses);
	QListIterator<QGraphicsPixmapItem*> iterPixmapItem(m_pixmapItems);
	QListIterator<QGraphicsTextItem*> iterTextItem(m_textItems);

	while(iterStatus.hasNext()){
		QTwitStatus ts = iterStatus.next();
		QGraphicsPixmapItem* pixmapItem = iterPixmapItem.next();
		QGraphicsTextItem* textItem = iterTextItem.next();

		QImage img = images.value(ts.profileImageUrl());

		pixmapItem->setPixmap(QPixmap::fromImage(img));
		textItem->setHtml(replaceLinksWithHref(ts.text()));
	}

	QList<QGraphicsView*> graphicsViews = views();
	QGraphicsView* twitView = graphicsViews.at(0);
	
	setSceneRect(0, 0, twitView->viewport()->width(), boundingWidth());
	resizeItems(twitView->viewport()->width() - 48 - 1);
}

void QTwitScene::resizeItems(int w)
{
	QListIterator<QGraphicsTextItem*> iterTextItem(m_textItems);
	while(iterTextItem.hasNext()){
		QGraphicsTextItem* textItem = iterTextItem.next();
		textItem->setTextWidth(w);
	}

	QListIterator<QGraphicsRectItem*> iterRectItem(m_rectItems);
	while(iterRectItem.hasNext()){
		QGraphicsRectItem* rectItem = iterRectItem.next();
		rectItem->setRect(0, 0, w, 48);
	}
}

float QTwitScene::boundingWidth() const
{
	return 50.0f * m_textItems.count();
}