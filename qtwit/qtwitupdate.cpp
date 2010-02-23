/* Copyright (c) 2008, Antonie Jovanoski
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
 */

#include <QtDebug>
#include "qtwitupdate.h"

QTwitUpdate::QTwitUpdate(QObject *parent)
:	QTwitBase(parent)
{
}

void QTwitUpdate::setUpdate(const QString &update, qint64 inReplyToStatusId)
{
	QByteArray statusUtf8 = update.toUtf8();
	QByteArray status = statusUtf8.toPercentEncoding();
	QByteArray key("status");

    QUrl url("http://api.twitter.com/1/statuses/update.xml");
	QUrl urlQuery(url);

	urlQuery.addEncodedQueryItem(key, status);

	if(inReplyToStatusId != 0){
		QString strInReplyToStatusId = QString("%1").arg(inReplyToStatusId);
		urlQuery.addQueryItem("in_reply_to_status_id", strInReplyToStatusId);
	}
	
	QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(urlQuery, OAuth::POST);
	QNetworkRequest req(url);
	req.setRawHeader("Authorization", oauthHeader);

	//build status post
	QByteArray post = key + '=' + status;
	
	if(inReplyToStatusId != 0){
		post.append("&in_reply_to_status_id=");
		post.append(QByteArray::number(inReplyToStatusId));
	}
	
	QNetworkReply *netReply = networkAccessManager()->post(req, post);
	connect(netReply, SIGNAL(finished()), this, SLOT(reply()));
	connect(netReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTwitUpdate::reply()
{
	//TODO: Check result
	emit finished();
}

void QTwitUpdate::error()
{
	qDebug() << "Update error";
}
