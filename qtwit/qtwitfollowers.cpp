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
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77@gmail.com>
 */

#include "qtwitfollowers.h"

QTwitFollowers::QTwitFollowers(QObject *parent)
:	QTwitBase(parent)
{
}

//TODO: Remove requirement

QStringList QTwitFollowers::getFollowers() const
{
	return m_followers;
}

//TODO: Implement other parameters, id, page
//TODO: username and password here?

void QTwitFollowers::updateFollowers()
{
	QNetworkRequest req(QUrl("https://twitter.com/statuses/followers.xml"));

	QByteArray header("Authorization");

	QByteArray userpass;
	userpass.append(m_username.toUtf8());
	userpass.append(":");
	userpass.append(m_password.toUtf8());

	QByteArray userpass64enc = userpass.toBase64();

	QByteArray basicAuth("Basic ");
	basicAuth.append(userpass64enc);

	req.setRawHeader(header, basicAuth);

	m_netManager.get(req);
}

void QTwitFollowers::updateFollowers(int id)
{
	QString urlString = QString("http://twitter.com/statuses/followers/%1.xml").arg(id);
	QUrl url(urlString);

	QNetworkRequest req(url);

	m_netManager.get(req);
}

void QTwitFollowers::updateFollowers(const QString &screenName)
{
	QString urlString = QString("http://twitter.com/statuses/followers/%1.xml").arg(screenName);
	QUrl url(urlString);

	QNetworkRequest req(url);

	m_netManager.get(req);
}

void QTwitFollowers::reply(QNetworkReply *reply)
{
	if(reply->error() != QNetworkReply::NoError)
		return;

	QByteArray output = reply->readAll();

	QBuffer outputBuffer(&output);
	outputBuffer.open(QIODevice::ReadOnly);

	QXmlQuery query;
	query.bindVariable("inputDocument", &outputBuffer);
	query.setQuery(tr("declare variable $inputDocument external; doc($inputDocument)//user/string(name)"));

	m_followers.clear();

	if(!query.evaluateTo(&m_followers))
		return;

	reply->deleteLater();

	emit finished();
}
