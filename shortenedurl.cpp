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
#include <QUrl>
#include <QNetworkRequest>
#include <QApplication>
#include "shortenedurl.h"

ShortenedUrl::ShortenedUrl()
{
	connect(&m_netManager, SIGNAL(finished(QNetworkReply*)), SLOT(finished(QNetworkReply*)));
}

QString ShortenedUrl::shortUrl(const QString &url)
{s
	//TODO: Add other services for shortening
	QString urlService("http://is.gd/api.php?longurl=");
	urlService += url;

	QUrl sUrl(urlService);
	QNetworkRequest req(sUrl);
	m_blocked = true;
	
	m_netManager.get(req);

	while(m_blocked)
		qApp->processEvents();

	return m_shortenedUrl;
}

void ShortenedUrl::finished(QNetworkReply* reply)
{
	m_blocked = false;

	if(reply->error() != QNetworkReply::NoError){
		qDebug() << "Error shortening url";
		qDebug() << reply->readAll();
		m_shortenedUrl.clear();
		return;
	}

	m_shortenedUrl = reply->readAll();
}