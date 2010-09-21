/* Copyright (c) 2010, Antonie Jovanoski
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
#include "qtwitnewdirectmessage.h"

QTwitNewDirectMessage::QTwitNewDirectMessage(QObject *parent) :
    QTwitBase(parent)
{
}

QTwitNewDirectMessage::QTwitNewDirectMessage(QNetworkAccessManager *netManager, OAuthTwitter *oauthTwitter, QObject *parent)
    :   QTwitBase(netManager, oauthTwitter, parent)
{
}

void QTwitNewDirectMessage::sendMessage(qint64 userId, const QString &text)
{
    Q_ASSERT(networkAccessManager() != 0);

    QUrl url("http://api.twitter.com/1/direct_messages/new.json");

    url.addQueryItem("user_id", QString::number(userId));
    url.addQueryItem("text", text);

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::POST);
    req.setRawHeader("Authorization", oauthHeader);

    QNetworkReply *netReply = networkAccessManager()->post(req, QByteArray());
    connect(netReply, SIGNAL(finished()), this, SLOT(reply()));
    connect(netReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTwitNewDirectMessage::sendMessage(const QString &screenName, const QString &text)
{
    Q_ASSERT(networkAccessManager() != 0);

    QUrl url("http://api.twitter.com/1/direct_messages/new.json");

    url.addQueryItem("screen_name", screenName);
    url.addQueryItem("text", text);

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::POST);
    req.setRawHeader("Authorization", oauthHeader);

    QNetworkReply *netReply = networkAccessManager()->post(req, QByteArray());
    connect(netReply, SIGNAL(finished()), this, SLOT(reply()));
    connect(netReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTwitNewDirectMessage::reply()
{
    QNetworkReply* netReply = qobject_cast<QNetworkReply*>(sender());
    if (netReply) {
        // ### TODO: parsing and error checking
        netReply->deleteLater();
        emit finished();
    }
}

void QTwitNewDirectMessage::error()
{
    qDebug() << "http://api.twitter.com/version/direct_messages/new.json error";
    emit networkError(QString());
}
