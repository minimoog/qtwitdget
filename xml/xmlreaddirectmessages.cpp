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

#include "xmlreaddirectmessages.h"

XmlReadDirectMessages::XmlReadDirectMessages()
{
}

bool XmlReadDirectMessages::read(QIODevice *device)
{
    m_xml.setDevice(device);

    if (m_xml.readNextStartElement()) {
        if (m_xml.name() == "direct-messages")
            readDirectMessages();
        else
            m_xml.raiseError(QObject::tr("Not a direct-messages element"));
    }

    return !m_xml.error();
}

void XmlReadDirectMessages::readDirectMessages()
{
    while (m_xml.readNextStartElement()) {
        if (m_xml.name() == "direct_message") {
            QTwitDMStatus dmStatus;
            readDirectMessage(dmStatus);
            m_directMessages.append(dmStatus);
        }
        else
            m_xml.skipCurrentElement();
    }
}

void XmlReadDirectMessages::readDirectMessage(QTwitDMStatus& dmStatus)
{
    while (m_xml.readNextStartElement()) {
        if (m_xml.name() == "id")
            dmStatus.setId(m_xml.readElementText().toInt());
        else if (m_xml.name() == "sender_id")
            dmStatus.setSenderId(m_xml.readElementText().toInt());
        else if (m_xml.name() == "text")
            dmStatus.setText(m_xml.readElementText());
        else if (m_xml.name() == "recipient_id")
            dmStatus.setRecipientId(m_xml.readElementText().toInt());
        else if (m_xml.name() == "created_at")
            ;                                   // ### TODO
        else if (m_xml.name() == "sender_screen_name")
            dmStatus.setSenderScreenName(m_xml.readElementText());
        else if (m_xml.name() == "recipient_screen_name")
            dmStatus.setRecipientScreenName(m_xml.readElementText());
        else
            m_xml.skipCurrentElement();
    }
}

QList<QTwitDMStatus> XmlReadDirectMessages::directMessages() const
{
    return m_directMessages;
}
