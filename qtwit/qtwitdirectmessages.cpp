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
#include "qtwitdirectmessages.h"

QTwitDirectMessages::QTwitDirectMessages(QObject *parent) :
    QTwitBase(parent)
{
}

void QTwitDirectMessages::directMessages(qint64 sinceid, qint64 maxid, int count, int page)
{
    Q_ASSERT(networkAccessManager() != 0);

    QUrl url("http://api.twitter.com/1/direct_messages.json");

    if (sinceid != 0)
        url.addQueryItem("since_id", QString::number(sinceid));

    if (maxid != 0)
        url.addQueryItem("max_id", QString::number(maxid));

    if (count != 0)
        url.addQueryItem("count", QString::number(count));

    if (page != 0)
        url.addQueryItem("page", QString::number(page));

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
    req.setRawHeader("Authorization", oauthHeader);

    QNetworkReply *netReply = networkAccessManager()->get(req);
    connect(netReply, SIGNAL(finished()), this, SLOT(reply()));
    connect(netReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTwitDirectMessages::reply()
{
    QNetworkReply* netReply = qobject_cast<QNetworkReply*>(sender());
    if (netReply) {
        QList<QTwitDMStatus> dm = parseDirectMessagesListJSON(netReply);
        emit finishedDirectMessages(dm);

        netReply->deleteLater();
    }
}

void QTwitDirectMessages::error()
{
    qDebug() << "http://api.twitter.com/1/direct_messages.json error";
    emit networkError(QString());
}
