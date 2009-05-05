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
 
#include <QSharedData>
#include <QDateTime>
#include <QString>
#include <QColor>

class QTwitExtUserInfoData : public QSharedData
{
public:
	QTwitExtUserInfoData() : id(0), protected_(false), followersCount(0), friendsCount(0), favouritesCount(0), utcOffset(0),
		profileBackgroundTile(false), statusesCount(0), notifications(false), following(false)
	{
		name.clear();
		screenName.clear();
		location.clear();
		description.clear();
		profileImageUrl.clear();
		url.clear();
		timezone.clear();
		profileBackgroundImageUrl.clear();
	}
	
	QTwitExtUserInfoData(const QTwitExtUserInfoData &other) : QSharedData(other),
		id(other.id),
		name(other.name),
		screenName(other.screenName),
		location(other.location),
		description(other.description),
		profileImageUrl(other.profileImageUrl),
		url(other.url),
		protected_(other.protected_),
		followersCount(other.followersCount),
		profileBackgroundColor(other.profileBackgroundColor),
		profileTextColor(other.profileTextColor),
		profileLinkColor(other.profileLinkColor),
		profileSidebarFillColor(other.profileSidebarFillColor),
		profileSidebarBorderColor(other.profileSidebarBorderColor),
		friendsCount(other.friendsCount),
		created(other.created),
		favouritesCount(other.favouritesCount),
		utcOffset(other.utcOffset),
		timezone(other.timezone),
		profileBackgroundImageUrl(other.profileBackgroundImageUrl),
		profileBackgroundTile(other.profileBackgroundTile),
		statusesCount(other.statusesCount),
		notifications(other.notifications),
		following(other.following)
	{}
	
	~QTwitExtUserInfoData() {}
	
	int id;
	QString name;
	QString screenName;
	QString location;
	QString description;
	QString profileImageUrl;
	QString url;
	bool protected_;
	int followersCount;
	QColor profileBackgroundColor;
	QColor profileTextColor;
	QColor profileLinkColor;
	QColor profileSidebarFillColor;
	QColor profileSidebarBorderColor;
	int friendsCount;
	QDateTime created;
	int favouritesCount;
	int utcOffset;
	QString timezone;
	QString profileBackgroundImageUrl;
	bool profileBackgroundTile;
	int statusesCount;
	bool notifications;
	bool following;
};
	