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
 * Contact e-mail: Antonie Jovanoski <minimoog77@gmail.com>
 */
 
#ifndef XMLREADEREXTUSERS_H
#define XMLREADEREXTUSERS_H

#include "xmlreaderextuserinfo.h"

class XmlReaderExtUsers : public XmlReaderExtUserInfo
{
public:
	XmlReaderExtUsers();
	bool readUsers(QIODevice *device);
	QList<QTwitExtUserInfo> users() const;

private:
	void readUnknownElement();
	void readUsers();

	QList<QTwitExtUserInfo> m_users;
};

#endif //XMLREADEREXTUSERS_H