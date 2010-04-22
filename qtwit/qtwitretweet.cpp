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
#include <QBuffer>
#include "qtwitretweet.h"

/*!
    Constructor
 */
QTwitRetweet::QTwitRetweet(QObject *parent)
:   QTwitBase(parent)
{
}

/*!
    Retweets tweet with id
    \param statusid tweet id
 */
void QTwitRetweet::retweet(qint64 statusid)
{
    Q_ASSERT(networkAccessManager() != 0);

    m_retweetStatus.setId(0);

    QString urlString = QString("http://api.twitter.com/1/statuses/retweet/%1.json").arg(statusid);
    QUrl url(urlString);

    QNetworkRequest req(url);

    QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::POST);
    req.setRawHeader("Authorization", oauthHeader);

    QNetworkReply *netReply = networkAccessManager()->post(req, QByteArray());
    connect(netReply, SIGNAL(finished()), this, SLOT(reply()));
    connect(netReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTwitRetweet::reply()
{
    QNetworkReply *netReply = qobject_cast<QNetworkReply*>(sender());
    if (netReply) {

        m_retweetStatus = parseStatusJSON(netReply);

        netReply->deleteLater();

        emit finished();
    }
}

void QTwitRetweet::error()
{
    qDebug() << "Mentions Error";

    QNetworkReply *netReply = qobject_cast<QNetworkReply*>(sender());
    if (netReply) {
        qDebug() << netReply->readAll();
    }
}

/*!
    Getsid of last retweeted tweet
    \return id of last retweet
    \remarks If retweeting was with error id is zero
 */
QTwitStatus QTwitRetweet::getRetweetedStatus() const
{
    return m_retweetStatus;
}
