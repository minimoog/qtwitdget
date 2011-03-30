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

#include "shortenedurl.h"
#include <QtDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

/**
 *   Constructor
 */
ShortenedUrl::ShortenedUrl(QObject *parent)
    : QObject(parent)
{
}

/**
 *   Constructor
 *   @param netManager network Access Manager
 *   @param parent parent
 */
ShortenedUrl::ShortenedUrl(QNetworkAccessManager *netManager, QObject *parent)
    : QObject(parent)
{
    m_netManager = netManager;
}

/**
 *  Sets network access manager
 */
void ShortenedUrl::setNetworkAccessManager(QNetworkAccessManager *netManager)
{
    m_netManager = netManager;
}

/**
 *  Shortens url
 *  @param url url to shorten
 */
void ShortenedUrl::shortUrl(const QString &url)
{
    //TODO: Add other services for shortening
    QUrl urlService("http://api.bit.ly/v3/shorten");
    urlService.addQueryItem("login", "minimoog");
    urlService.addQueryItem("apiKey", "R_17b8dfb25efd629d31c8c8f93ea51b9e");
    urlService.addQueryItem("longUrl", url);
    urlService.addQueryItem("format", "txt");

    QNetworkRequest req(urlService);
    req.setAttribute(QNetworkRequest::User, url);

    QNetworkReply *reply = m_netManager->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(finished()));
}

void ShortenedUrl::finished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply) {
        if (reply->error() != QNetworkReply::NoError) {
            QString shortenedUrl(reply->readAll());
            QString longUrl = reply->attribute(QNetworkRequest::User).toString();
            emit finishedShortingUrl(shortenedUrl, longUrl);
        } else {
            QString longUrl = reply->attribute(QNetworkRequest::User).toString();
            emit finishedShortingUrl(QString(), longUrl);
        }

        reply->deleteLater();
    }
}
