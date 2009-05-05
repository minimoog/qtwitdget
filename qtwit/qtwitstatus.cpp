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
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#include "qtwitstatus.h"

void QTwitStatus::setCreatedFromTwitter(const QString &twitterDate)
{
	//Date format: Fri Dec 12 01:06:43 +0000 2008
	//Hopefully it will stay in UTC in the future

	QStringList datesplitted = twitterDate.split(" ", QString::SkipEmptyParts);

	QString date = datesplitted.takeFirst();
	date += datesplitted.takeFirst();
	date += datesplitted.takeFirst();
	date += datesplitted.takeLast();

	QString time = datesplitted.takeFirst();

	//date to english locale //Why English?????

	QLocale locale(QLocale::English);
	QDate dateFinal = locale.toDate(date, "dddMMMddyyyy");
	QTime timeFinal = QTime::fromString(time, "hh:mm:ss");

	d->created.setTimeSpec(Qt::UTC);
	d->created.setDate(dateFinal);
	d->created.setTime(timeFinal);
}

QString QTwitStatus::toTwitterDate() const
{
	//Date Format: Tue,+27+Mar+2007+22:55:48+GMT

	QLocale locale(QLocale::English);

	return locale.toString(d->created, "ddd,+dd+MMM+yyyy+hh:mm:ss+") + QString("GMT");
}