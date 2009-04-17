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

QTwitdget::QTwitdget(QWidget *parent)
	:	QWidget(parent), m_graphicsScene(new QGraphicsScene(this))
{
	ui.setupUi(this);
	ui.graphicsView->setScene(m_graphicsScene);
	m_graphicsScene->setSceneRect(0, 0, width(), height());

	QScrollBar *vertScrollBar = ui.graphicsView->verticalScrollBar();
	connect(vertScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollbarPos(int)));
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
	QList<QUrl> urlsImages;

	QListIterator<QTwitStatus> iterStatus(m_statuses);
	while(iterStatus.hasNext()){
		QTwitStatus ts = iterStatus.next();
		urlsImages << ts.profileImageUrl();
	}

	if(!urlsImages.isEmpty())
		if(m_imageDownloader)
			m_imageDownloader->downloadImages(urlsImages);
}

void QTwitdget::finishedDownloadImages()
{
	//delete text items
	QListIterator<QGraphicsTextItem*> iterTextItems(m_textItems);
	while(iterTextItems.hasNext()){
		QGraphicsTextItem *textItem = iterTextItems.next();
		m_graphicsScene->removeItem(textItem);
		delete textItem;
	}
	m_textItems.clear();

	//delete rest of the items
	QList<QGraphicsItem*> items = m_graphicsScene->items();
	QListIterator<QGraphicsItem*> iterItems(items);
	while(iterItems.hasNext()){
		QGraphicsItem *it = iterItems.next();
		m_graphicsScene->removeItem(it);
		delete it;
	}

	QHash<QString, QImage> images = m_imageDownloader->getImages();

	QListIterator<QTwitStatus> iterStatus(m_statuses);

	float posY = 0.0f;
	while(iterStatus.hasNext()){
		QTwitStatus ts = iterStatus.next();

		QImage img = images.value(ts.profileImageUrl());

		QGraphicsPixmapItem* pixmapItem = m_graphicsScene->addPixmap(QPixmap::fromImage(img));
		pixmapItem->setPos(0, posY);

		QGraphicsTextItem* textItem = m_graphicsScene->addText(ts.text());
		textItem->setOpenExternalLinks(true);
		textItem->setPos(50, posY);
		textItem->setTextInteractionFlags(Qt::TextBrowserInteraction);
		textItem->setTextWidth(ui.graphicsView->viewport()->width() - 50);
		m_textItems << textItem;

		posY += 50.0f;
	}

	//force resize
	QResizeEvent *resizeEvent = new QResizeEvent(size(), size());
	QCoreApplication::postEvent(this, resizeEvent);
}

void QTwitdget::scrollbarPos(int value)
{
	QScrollBar *vertScrollbar = ui.graphicsView->verticalScrollBar();
	if(value == vertScrollbar->maximum())
		emit scrollBarMaxPos();
}

void QTwitdget::changeEvent(QEvent *e)
{
	if(e->type() == QEvent::LanguageChange)
		ui.retranslateUi(this);

	QWidget::changeEvent(e);
}

void QTwitdget::resizeEvent(QResizeEvent *)
{
	QRectF boundingRect = m_graphicsScene->itemsBoundingRect();
	qreal itemsHeight = boundingRect.height();
	int viewportWidth = ui.graphicsView->viewport()->width();
	m_graphicsScene->setSceneRect(0, 0, viewportWidth, itemsHeight);

	QListIterator<QGraphicsTextItem*> iterTextItems(m_textItems);
	while(iterTextItems.hasNext()){
		QGraphicsTextItem *textItem = iterTextItems.next();
		textItem->setTextWidth(ui.graphicsView->viewport()->width() - 50);
	}
}