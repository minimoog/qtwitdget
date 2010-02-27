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
#include <QtSql>
#include "sceneitems/pixmapbuttonitem.h"
#include "sceneitems/gradientrectitem.h"
#include "sceneitems/netpixmapitem.h"
#include "sceneitems/statustextitem.h"

class QTwitStatus;
class StatusItemGroup;

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
    /*! removes status from scene */
    bool removeStatus(qint64 id);
    /*! sets/unsets favorited/unfavorited for status with id on the scene */
    bool setFavorited(qint64 id, bool fav);
	/*! sets user id */
	void setUserid(int id);
    /*! returns true if status with id is on the scene */
    bool containsStatus(qint64 id);
    /*! returns scene pos with status id (gradRectItem pos) */
    QPointF statusScenePos(qint64 id);
    /*! resize items to width w */
	void resizeItems(int w);
    /*! returns bounding height */
	float boundingHeight() const;
    /*! refreshes statuses */
    virtual void updateStatuses();
    /*! adds next statuses */
    virtual void nextStatuses();
    /*! marks (different gradient) status with id read */
    void markRead(qint64 id);
    QString additionalQuery() const;
    void setAdditionalQuery(const QString& query);
    qint64 lastStatusId() const;
    void setLastStatusId(qint64 id);
    qint64 firstStatusId() const;
    void setFirstStatusId(qint64 id);
    /*! adds statuses to Scene, at y position with width width */
    void addToScene(const QList<QTwitStatus>& statuses, qreal ypos, int width);
    /*! adds new statuses */
    /*! if paging is true removes old statuses (50 per page) */
    /*! if paging is false doesn't remove old statuses */
    /*! \return oldest status id on the scene */
    qint64 addStatuses(const QList<QTwitStatus>& statuses, bool paging = true);
    /*! appends statuses */
    /*! \return oldest status id on the scene */
    qint64 appendStatuses(const QList<QTwitStatus>& statuses);
    /*! returns number of statuses on scene */
    int numStatuses() const;
    /*! mark all statuses read on the scene*/
    virtual void markAllRead();
    /*! removes all statuses on scene */
    void removeAll();
    /*! removes status with fade out */
    void removeStatusAnim(qint64 id);

signals:
	void requestReply(qint64 statusId, const QString& screenName);
	void requestRetweet(qint64 statusid);
	void requestFavorited(qint64 statusId);
    void requestDelete(qint64 statusId);

protected:

private slots:
	void replyClicked(qint64 i);
	void retweetClicked(qint64 i);
    void deleteClicked(qint64 i);
    void favoritedClicked(qint64 i);
    void finishedFadeOut();
    void testSlot(const QVariant& value);   //TESTTESTTEST
    void finishedMoveAnim();
	
private:
	GroupItems createStatusSceneItem(int count);
    void resizeItem(int w, GroupItems& sceneItems);

	int m_userid; //user id
    int m_numPages; //number of pages
    qint64 m_lastStatus; //newest status id(top of)
    qint64 m_firstStatus; //latest status id (bottom)
    QString m_additionalQuery;
    qint64 m_fadeoutId;

    QNetworkAccessManager * m_netManager;

    QMap<qint64, GroupItems> m_sceneItems;
    QPropertyAnimation *m_moveAnim;
    StatusItemGroup *m_tempGroup;
};

#endif //QTWITSCENE_H
