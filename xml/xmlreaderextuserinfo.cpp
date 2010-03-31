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

#include "xmlreaderextuserinfo.h"

/*!
    Constructor
 */
XmlReaderExtUserInfo::XmlReaderExtUserInfo()
{
}

/*!
    Returns extended user information
    \remarks Valid after correct parsing
 */
QTwitExtUserInfo XmlReaderExtUserInfo::userInfo() const
{
	return m_userInfo;
}

/*!
    Reads and parses XML extended user information
    \param device Device from where XML is read
    \return True if parsing was succesful, otherwise false
 */
bool XmlReaderExtUserInfo::read(QIODevice *device)
{
	setDevice(device);

	while(!atEnd()){
		readNext();

		if(isStartElement()){
			if(name() == "user")
				readUser();
			else
				raiseError(QObject::tr("parse error"));
		}
	}

	return !error();
}

void XmlReaderExtUserInfo::readUnknownElement()
{
	while(!atEnd()){
		readNext();

		if(isEndElement())
			break;

		if(isStartElement())
			readUnknownElement();
	}
}

void XmlReaderExtUserInfo::readUser()
{
	while(!atEnd()){
		readNext();

		if(isEndElement())
			break;

		if(isStartElement()){
			if(name() == "id")
				m_userInfo.setId(readElementText().toInt());
			else if(name() == "name")
				m_userInfo.setName(readElementText());
			else if(name() == "screen_name")
				m_userInfo.setScreenName(readElementText());
			else if(name() == "location")
				m_userInfo.setLocation(readElementText());
			else if(name() == "description")
				m_userInfo.setDescription(readElementText());
			else if(name() == "profile_image_url")
				m_userInfo.setProfileImageUrl(readElementText());
			else if(name() == "url")
				m_userInfo.setUrl(readElementText());
			else if(name() == "protected")
				//TODO
				readElementText();
			else if(name() == "followers_count")
				m_userInfo.setFollowersCount(readElementText().toInt());
			else if(name() == "profile_background_color")
				//TODO
				readElementText();
			else if(name() == "profile_text_color")
				//TODO
				readElementText();
			else if(name() == "profile_link_color")
				//TODO
				readElementText();
			else if(name() == "profile_sidebar_fill_color")
				//TODO
				readElementText();
			else if(name() == "profile_sidebar_border_color")
				//TODO
				readElementText();
			else if(name() == "friends_count")
				m_userInfo.setFriendsCount(readElementText().toInt());
			else if(name() == "created_at")
				//TODO
				readElementText();
			else if(name() == "favourites_count")
				m_userInfo.setFavouritesCount(readElementText().toInt());
			else if(name() == "utc_offset")
				m_userInfo.setUtcOffset(readElementText().toInt());
			else if(name() == "time_zone")
				m_userInfo.setTimezone(readElementText());
			else if(name() == "profile_background_image_url")
				//TODO
				readElementText();
			else if(name() == "profile_background_tile")
				//TODO
				readElementText();
			else if(name() == "statuses_count")
				m_userInfo.setStatusesCount(readElementText().toInt());
			else if(name() == "notifications")
				//TODO
				readElementText();
			else if(name() == "following")
				//TODO
				readElementText();
			else
				readUnknownElement();
		}
	}
}
