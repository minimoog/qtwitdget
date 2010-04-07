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
#include "qtwit/qtwitstatus.h"
#include "qtwit/qtwitrtstatus.h"

class QTwitStatusData : public QSharedData
{
public:

	QTwitStatusData() : id(0), replyToStatusId(0), replyToUserId(0), favorited(false), userId(0), followersCount(0), 
        isRetweet(false), friendsCount(0), favouritesCount(0), utcOffset(0), statusesCount(0), geoEnabled(false),
        verified(false), isRead(false)
    { }

	QDateTime created;
	qint64 id;		
	QString text;		
	QString source;
	qint64 replyToStatusId;
	int replyToUserId;
	bool favorited;
	QString replyToScreenName;
    bool isRetweet;
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
    bool isRead;
    QTwitRtStatus rtStatus;
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
    <retweeted_status>
        <created_at></created_at>
        <id></id>
        <text></text>
        <source></source>
        <truncated></truncated>
        <in_reply_to_status_id></in_reply_to_status_id>
        <in_reply_to_user_id></in_reply_to_user_id>
        <favorited>false</favorited>
        <in_reply_to_screen_name></in_reply_to_screen_name>
        <user>
            <id></id>
            <name></name>
            <screen_name></screen_name>
            <location></location>
            <description></description>
            <profile_image_url></profile_image_url>
            <url></url>
            <protected></protected>
            <followers_count></followers_count>
            <profile_background_color></profile_background_color>
            <profile_text_color></profile_text_color>
            <profile_link_color></profile_link_color>
            <profile_sidebar_fill_color></profile_sidebar_fill_color>
            <profile_sidebar_border_color></profile_sidebar_border_color>
            <friends_count></friends_count>
            <created_at></created_at>
            <favourites_count></favourites_count>
            <utc_offset></utc_offset>
            <time_zone></time_zone>
            <profile_background_image_url></profile_background_image_url>
            <profile_background_tile></profile_background_tile>
            <statuses_count></statuses_count>
            <notifications></notifications>
            <geo_enabled></geo_enabled>
            <verified></verified>
            <following></following>
        </user>
        <geo/>
    </retweeted_status>
	<user>
		<id></id>
		<name></name>
		<screen_name></screen_name>
		<location></location>
		<description></description>
		<profile_image_url></profile_image_url>
		<url></url>
		<protected></protected>
		<followers_count></followers_count>
        <profile_background_color></profile_background_color>
        <profile_text_color></profile_text_color>
        <profile_link_color></profile_link_color>
        <profile_sidebar_fill_color></profile_sidebar_fill_color>
        <profile_sidebar_border_color></profile_sidebar_border_color>
        <friends_count></friends_count>
        <created_at></created_at>
        <favourites_count></favourites_count>
        <utc_offset></utc_offset>
        <time_zone></time_zone>
        <profile_background_image_url></profile_background_image_url>
        <profile_background_tile></profile_background_tile>
        <statuses_count></statuses_count>
        <notifications></notifications>
        <geo_enabled></geo_enabled>
        <verified></verified>
        <following></following>
	</user>
</status>

*/
