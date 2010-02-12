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
#include "qtwithometimeline.h"
#include "xml/xmlreaderstatus.h"

QTwitHomeTimeline::QTwitHomeTimeline(QObject *parent)
:	QTwitBase(parent)
{
}

void QTwitHomeTimeline::update(qint64 sinceId, qint64 maxId, int count, int page)
{
	Q_ASSERT(networkAccessManager() != 0);

	m_statuses.clear();

	QUrl url("http://api.twitter.com/1/statuses/home_timeline.xml");
	
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
		XmlReaderStatus xrs;
		if(xrs.read(&response))
			m_statuses = xrs.statuses();		

		netReply->deleteLater();

		emit finished();
	}
}

void QTwitHomeTimeline::error()
{
	qDebug() << "Home Timeline Error";
    emit networkError(QString());
}

QList<QTwitStatus> QTwitHomeTimeline::getStatuses() const
{
	return m_statuses;
}
