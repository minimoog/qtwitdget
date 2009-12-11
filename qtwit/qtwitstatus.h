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

#ifndef QTWITSTATUS_H
#define QTWITSTATUS_H

#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QLocale>
#include <QSharedData>

//forward declaration doesn't work in VS
#include "qtwitstatusdata.h"

//class QTwitStatusData;

//implicitly shared class

class QTwitStatus
{
public:

	QTwitStatus() { d = new QTwitStatusData(); }

	QTwitStatus(const QTwitStatus &other)
		:	d(other.d) { }

	void setCreated(const QDateTime &datetime) { d->created = datetime; }
	QDateTime created() const { return d->created; }

	void setId(qint64 id) { d->id = id; }
	qint64 id() const { return d->id; }

	void setText(const QString& text) { d->text = text; }
	QString text() const { return d->text; }

	void setSource(const QString& source) { d->source = source; }
	QString source() const { return d->source; }

	void setReplyToStatusId(qint64 statusId) { d->replyToStatusId = statusId; }
	qint64 replyToStatusId() const { return d->replyToStatusId; }

	void setReplyToUserId(int userId) { d->replyToUserId = userId; }
	int replyToUserId() const { return d->replyToUserId; }

	void setFavorited(bool favorited) { d->favorited = favorited; }
	bool favorited() const { return d->favorited; }

	void setReplyToScreenName(const QString& screenName) { d->replyToScreenName = screenName; }
	QString replyToScreenName() const { return d->replyToScreenName; }

	void setUserId(int userId) { d->userId = userId; }
	int userId() const { return d->userId; }

	void setName(const QString& name) { d->name = name; }
	QString name() const { return d->name; }

	void setScreenName(const QString& screenName) { d->screenName = screenName; }
	QString screenName() const { return d->screenName; }

	void setLocation(const QString& location) { d->location = location; }
	QString location() const { return d->location; }

	void setDescription(const QString& description) { d->description = description; }
	QString description() const { return d->description; }

	void setProfileImageUrl(QString url) { d->profileImageUrl = url; }
	QString profileImageUrl() const { return d->profileImageUrl; }

	void setUrl(const QString& url) { d->url = url; }
	QString url() const { return d->url; }

	void setFollowersCount(int followersCount) { d->followersCount = followersCount; }
	int folllowersCount() const { return d->followersCount; }

	void setCreatedFromTwitter (const QString &twitterDate);
	QString toTwitterDate() const;

    bool isRetweet() const { return d->isRetweet; }
    void setRetweet(bool rt) { d->isRetweet = rt; }

    void setFriendsCount(int count) { d->friendsCount = count; }
    int friendsCount() const { return d->friendsCount; }

    void setUserCreatedAt(const QDateTime& datetime) { d->userCreatedAt = datetime; }
    QDateTime userCreatedAt() const { return d->userCreatedAt; }

    void setFavouritesCount(int count) { d->favouritesCount = count; }
    int favouritesCount() const { return d->favouritesCount; }

private:
	QSharedDataPointer<QTwitStatusData> d;
};

#endif //QTWITSTATUS_H