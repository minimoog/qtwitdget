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

#ifndef QTWITSCENE_H
#define QTWITSCENE_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include "sceneitems/pixmapbuttonitem.h"
#include "sceneitems/gradientrectitem.h"
#include "sceneitems/netpixmapitem.h"
#include "sceneitems/statustextitem.h"

class QTwitStatus;

struct GroupItems
{
	GroupItems() : gradRectItem(0), avatarItem(0), nameItem(0), 
		textItem(0), favoritedItem(0), replyButtonItem(0), lineItem(0) {}

	GradientRectItem*	gradRectItem;       
	NetPixmapItem*		avatarItem;
	QGraphicsTextItem*	nameItem;
	StatusTextItem*		textItem;
    PixmapButtonItem*	replyButtonItem;
    PixmapButtonItem*   retweetItem;
	PixmapButtonItem*	favoritedItem;
	QGraphicsLineItem*	lineItem;
    QString             statusText;
    QString             screenName;
};

/*! class QTwitdget */
class QTwitScene : public QGraphicsScene
{
	Q_OBJECT
	
public:
	/*! Constructor */
	QTwitScene(QObject *parent = 0);
	/*! Sets network access manager */
    void setNetworkAccessManager(QNetworkAccessManager * netManager);
    /*! adds new statuses (prepend) */
    /*! \return oldest status id on the scene */
    qint64 addStatuses(const QList<QTwitStatus>& statuses);
    /*! appends statuses */
    /*! \return oldest status id on the scene */
    qint64 appendStatuses(const QList<QTwitStatus>& statuses);
    /*! removes status from scene */
    bool removeStatus(qint64 id);
	/*! sets user id */
	void setUserid(int id);
	void resizeItems(int w);
	float boundingHeight() const;

signals:
	void requestReply(qint64 statusId, const QString& screenName);
	void requestRetweet(const QString& text, const QString& screenName);
	void requestFavorited(qint64 statusId);
    void requestDelete(qint64 statusId);

private slots:
	void replyClicked(qint64 i);
	void retweetClicked(qint64 i);
    void deleteClicked(qint64 i);
    void favoritedClicked(qint64 i);
	
private:
	GroupItems createStatusSceneItem(int count);
    void resizeItem(int w, GroupItems& sceneItems);

	int m_userid; //user id
    int m_numPages; //number of pages

    QNetworkAccessManager * m_netManager;

    QMap<qint64, GroupItems> m_sceneItems;
};

#endif //QTWITSCENE_H