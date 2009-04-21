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
#include <QMessageBox>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QScrollBar>
#include <QResizeEvent>
#include "qtwit/qtwitstatus.h"
#include "qtwitdget.h"

QTwitdget::QTwitdget(ImageDownloader *imgDown, QObject *parent)
	:	QGraphicsScene(parent), m_imageDownloader(imgDown)
{
}

void QTwitdget::setImageDownloader(ImageDownloader *imgDown)
{	
	m_imageDownloader = imgDown;
	connect(m_imageDownloader, SIGNAL(finished()), SLOT(finishedDownloadImages()));
}

void QTwitdget::setStatuses(const QList<QTwitStatus>& statuses)
{
	m_statuses = statuses;

	updateStatusWidgets();
}

void QTwitdget::setUserid(int id)
{
	m_userid = id;
}

void QTwitdget::updateStatusWidgets()
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

			QGraphicsTextItem* textItem = addText(QString());
			textItem->setOpenExternalLinks(true);
			textItem->setPos(50, posY);
			textItem->setTextInteractionFlags(Qt::TextBrowserInteraction);
			//textItem->setTextWidth(ui.graphicsView->viewport()->width() - 50);
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

void QTwitdget::finishedDownloadImages()
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
		textItem->setHtml(ts.text());
	}

	//int viewportWidth = ui.graphicsView->viewport()->width();
	//m_graphicsScene->setSceneRect(0, 0, viewportWidth, 50.0f * m_textItems.count());
}
