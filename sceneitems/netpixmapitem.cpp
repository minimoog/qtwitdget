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
 
#include <QtDebug>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QPixmapCache>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtSql>
#include "netpixmapitem.h"

/*!
    Strips url path for pixmap/image url
    \return Unique string identifying pixmap from url
 */
static QString decodeTwitterImageUrlToFilename(const QUrl& url)
{
    QString wholePath = url.path();
    QFileInfo fileInfo(wholePath);
    QString basename = fileInfo.fileName();

    if(basename.isEmpty())
        return QString();

    QString path = fileInfo.path();

    if(path.isEmpty())
        return QString();

    //extract number from path
    int pos = path.lastIndexOf('/');
    path.remove(0, pos + 1);

    return path + '_' + basename;
}

/*!
    Constructor
    \param parent Graphics item parent
 */
NetPixmapItem::NetPixmapItem(QGraphicsItem * parent)
:   QGraphicsPixmapItem(parent), m_netManager(0)
{
}

/*!
    Constructor
    \param netManager Network Access Manager
    \param Graphics Item parent
 */
NetPixmapItem::NetPixmapItem(QNetworkAccessManager * netManager, QGraphicsItem * parent)
:   QGraphicsPixmapItem(parent), m_netManager(netManager)
{
}

/*!
    Sets network access manager
    \param netManager Network access manager
 */
void NetPixmapItem::setNetworkAccessManager(QNetworkAccessManager *netManager)
{
    m_netManager = netManager;
}

/*!
    Gets network access manager
    \return network access manager
 */
QNetworkAccessManager* NetPixmapItem::networkAccessManager() const
{
    return m_netManager;
}

/*!
    Sets pixmap url.

    First check if pixmap is in QPixmapCache.
    If it isn't then check if it is in the local database.
    If it isn't then downloads the pixmap

    \param url Url of the pixmap
 */
void NetPixmapItem::setPixmapUrl(const QUrl &url)
{
    //check if pixmap is already cache
    QPixmap pm;

    if (QPixmapCache::find(url.toString(), pm)) {  
        setPixmap(pm);
    } else {
        //no, try to load it from database
        QString imageName = decodeTwitterImageUrlToFilename(url);
		QString qs = QString("SELECT image FROM images WHERE imageName = '%1'").arg(imageName);

		QSqlQuery query;
		query.exec(qs);

		if (!query.next()) {
			//there is not image on the database, download it
            Q_ASSERT(m_netManager != 0);
            QNetworkRequest request;
            request.setUrl(url);
            
            QNetworkReply *reply = m_netManager->get(request);
            connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
            connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
        } else {
			QByteArray image = query.value(0).toByteArray();
			if (!pm.loadFromData(image)) {
				qDebug() << "Error load image from database";
				return;
			}
            QPixmapCache::insert(url.toString(), pm);
            setPixmap(pm);
        }
    }
}

void NetPixmapItem::downloadFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply) {
        QPixmap pm;

        QByteArray buffer = reply->readAll();

        reply->deleteLater();

        if (!pm.loadFromData(buffer)) {
            qDebug() << "Error load from data";
            return;
        } else {
            QPixmap pmScaled;

            //resize pixmap if size different than 48x48
            if (pm.width() > 48 || pm.height() > 48) {
                pmScaled = pm.scaled(48, 48, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            } else {
                pmScaled = pm;
            }

            setPixmap(pmScaled);

            //save it to cache
            QPixmapCache::insert(reply->url().toString(), pmScaled);

            //and to database
            QString imageName = decodeTwitterImageUrlToFilename(reply->url());
            if (imageName.isEmpty())
                return;
			
			QByteArray image;
			QBuffer buffer(&image);
			buffer.open(QIODevice::WriteOnly);
			pmScaled.save(&buffer, "PNG");
			
			QSqlQuery query;
			query.prepare("INSERT INTO images (imageName, image) VALUES (:imageName, :image)");
			query.bindValue(":imageName", imageName);
			query.bindValue(":image", image);

			query.exec();
        }
    }
}

void NetPixmapItem::error()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if(reply){
        QUrl url = reply->url();
        qDebug() << "error downloading" << url << reply->errorString();
    }
}
