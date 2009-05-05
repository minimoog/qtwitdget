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

#include <QSharedData>
#include <QDateTime>
#include <QString>

class QTwitStatusData : public QSharedData
{
public:

	QTwitStatusData() : id(0), replyToStatusId(0), replyToUserId(0), favorited(false), userId(0), followersCount(0) 
	{	text.clear(); 
		source.clear();
		replyToScreenName.clear();
		name.clear();
		screenName.clear();
		location.clear();
		description.clear();
		profileImageUrl.clear();
		url.clear(); }

	QTwitStatusData(const QTwitStatusData &other) 
		: QSharedData(other), 
		created(other.created), 
		id(other.id), 
		text(other.text),
		source(other.source),
		replyToStatusId(other.replyToStatusId),
		replyToUserId(other.replyToUserId),
		favorited(other.favorited),
		replyToScreenName(other.replyToScreenName),
		userId(other.userId),
		name(other.name),
		screenName(other.screenName),
		location(other.location),
		description(other.description),
		profileImageUrl(other.profileImageUrl),
		url(other.url),
		followersCount(other.followersCount)
		 { }

	~QTwitStatusData() { }

	QDateTime created;
	int id;		
	QString text;		
	QString source;
	int replyToStatusId;
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
};

/*

<status>
	<created_at></created_at>
	<id></id>
	<text></text>
	<source></source>
	<truncated></truncated>
	<in_reply_to_status_id></in_reply_to_status_id>
	<in_reply_to_user_id></in_reply_to_user_id>
	<favorited></favorited>
	<in_reply_to_screen_name></in_reply_to_screen_name>
	<user>
		<id></id>
		<name></name>
		<screen_name></screen_name>
		<location></location>
		<description></description>
		<profile_image_url></profile_image_url>
		<url></url>
		<protected>false</protected>
		<followers_count></followers_count>
	</user>
</status>

*/