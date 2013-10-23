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
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include "namsingleton.h"

/**
 *   Constructor
 */
ShortenedUrl::ShortenedUrl(QObject *parent)
    : QObject(parent)
{
}

/**
 *  Shortens url
 *  @param url url to shorten
 */
void ShortenedUrl::shortUrl(const QString &url)
{
    //TODO: Add other services for shortening

    QUrlQuery urlQuery("http://api.bit.ly/v3/shorten");
    urlQuery.addQueryItem("login", "minimoog");
    urlQuery.addQueryItem("apiKey", "R_17b8dfb25efd629d31c8c8f93ea51b9e");
    urlQuery.addQueryItem("longUrl", url);
    urlQuery.addQueryItem("format", "txt");

    QUrl urlService;
    urlService.setQuery(urlQuery);

    QNetworkRequest req(urlService);

    QNetworkReply *reply = NAMSingleton::instance()->qnam()->get(req);
    reply->setProperty("longurl", QVariant(url));
    connect(reply, SIGNAL(finished()), this, SLOT(finished()));
}

void ShortenedUrl::finished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QString shortenedUrl(reply->readAll());
            QString longUrl = reply->property("longurl").toString();
            emit finishedShortingUrl(shortenedUrl, longUrl);
        } else {
            QString longUrl = reply->property("longurl").toString();
            emit finishedShortingUrl(QString(), longUrl);
        }
        reply->deleteLater();
    }
}
