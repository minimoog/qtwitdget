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
 
#ifndef QTWITHOMETIMELINE_H
#define	QTWITHOMETIMELINE_H

#include <QDateTime>
#include <QLocale>
#include <QList>
#include "qtwitbase.h"
#include "qtwitstatus.h"

/*!
    Fetches user home timeline
*/
class QTwitHomeTimeline : public QTwitBase
{
	Q_OBJECT
public:
	QTwitHomeTimeline(QObject *parent = 0);
	void update(qint64 sinceId = 0, qint64 maxId = 0, int count = 0, int page = 0);
	QList<QTwitStatus> getStatuses() const;

private slots:
	void reply();
	void error();

private:
	QList<QTwitStatus> m_statuses;
};

#endif //QTWITHOMETIMELINE_H
