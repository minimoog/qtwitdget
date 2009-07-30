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
#include "netpixmapitem.h"

QString NetPixmapItem::dirImagesPath;

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

NetPixmapItem::NetPixmapItem(QGraphicsItem * parent)
:   QGraphicsPixmapItem(parent), m_netManager(0)
{

}

NetPixmapItem::NetPixmapItem(QNetworkAccessManager * netManager, QGraphicsItem * parent)
:   QGraphicsPixmapItem(parent), m_netManager(netManager)
{

}

void NetPixmapItem::setNetworkAccessManager(QNetworkAccessManager *netManager)
{
    m_netManager = netManager;
}

QNetworkAccessManager* NetPixmapItem::networkAccessManager() const
{
    return m_netManager;
}

void NetPixmapItem::setPixmapUrl(const QUrl &url)
{
    //check if pixmap is already cache
    QPixmap pm;

    if (!QPixmapCache::find(url.toString(), pm)) {  
        //no, try to load it from disk
        QString imageFilename = dirImagesPath + decodeTwitterImageUrlToFilename(url);
        if (!QFile::exists(imageFilename)) {
            //there is not image on the disk cache, download it
            Q_ASSERT(m_netManager != 0);
            QNetworkRequest request;
            request.setUrl(url);

            QNetworkReply *reply = m_netManager->get(request);
            connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
            connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
        } else {
            //load it from disk and cache it
            pm.load(imageFilename);
            QPixmapCache::insert(url.toString(), pm);
        }
    } else {
        setPixmap(pm);
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
            //save it to cache
            QPixmapCache::insert(reply->url().toString(), pm);

            //and to disk
            QString filename = decodeTwitterImageUrlToFilename(reply->url());
            if (filename.isEmpty())
                return;

            QString fullpath = dirImagesPath + filename;

            QFile file(fullpath);
            if (!file.open(QIODevice::WriteOnly)) {
                qDebug() << "Could not write";
            }

            file.write(buffer);
            file.close();
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

void NetPixmapItem::resolveDirImagesPath()
{
    if (dirImagesPath.isEmpty()) {
        QCoreApplication::setOrganizationName("QTwitdget");
        dirImagesPath = QDesktopServices::storageLocation(QDesktopServices::DataLocation);

        QDir dir;
        dir.setPath(dirImagesPath);
        if (!dir.exists()) {
            dir.mkpath(".");
        }

        dirImagesPath += '/';
    }
}