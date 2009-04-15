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

#ifndef QTWITFOLLOWERS_H
#define QTWITFOLLOWERS_H

#include <QBuffer>
#include <QXmlQuery>
#include <QStringList>
#include "qtwitbase.h"

/*! QTwitFollowers class */
class QTwitFollowers : public QTwitBase
{
	Q_OBJECT

public:
	/*! Constructor */
	QTwitFollowers(QObject *parent = 0);
	/*! Gets followers */
	QStringList getFollowers() const;
	/*! Updates followers for authenticated user */
	void updateFollowers();
	/*! Updates followers for user with twitter ID */
	void updateFollowers(int id);
	/*! Updates followers for user with screenName */
	void updateFollowers(const QString &screenName);

private:
	QStringList m_followers;
	
private slots:
	void reply(QNetworkReply *reply);
};

#endif //QTWITFOLLOWERS_H