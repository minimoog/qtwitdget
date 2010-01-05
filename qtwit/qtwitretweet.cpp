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
#include "xml/xmlreaderstatus.h"

QTwitRetweet::QTwitRetweet(QObject *parent)
:   QTwitBase(parent)
{
}

void QTwitRetweet::retweet(qint64 statusid)
{
    Q_ASSERT(networkAccessManager() != 0);

    m_retweetStatus.setId(0);

    QString urlString = QString("http://api.twitter.com/1/statuses/retweet/%1.xml").arg(statusid);
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
        XmlReaderStatus xrs;

        QByteArray replyArray = netReply->readAll();
        //HACK: prepend <statuses> and append </statuses> tags to parse them like statuses array
        int prIndex = replyArray.indexOf("<status>");
        
        if (prIndex == -1)
            //something wrong
            return;

        replyArray.insert(prIndex, "<statuses>");
        replyArray.append("</statuses>");

        //TODO:
        //XmlReaderStatus -> parse one status
        //XmlReaderStatusArray -> parse array of statuses

        QBuffer replyBuffer(&replyArray);
        replyBuffer.open(QIODevice::ReadOnly);

        if (xrs.read(&replyBuffer))
            m_retweetStatus = xrs.statuses().at(0); //fails if there are no statuses

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

QTwitStatus QTwitRetweet::getRetweetedStatus() const
{
    return m_retweetStatus;
}
