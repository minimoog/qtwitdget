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

#ifndef XMLREADDIRECTMESSAGES_H
#define XMLREADDIRECTMESSAGES_H

#include <QXmlStreamReader>
#include "qtwit/qtwitdmstatus.h"

class XmlReadDirectMessages
{
public:
    XmlReadDirectMessages();
    bool read(QIODevice* device);
    QString errorString() const;
    QList<QTwitDMStatus> directMessages() const;

private:
    void readDirectMessages();
    void readDirectMessage(QTwitDMStatus& dmStatus);

    QXmlStreamReader m_xml;
    QList<QTwitDMStatus> m_directMessages;
};

#endif // XMLREADDIRECTMESSAGES_H
