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

#ifndef XMLREADEREXTUSERINFO_H
#define XMLREADEREXTUSERINFO_H

#include <QXmlStreamReader>
#include "qtwit/qtwitextuserinfo.h"

/*!
    Parses XML Twitter extended user info
 */
class XmlReaderExtUserInfo : public QXmlStreamReader
{
public:
	XmlReaderExtUserInfo();
	bool read(QIODevice *device);
	QTwitExtUserInfo userInfo() const;

protected:
	void readUser();

private:
	void readUnknownElement();

	QTwitExtUserInfo m_userInfo;
};

#endif //XMLREADEREXTUSERINFO_H
