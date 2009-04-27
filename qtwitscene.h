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

#ifndef QTWITSCENE_H
#define QTWITSCENE_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include "imagedownloader.h"
#include "pixmapbuttonitem.h"

class QTwitStatus;

/*! class QTwitdget */
class QTwitScene : public QGraphicsScene
{
	Q_OBJECT
	
public:
	/*! Constructor */
	QTwitScene(QObject *parent = 0);
	/*! Sets image downloader */
	/*! must be set to see images */
	void setImageDownloader(ImageDownloader *imgDown);
	/*! sets statuses */
	void setStatuses(const QList<QTwitStatus>& statuses);
	/*! sets user id */
	void setUserid(int id);
	void resizeItems(int w);
	float boundingWidth() const;

signals:
	void requestReplyStatus(const QString& replyText, int replyToStatus);
	void requestDeleteStatus(int statusId);
	
private slots:
	void finishedDownloadImages();
	
private:
	void updateStatusWidgets();

	int m_userid; //user id

	ImageDownloader *m_imageDownloader;

	QList<QTwitStatus> m_statuses;

	QList<QGraphicsTextItem*> m_textItems;
	QList<QGraphicsPixmapItem*> m_pixmapItems;
	QList<QGraphicsRectItem*> m_rectItems;
	QList<PixmapButtonItem*> m_replyItems;
};

#endif //QTWITSCENE_H