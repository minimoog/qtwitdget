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

#include "xmlreaduser.h"

XmlReadUser::XmlReadUser()
{
}

bool XmlReadUser::read(QIODevice *device)
{
    xml.setDevice(device);

    if (xml.readNextStartElement()) {
        if (xml.name() == "user")
            readUser();
        else
            xml.raiseError(QObject::tr("Not a user xml element"));
    }

    return !xml.error();
}

void XmlReadUser::readUser()
{
    while (xml.readNextStartElement()) {
        if (xml.name() == "id")
            m_twitUser.setId(xml.readElementText().toInt());
        else if (xml.name() == "name")
            m_twitUser.setName(xml.readElementText());
        else if (xml.name() == "location")
            m_twitUser.setLocation(xml.readElementText());
        else if (xml.name() == "description")
            m_twitUser.setDescription(xml.readElementText());
        else if (xml.name() == "profile_image_url")
            m_twitUser.setProfileImageUrl(xml.readElementText());
        else if (xml.name() == "url")
            m_twitUser.setUrl(xml.readElementText());
        else
            xml.skipCurrentElement();
    }
}

QString XmlReadUser::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3")
                 .arg(xml.errorString())
                 .arg(xml.lineNumber())
                 .arg(xml.columnNumber());
}

QTwitUser XmlReadUser::twitUser() const
{
    return m_twitUser;
}
