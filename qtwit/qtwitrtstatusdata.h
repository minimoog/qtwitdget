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

#ifndef QTWITRTSTATUSDATA_H
#define QTWITRTSTATUSDATA_H 
 
#include <QSharedData>
#include <QDateTime>
#include <QString>

class QTwitRtStatusData : public QSharedData
{
public:

	QTwitRtStatusData() : id(0), replyToStatusId(0), replyToUserId(0), favorited(false), userId(0), followersCount(0), 
        friendsCount(0), favouritesCount(0), utcOffset(0), statusesCount(0), geoEnabled(false),
        verified(false), following(false)
    { }

	QDateTime created;
	qint64 id;		
	QString text;		
	QString source;
	qint64 replyToStatusId;
	int replyToUserId;
	bool favorited;
	QString replyToScreenName;
	int userId;
	QString name;
	QString screenName;
	QString location;
	QString description;
	QString profileImageUrl;
	QString url;
	int followersCount;
    int friendsCount;
    QDateTime userCreatedAt;
    int favouritesCount;
    int utcOffset;
    QString timezone;
    int statusesCount;
    bool geoEnabled;
    bool verified;
    bool following;
};

#endif //QTWITRTSTATUSDATA
