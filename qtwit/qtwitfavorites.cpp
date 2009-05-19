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
 
#include "qtwitfavorites.h"

static const QNetworkRequest::Attribute createFavoriteAttribute = static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User + 1);
static const QNetworkRequest::Attribute idAttribute = static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User + 2);


QTwitFavorites::QTwitFavorites(QObject *parent)
:	QTwitBase(parent)
{	
}

void QTwitFavorites::create(int id)
{
    QString urlString = QString("http://twitter.com/favorites/create/%1.xml").arg(id);

    QUrl url(urlString);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::POST);
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", oauthHeader);
    req.setAttribute(createFavoriteAttribute, true);
    req.setAttribute(idAttribute, id);

    QNetworkReply *netReply = networkAccessManager()->post(req, QByteArray());
    connect(netReply, SIGNAL(finished()), this, SLOT(reply()));
    connect(netReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTwitFavorites::destroy(int id)
{
    QString urlString = QString("http://twitter.com/favorites/destroy/%1.xml").arg(id);

    QUrl url(urlString);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::POST);
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", oauthHeader);
    req.setAttribute(createFavoriteAttribute, false);
    req.setAttribute(idAttribute, id);

    QNetworkReply *netReply = networkAccessManager()->post(req, QByteArray());
    connect(netReply, SIGNAL(finished()), this, SLOT(reply()));
    connect(netReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}