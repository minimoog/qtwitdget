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
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>
#include <QNetworkAccessManager>
#include "shortenedurl.h"
#include "mainwindow.h"

/*!
    Constructor
    \param parent QObject parent
    \remarks Uses global network access manager from MainWindow class
 */
ShortenedUrl::ShortenedUrl(QObject *parent)
    : QObject(parent)
{
    //m_netManager = MainWindow::networkAccessManager();
}

/*!
    Constructor
    \param netManager Network Access Manager
    \param parent Object parent
 */
ShortenedUrl::ShortenedUrl(QNetworkAccessManager *netManager, QObject *parent)
    : QObject(parent)
{
    m_netManager = netManager;
}

/*!
   Shortens url
   \param url Url to shorten
   \return Shorten url
 */
QString ShortenedUrl::shortUrl(const QString &url)
{
	//TODO: Add other services for shortening
    QUrl urlService("http://api.bit.ly/v3/shorten");
    urlService.addQueryItem("login", "minimoog");
    urlService.addQueryItem("apiKey", "R_17b8dfb25efd629d31c8c8f93ea51b9e");
    urlService.addQueryItem("longUrl", url);
    urlService.addQueryItem("format", "txt");

    //QNetworkRequest req(urlService);
    //QNetworkReply *reply = m_netManager->get(req);

//    SignalWaiter signalWaiter(reply, SIGNAL(finished()));

//    if (signalWaiter.wait(2000)) {
//        if (reply->error() != QNetworkReply::NoError) {
//            qDebug() << "Error shortening url";
//            qDebug() << reply->readAll();

//            return QString();
//        }

//        return reply->readAll();
//    } else {
//        qDebug() << "Timeout";
//        return QString();
//    }

    return url;
}
