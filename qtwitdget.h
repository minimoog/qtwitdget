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

#ifndef QTWITDGET_H
#define QTWITDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include "imagedownloader.h"

class QTwitStatus;

/*! class QTwitdget */
class QTwitdget : public QWidget
{
	Q_OBJECT
	
public:
	/*! Constructor */
	QTwitdget(QWidget *parent = 0);
	/*! Sets image downloader */
	/*! must be set to see images */
	void setImageDownloader(ImageDownloader *imgDown);
	/*! sets statuses */
	void setStatuses(const QList<QTwitStatus>& statuses);
	/*! sets user id */
	void setUserid(int id);

signals:
	/*! emitted when user clicks reply button of status widget */
	/*! \param replyText replying text */
	/*! \param replyToStatus Id of status in which is replying */
	void requestReplyStatus(const QString& replyText, int replyToStatus);
	/*! emitted when user click delete button of status widget */
	/*! \param statusId Id of status which should be deleted */
	void requestDeleteStatus(int statusId);
	void scrollBarMaxPos();

protected:
	void changeEvent(QEvent *e);
	void resizeEvent(QResizeEvent *e);
	
private slots:
	void finishedDownloadImages();
	void scrollbarPos(int value);

private:
	void updateStatusWidgets();

	int m_userid; //user id

	ImageDownloader *m_imageDownloader;

	QList<QTwitStatus> m_statuses;

	QGraphicsScene* m_graphicsScene;
	QList<QGraphicsTextItem*> m_textItems;
	QList<QGraphicsPixmapItem*> m_pixmapItems;
};

#endif //QTWITDGET_H