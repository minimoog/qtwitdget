/* Copyright (c) 2010, Antonie Jovanoski
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

#include <QtDebug>
#include "xmlreaduserlist.h"
#include "xmlreaduser.h"
#include "qtwit/qtwituser.h"

XmlReadUserList::XmlReadUserList()
{
}

bool XmlReadUserList::read(QIODevice *device)
{
    xml.setDevice(device);

    if (xml.readNextStartElement()) {
        if (xml.name() == "users_list")
            readUsersList();
        else
            xml.raiseError(QObject::tr("Not a user xml element"));
    }
    return !xml.error();
}

void XmlReadUserList::readUsersList()
{
    while(xml.readNextStartElement()) {
        if (xml.name() == "users")
            readUsers();
        else if (xml.name() == "next_cursor")
            m_nextCursor = xml.readElementText();
        else if (xml.name() == "previous_cursor")
            m_prevCursor = xml.readElementText();
        else
            xml.skipCurrentElement();
    }
}

void XmlReadUserList::readUsers()
{
    while (xml.readNextStartElement()) {
        if (xml.name() == "user") {
            QTwitUser twitUser;
            readUser(twitUser);
            m_users.append(twitUser);
        }
        else
            xml.skipCurrentElement();
    }
}

void XmlReadUserList::readUser(QTwitUser& twitUser)
{
    while (xml.readNextStartElement()) {
        if (xml.name() == "id")
            twitUser.setId(xml.readElementText().toInt());
        else if (xml.name() == "name")
            twitUser.setName(xml.readElementText());
        else if (xml.name() == "screen_name")
            twitUser.setScreenName(xml.readElementText());
        else if (xml.name() == "location")
            twitUser.setLocation(xml.readElementText());
        else if (xml.name() == "description")
            twitUser.setDescription(xml.readElementText());
        else if (xml.name() == "profile_image_url")
            twitUser.setProfileImageUrl(xml.readElementText());
        else if (xml.name() == "url")
            twitUser.setUrl(xml.readElementText());
        else
            xml.skipCurrentElement();
    }
}

QString XmlReadUserList::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3")
                 .arg(xml.errorString())
                 .arg(xml.lineNumber())
                 .arg(xml.columnNumber());
}

QList<QTwitUser> XmlReadUserList::users() const
{
    return m_users;
}

QString XmlReadUserList::nextCursor() const
{
    return m_nextCursor;
}

QString XmlReadUserList::prevCursor() const
{
    return m_prevCursor;
}

