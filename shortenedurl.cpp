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
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>
#include "shortenedurl.h"

ShortenedUrl::ShortenedUrl()
{
}

QString ShortenedUrl::shortUrl(const QString &url)
{
	//TODO: Add other services for shortening
	QUrl urlService("http://api.0.mk/");
	urlService.addQueryItem("dolg", url);

	QNetworkRequest req(urlService);
	
    QEventLoop eventloop;
    
    QTimer timer;
    timer.setSingleShot(true);

    connect(&timer, SIGNAL(timeout()), &eventloop, SLOT(quit()));
    connect(&m_netManager, SIGNAL(finished(QNetworkReply*)), &eventloop, SLOT(quit()));

    QNetworkReply *reply = m_netManager.get(req);

	timer.start(5000);
    eventloop.exec();

    if (timer.isActive()) {
        timer.stop();

        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Error shortening url";
            qDebug() << reply->readAll();

            return QString();
        }

        qDebug() << reply->readAll();
        return QString();
        //return reply->readAll();

    } else {
        qDebug() << "Timeout";
        return QString();
    }
}
