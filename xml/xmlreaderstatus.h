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

#ifndef XMLREADERSTATUS_H
#define XMLREADERSTATUS_H

#include <QXmlStreamReader>
#include "qtwit/qtwitstatus.h"

/*!
    Parses XML Twitter list of statuses
 */
class XmlReaderStatus : public QXmlStreamReader
{
public:
	XmlReaderStatus();
	bool read(QIODevice *device);
	QList<QTwitStatus> statuses() const;

private:
	void readUnknownElement();
	void readStatuses();
	void readStatus(QTwitStatus& ts);
	void readInReplyToStatusId(QTwitStatus& ts);
	void readInReplyToUserId(QTwitStatus& ts);
	void readUser(QTwitStatus& ts);
    QString stringError() const;

	QList<QTwitStatus> m_statuses;
};

#endif //XMLREADERSTATUS_H
