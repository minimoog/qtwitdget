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

class NetPixmapItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
	NetPixmapItem(QGraphicsItem * parent = 0);
    NetPixmapItem(QNetworkAccessManager * netManager, QGraphicsItem * parent = 0);

    void setNetworkAccessManager(QNetworkAccessManager * netManager);
    QNetworkAccessManager* networkAccessManager() const;

    void setPixmapUrl(const QUrl& url);

private slots:
    void downloadFinished();
    void error();

private:
    void resolveDirImagesPath();

    QNetworkAccessManager * m_netManager;
    static QString dirImagesPath;

};

#endif //NETPIXMAPITEM_H