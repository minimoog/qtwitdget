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
#include <QBuffer>
#include <QUrl>
#include <QVariant>
#include "qtwithometimeline.h"
#include "qtwitstatus.h"
#include "qjson/parser.h"

/*!
    Constructor
 */
QTwitHomeTimeline::QTwitHomeTimeline(QObject *parent)
:	QTwitBase(parent)
{
}

/*!
    Updates home timeline
    \param sinceid fetch tweets newest then sinceid tweet
    \param maxid fetch tweets older then maxid tweet
    \param count number of tweet to fetch (maximum 200)
    \param page page number (starts with 1)
    \remarks Set parameters to zero or default ctr to NOT query them
 */
void QTwitHomeTimeline::update(qint64 sinceId, qint64 maxId, int count, int page)
{
	Q_ASSERT(networkAccessManager() != 0);

    QUrl url("http://api.twitter.com/1/statuses/home_timeline.json");
	
	if(sinceId != 0){
		QString strSinceId = QString("%1").arg(sinceId);
		url.addQueryItem("since_id", strSinceId);
	}

	if(maxId != 0){
		url.addQueryItem("max_id", QString::number(maxId));
	}

	if(count != 0){
		QString strCount = QString("%1").arg(count);
		url.addQueryItem("count", strCount);
	}

	if(page != 0){
		QString strPage = QString("%1").arg(page);
		url.addQueryItem("page", strPage);
	}

	QNetworkRequest req(url);

	QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
	req.setRawHeader("Authorization", oauthHeader);

	QNetworkReply *netReply = networkAccessManager()->get(req);
	connect(netReply, SIGNAL(finished()), this, SLOT(reply()));
	connect(netReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTwitHomeTimeline::reply()
{
	QNetworkReply *netReply = qobject_cast<QNetworkReply*>(sender());
    QByteArray arrayReply = netReply->readAll();
    QBuffer response(&arrayReply);
    response.open(QIODevice::ReadOnly);

	if(netReply){
        //XmlReaderStatus xrs;
        //if(xrs.read(&response))
        //    emit finishedHomeTimeline(xrs.statuses());

        QJson::Parser parser;
        bool ok;

        QVariant jsonResponse = parser.parse(arrayReply, &ok);
        if (!ok) {
            qFatal("An error occured while parsing json response");
            return;
        }

        QList<QTwitStatus> qtStatuses;
        QList<QVariant> listStatuses = jsonResponse.toList();

        foreach(const QVariant& status, listStatuses) {
            QTwitStatus qtStatus;
            QVariantMap statusMap = status.toMap();
            //qDebug() << statusMap["created_at"].toString();

            qtStatus.setId(statusMap["id"].toLongLong());
            qtStatus.setText(statusMap["text"].toString());
            qtStatus.setReplyToStatusId(statusMap["in_reply_to_status_id"].toLongLong());
            qtStatus.setReplyToUserId(statusMap["in_reply_to_user_id"].toInt());
            qtStatus.setFavorited(statusMap["favorited"].toBool());
            qtStatus.setReplyToScreenName(statusMap["in_reply_to_screen_name"].toString());

            QVariantMap user = statusMap["user"].toMap();
            qtStatus.setUserId(user["id"].toInt());
            qtStatus.setName(user["name"].toString());
            qtStatus.setScreenName(user["screen_name"].toString());
            qtStatus.setLocation(user["location"].toString());
            qtStatus.setDescription(user["description"].toString());
            qtStatus.setProfileImageUrl(user["profile_image_url"].toString());
            qtStatus.setUrl(user["url"].toString());

            qtStatuses.append(qtStatus);
        }

        emit finishedHomeTimeline(qtStatuses);

		netReply->deleteLater();
	}
}

void QTwitHomeTimeline::error()
{
	qDebug() << "Home Timeline Error";
    emit networkError(QString());
}
