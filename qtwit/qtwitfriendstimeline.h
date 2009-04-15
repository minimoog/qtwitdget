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
 * Contact e-mail: Antonie Jovanoski <minimoog77@gmail.com>
 */

#ifndef QTWITFRIENDSTIMELINE_H
#define	QTWITFRIENDSTIMELINE_H

#include <QDateTime>
#include <QLocale>
#include <QList>
#include "qtwitbase.h"
#include "qtwitstatus.h"

/*! class QTwitFriendsTimeline */
class QTwitFriendsTimeline : public QTwitBase
{
	Q_OBJECT
public:
	/*! Constructor */
	QTwitFriendsTimeline(QObject *parent = 0);
	/*! Updates friends timeline 
	 * \remark Set parameters to zero or default ctr to NOT query them
	 * \remark clears all statuses
	 */
	void update(int sinceId = 0, int maxId = 0, int count = 0, int page = 0);
	/*! Gets statuses */
	QList<QTwitStatus> getStatuses() const;

private slots:
	void reply();
	void error();

private:
	QList<QTwitStatus> m_statuses;
};

#endif //QTWITFRIENDSTIMELINE_H