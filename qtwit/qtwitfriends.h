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
#include "qtwituser.h"

/*!
    Gets user's friends
 */
class QTwitFriends : public QTwitBase
{
	Q_OBJECT

public:
	QTwitFriends(QObject *parent = 0);
	QTwitFriends(QNetworkAccessManager* netManager, OAuthTwitter* oauthTwitter, QObject *parent = 0);
    void updateFriends(int id = 0,
                       int userId = 0,
                       const QString& screenName = QString(),
                       const QString& cursor = QString());

    QList<QTwitUser> getFriends() const;

private slots:
    void reply();
    void error();

private:
    QList<QTwitUser> m_friends;
    bool m_paging;
};

#endif //QTWITFRIENDS_H
