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

#include "qtwitusertimeline.h"

QTwitUserTimeline::QTwitUserTimeline(QObject *parent)
:	QTwitBase(parent)
{
}

QStringList QTwitUserTimeline::getTimeline() const
{
	return m_timeline;
}

//TODO: Implement full user_timeline (id for different user, count, since, page)

void QTwitUserTimeline::updateTimeline()
{
	QNetworkRequest req(QUrl("https://twitter.com/statuses/user_timeline.xml"));

	m_netManager.get(req);
}

void QTwitUserTimeline::updateTimeline(const QString &screenName)
{
	QString urlString = QString("http://twitter.com/statuses/user_timeline/%1.xml").arg(screenName);
	QUrl url(urlString);
	QNetworkRequest req(url);
	m_netManager.get(req);
}

void QTwitUserTimeline::updateTimeline(const QString &screenName, int count)
{
	if(count > 200)
		count = 0;

	QString urlString;

	if(count == 0){
		urlString = QString("http://twitter.com/statuses/user_timeline/%1.xml").arg(screenName);
	} else {
		urlString = QString("http://twitter.com/statuses/user_timeline/%1.xml?count=%2").arg(screenName).arg(count);
	}

	QUrl url(urlString);
	QNetworkRequest req(url);
	m_netManager.get(req);
}

void QTwitUserTimeline::updateTimeline(const QString &screenName, QDateTime &datetime)
{
	QString urlString;

	//TODO: check for longer than 24 hours

	if(datetime.isNull()){
		urlString = QString("http://twitter.com/statuses/user_timeline/%1.xml").arg(screenName);
	} else {
		QLocale locale(QLocale::English);
		QString dateEnglish = locale.toString(datetime, "ddd,+dd+MMM+yyyy+hh:mm:ss+") + QString("GMT");
		urlString = QString("http://twitter.com/statuses/user_timeline/%1.xml?since=%2").arg(screenName).arg(dateEnglish);
	}

	QUrl url(urlString);
	QNetworkRequest req(url);
	m_netManager.get(req);
}

void QTwitUserTimeline::updateTimeline(int id)
{
	QString urlString = QString("http://twitter.com/statuses/user_timeline/%1.xml").arg(id);
	QUrl url(urlString);
	QNetworkRequest req(url);
	m_netManager.get(req);
}

void QTwitUserTimeline::updateTimeline(int id, int count)
{
	if(count > 200)
		count = 0;

	QString urlString;

	if(count == 0){
		urlString = QString("http://twitter.com/statuses/user_timeline/%1.xml").arg(id);
	} else {
		urlString = QString("http://twitter.com/statuses/user_timeline/%1.xml?count=%2").arg(id).arg(count);
	}

	QUrl url(urlString);
	QNetworkRequest req(url);
	m_netManager.get(req);
}

void QTwitUserTimeline::updateTimeline(int id, QDateTime &datetime)
{
	QString urlString;

	//TODO: check for longer than 24 hours

	if(datetime.isNull()){
		urlString = QString("http://twitter.com/statuses/user_timeline/%1.xml").arg(id);
	} else {
		QLocale locale(QLocale::English);
		QString dateEnglish = locale.toString(datetime, "ddd,+dd+MMM+yyyy+hh:mm:ss+") + QString("GMT");
		urlString = QString("http://twitter.com/statuses/user_timeline/%1.xml?since=%2").arg(id).arg(dateEnglish);
	}

	QUrl url(urlString);
	QNetworkRequest req(url);
	m_netManager.get(req);
}

//TODO: Instead parsing with xmlpatterns try it with regular expressions

void QTwitUserTimeline::reply(QNetworkReply *reply)
{
	if(reply->error() != QNetworkReply::NoError)
		return;

	QByteArray output = reply->readAll();

	QBuffer outputBuffer(&output);
	outputBuffer.open(QIODevice::ReadOnly);

	QXmlQuery query;
	query.bindVariable("inputDocument", &outputBuffer);
	query.setQuery("declare variable $inputDocument external; doc($inputDocument)//status/string(text)");

	m_timeline.clear();

	if(!query.evaluateTo(&m_timeline))
		return;

	reply->deleteLater();

	emit finished();
}
