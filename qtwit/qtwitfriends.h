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

#ifndef QTWITFRIENDS_H
#define QTWITFRIENDS_H

#include "qtwitbase.h"
#include "qtwitextuserinfo.h"

/*! QTwitFriends class */
class QTwitFriends : public QTwitBase
{
	Q_OBJECT

public:
	/*! Constructor */
	QTwitFriends(QObject *parent = 0);
	/*! Updates friends for authenticated user */
	void updateFriends(int id = 0, int userId = 0, const QString& screenName = QString(), int page = 0);
	QList<QTwitExtUserInfo> getFriends() const;

private:
	QList<QTwitExtUserInfo> m_friends;

private slots:
	void reply();
	void error();
};

#endif //QTWITFRIENDS_H