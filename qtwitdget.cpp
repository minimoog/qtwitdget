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
#include "qtwit/qtwitstatus.h"
#include "qtwitdget.h"

QTwitdget::QTwitdget(QWidget *parent)
	:	QWidget(parent)
{
	ui.setupUi(this);
	
	m_statusWidgetsPerPage = 20; //should be programmable

	//set StatusWidgets
	setupStatusWidgets();
}

void QTwitdget::setImageDownloader(ImageDownloader *imgDown)
{	
	m_imageDownloader = imgDown;
	connect(m_imageDownloader, SIGNAL(finished()), SLOT(finishedDownloadImages()));
}

void QTwitdget::setStatuses(const QList<QTwitStatus>& statuses)
{
	if(statuses.count() > m_statusWidgetsPerPage)
		qDebug() << "More status per page";

	m_statuses = statuses;

	updateStatusWidgets();
}

void QTwitdget::setUserid(int id)
{
	m_userid = id;
}

void QTwitdget::setupStatusWidgets()
{
	for(int i = 0; i < m_statusWidgetsPerPage; ++i){
		StatusWidget *sw = new StatusWidget(ui.scrollAreaWidgetContents);
		ui.verticalLayout->addWidget(sw);
		m_statusWidgets.append(sw);
		connect(sw, SIGNAL(replyRequest(const QString&, int)), SIGNAL(requestReplyStatus(const QString&, int)));
		connect(sw, SIGNAL(deleteRequest(int)), SIGNAL(requestDeleteStatus(int)));
	}

	ui.scrollAreaWidgetContents->adjustSize();
}

void QTwitdget::updateStatusWidgets()
{
	QList<QUrl> urlsImages;

	QListIterator<QTwitStatus> iterStatus(m_statuses);
	QListIterator<StatusWidget *> iterWidget(m_statusWidgets);
	while(iterWidget.hasNext()){
		StatusWidget* sw = iterWidget.next();

		if(iterStatus.hasNext()){
			QTwitStatus ts = iterStatus.next();
			sw->setCreated(ts.created());
			sw->setStatusId(ts.id());
			sw->setText(ts.text());
			sw->setScreenName(ts.screenName());
			sw->setSource(ts.source());
			sw->setEnabledReplyDeleteButton(true);
			urlsImages << ts.profileImageUrl();

			if(ts.userId() == m_userid){
				sw->setUserOwnStatus(true);
			} else {
				sw->setUserOwnStatus(false);
			}

		} else {
			sw->setCreated(QDateTime());
			sw->setStatusId(0);
			sw->setText(QString());
			sw->setScreenName(QString());
			sw->setSource(QString());
			sw->setProfileImagePixmap(QPixmap());
			sw->setUserOwnStatus(false);
			sw->setEnabledReplyDeleteButton(false);
		}
	}

	if(!urlsImages.isEmpty())
		if(m_imageDownloader)
			m_imageDownloader->downloadImages(urlsImages);
}

void QTwitdget::finishedDownloadImages()
{
	QHash<QString, QImage> images = m_imageDownloader->getImages();

	QListIterator<StatusWidget *> iterWidget(m_statusWidgets);
	QListIterator<QTwitStatus> iterStatus(m_statuses);

	while(iterStatus.hasNext()){
		QTwitStatus ts = iterStatus.next();
		StatusWidget *sw = iterWidget.next();
		QImage img = images.value(ts.profileImageUrl());
		sw->setProfileImagePixmap(QPixmap::fromImage(img));
	}
}

void QTwitdget::changeEvent(QEvent *e)
{
	if(e->type() == QEvent::LanguageChange)
		ui.retranslateUi(this);

	QWidget::changeEvent(e);
}
