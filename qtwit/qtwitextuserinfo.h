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
 
#ifndef QTWITEXTUSERINFO_H
#define QTWITEXTUSERINFO_H

#include "qtwitextuserinfodata.h"

//implicitly shared class

class QTwitExtUserInfo 
{
public:
	QTwitExtUserInfo() { d = new QTwitExtUserInfoData(); }
	QTwitExtUserInfo(const QTwitExtUserInfo& other) : d(other.d) {}
	void setId(int id) { d->id = id; }
	int id() const { return d->id; }
	void setName(const QString& name) { d->name = name; }
	QString name() const { return d->name; }
	void setScreenName(const QString& screenName) { d->screenName = screenName; }
	QString screenName() const { return d->screenName; }
	void setLocation(const QString& location) { d->location = location; }
	QString location() const { return d->location; }
	void setDescription(const QString& description) { d->description = description; }
	QString description() const { return d->description; }
	void setProfileImageUrl(const QString& profileImageUrl) { d->profileImageUrl = profileImageUrl; }
	QString profileImageUrl() const { return d->profileImageUrl; }
	void setUrl(const QString& url) { d->url = url; }
	QString url() const { return d->url; }
	void setProtected(bool protected_) { d->protected_ = protected_; }
	bool protected_() const { return d->protected_; }
	void setFollowersCount(int followersCount) { d->followersCount = followersCount; }
	int followersCount() const { return d->followersCount; }
	void setProfileBackgroundColor(const QColor& color) { d->profileBackgroundColor = color; }
	QColor profileBackgroundColor() const { return d->profileBackgroundColor; }
	void setProfileTextColor(const QColor& color) { d->profileTextColor = color; }
	QColor profileTextColor() const { return d->profileTextColor; }
	void setProfileLinkColor(const QColor& color) { d->profileLinkColor = color; }
	QColor profileLinkColor() const { return d->profileLinkColor; }
	void setProfileSidebarFillColor(const QColor& color) { d->profileSidebarFillColor = color; }
	QColor profileSidebarFillColor() const { return d->profileSidebarFillColor; }
	void setProfileSidebarBorderColor(const QColor& color) { d->profileSidebarBorderColor = color; }
	QColor profileSidebarBorderColor() const { return d->profileSidebarBorderColor; };
	void setFriendsCount(int count) { d->friendsCount = count; }
	int friendsCount() const { return d->friendsCount; }
	void setCreated(const QDateTime& datetime) { d->created = datetime; }
	QDateTime created() const { return d->created; }
	void setFavouritesCount(int count) { d->favouritesCount = count; }
	int favoritesCount() const { return d->favouritesCount; }
	void setUtcOffset(int offset) { d->utcOffset = offset; }
	int utcOffset() const { return d->utcOffset; }
	void setTimezone(const QString& timezone) { d->timezone = timezone; }
	QString timezone() const { return d->timezone; }
	void setProfileBackgroundImageUrl(const QString& url) { d->profileBackgroundImageUrl = url; }
	QString profileBackgroundImageUrl() const { return d->profileBackgroundImageUrl; }
	void setProfileBackgroundTile(bool tile) { d->profileBackgroundTile = tile; }
	bool profileBackgroundTile() const { return d->profileBackgroundTile; }
	void setStatusesCount(int count) { d->statusesCount = count; }
	int statusesCount() const { return d->statusesCount; }
	void setNotifications(bool notifications) { d->notifications = notifications; }
	bool notifications() const { return d->notifications; }
	void setFollowing(bool following) { d->following = following; }
	bool following() const { return d->following; }

private:
	QSharedDataPointer<QTwitExtUserInfoData> d;
};

#endif //QTWITEXTUSERINFO_H