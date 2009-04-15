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

#ifndef QTWITUSERTIMELINE_H
#define QTWITUSERTIMELINE_H

#include <QBuffer>
#include <QXmlQuery>
#include <QStringList>
#include <QDateTime>
#include <QLocale>
#include "qtwitbase.h"

/*! QTwitUserTimeline class */
class QTwitUserTimeline : public QTwitBase
{
	Q_OBJECT

public:
	/*! Constructor */
	QTwitUserTimeline(QObject *parent = 0);
	/*! Gets user timeline */
	QStringList getTimeline() const;
	/*! Update timeline for authenticated user */
	void updateTimeline();
	/*! Update timeline for user with screenName */
	void updateTimeline(const QString &screenName);
	/*! Update up to count timelines (statutes) for user with screenName */
	void updateTimeline(const QString &screenName, int count);
	/*! Update timeline since datetime (no longer then 24 hours?) for user with screenName */
	void updateTimeline(const QString &screenName, QDateTime &datetime);
	/*! Update timeline for user with ID */
	void updateTimeline(int id);
	/*! Update timeline with count statutes for user ID */
	void updateTimeline(int id, int count);
	/*! Update timeline since datetime for user ID */
	void updateTimeline(int id, QDateTime &datetime);

private:
	QStringList m_timeline;
	
private slots:	
	void reply(QNetworkReply *reply);
};

#endif //QTWITUSERTIMELINE_H