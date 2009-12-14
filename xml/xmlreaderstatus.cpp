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

#include "xmlreaderstatus.h"

XmlReaderStatus::XmlReaderStatus()
{
}

QList<QTwitStatus> XmlReaderStatus::statuses() const
{
	return m_statuses;
}

bool XmlReaderStatus::read(QIODevice *device)
{
	setDevice(device);

	while(!atEnd()){
		readNext();

		if(isStartElement()){
			if(name() == "statuses")
				readStatuses();
			else
				raiseError(QObject::tr("parse error"));
		}
	}

	return !error();
}

void XmlReaderStatus::readUnknownElement()
{
	while(!atEnd()){
		readNext();

		if(isEndElement())
			break;

		if(isStartElement())
			readUnknownElement();
	}
}

void XmlReaderStatus::readStatuses()
{
	while(!atEnd()){
		readNext();

		if(isEndElement())
			break;

		if(isStartElement()){
			if(name() == "status"){
				QTwitStatus ts;
				readStatus(ts);
				m_statuses.append(ts);
			}
			else
				readUnknownElement();
		}
	}
}

void XmlReaderStatus::readStatus(QTwitStatus& ts)
{
	while(!atEnd()){
		readNext();

		if(isEndElement())
			break;

		if(isStartElement()){
			if(name() == "created_at")
				ts.setCreatedFromTwitter(readElementText());
			else if(name() == "id")
				ts.setId(readElementText().toLongLong());
			else if(name() == "text")
				ts.setText(readElementText());
			else if(name() == "source")
				ts.setSource(readElementText());
			else if(name() == "in_reply_to_status_id")
				readInReplyToStatusId(ts);
			else if(name() == "in_reply_to_user_id"){
				readInReplyToUserId(ts);
			}
			else if(name() == "favorited")
				if(readElementText() == "true")
					ts.setFavorited(true);
				else
					ts.setFavorited(false);
			else if(name() == "in_reply_to_screen_name")
				ts.setReplyToScreenName(readElementText());
            else if(name() == "retweeted_status")
                ;           //TODO
			else if(name() == "user")
				readUser(ts);
			else
				readUnknownElement();
		}
	}
}

void XmlReaderStatus::readInReplyToStatusId(QTwitStatus& ts)
{
	QString sid = readElementText();
	if(!sid.isEmpty())
		ts.setReplyToStatusId(sid.toLongLong());
}

void XmlReaderStatus::readInReplyToUserId(QTwitStatus& ts)
{
	QString uid = readElementText();
	if(!uid.isEmpty())
		ts.setReplyToUserId(uid.toInt());
}

void XmlReaderStatus::readUser(QTwitStatus& ts)
{
	while(!atEnd()){
		readNext();

		if(isEndElement())
			break;

		if(isStartElement()){
			if(name() == "id")
				ts.setUserId(readElementText().toInt());
			else if(name() == "name")
				ts.setName(readElementText());
			else if(name() == "screen_name")
				ts.setScreenName(readElementText());
			else if(name() == "location")
				ts.setLocation(readElementText());
			else if(name() == "description")
				ts.setDescription(readElementText());
			else if(name() == "profile_image_url")
				ts.setProfileImageUrl(readElementText());
			else if(name() == "url")
				ts.setUrl(readElementText());
			else if(name() == "followers_count")
				ts.setFollowersCount(readElementText().toInt());
            else if(name() == "friends_count")
                ts.setFriendsCount(readElementText().toInt());
            else if(name() == "created_at")
                ts.setUserCreatedAt(QDateTime()); //TODO
            else if(name() == "favourites_count")
                ts.setFavouritesCount(readElementText().toInt());
            else if(name() == "utc_offset")
                ts.setUtcOffset(readElementText().toInt());
            else if(name() == "time_zone")
                ts.setTimezone(readElementText());
            else if(name() == "statuses_count")
                ts.setStatusesCount(readElementText().toInt());
            else if(name() == "geo_enabled")
                if(readElementText() == "true")
                    ts.setGeoEnabled(true);
                else
                    ts.setGeoEnabled(false);
            else if(name() == "verified")
                if(readElementText() == "true")
                    ts.setVerified(true);
                else
                    ts.setVerified(false);
            else if(name() == "following")
                if(readElementText() == "true")
                    ts.setFollowing(true);
                else
                    ts.setFollowing(false);
			else
				readUnknownElement();
		}
	}
}