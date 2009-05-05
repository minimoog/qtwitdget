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
#include "qtwitdestroy.h"

QTwitDestroy::QTwitDestroy(QObject *parent)
:	QTwitBase(parent)
{
}

void QTwitDestroy::deleteStatus(int id)
{
	Q_ASSERT(networkAccessManager() != 0);

	QString urlString = QString("http://twitter.com/statuses/destroy/%1.xml").arg(id);
	QUrl url(urlString);
	
	m_id = id;

	QNetworkRequest req(url);
	QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::POST);
	req.setRawHeader("Authorization", oauthHeader);
	QNetworkReply *netReply = networkAccessManager()->post(req, QByteArray());
	connect(netReply, SIGNAL(finished()), this, SLOT(reply()));
	connect(netReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTwitDestroy::reply()
{
	/*
	if(reply->error() != QNetworkReply::NoError){
		qDebug() << reply->error();
		return;
	}

	//QByteArray output = reply->readAll();

	//TODO: Error checking

	reply->deleteLater();
	*/

	emit destroyed(m_id);
	emit finished();
}

void QTwitDestroy::error()
{
	qDebug() << "Error Destroy";
}