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
 
#ifndef NETPIXMAPITEM_H
#define NETPIXMAPITEM_H

#include <QGraphicsPixmapItem>
#include <QNetworkAccessManager>
#include <QUrl>

/*! Network Pixmap (remote url) QGraphics Pixmap Item with memory/disk caching */
class NetPixmapItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    /*! Constructor */
	NetPixmapItem(QGraphicsItem * parent = 0);
    /*! Constructor */
    NetPixmapItem(QNetworkAccessManager * netManager, QGraphicsItem * parent = 0);

    /*! Sets network access manager */
    void setNetworkAccessManager(QNetworkAccessManager * netManager);
    /*! Gets network access maanger */
    QNetworkAccessManager* networkAccessManager() const;

    /*! Sets url pixmap */
    void setPixmapUrl(const QUrl& url);

private slots:
    void downloadFinished();
    void error();

private:
    QNetworkAccessManager * m_netManager;
    
};

#endif //NETPIXMAPITEM_H