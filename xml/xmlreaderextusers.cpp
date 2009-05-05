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

#include "xmlreaderextusers.h"

XmlReaderExtUsers::XmlReaderExtUsers()
{
}

QList<QTwitExtUserInfo> XmlReaderExtUsers::users() const
{
	return m_users;
}

bool XmlReaderExtUsers::readUsers(QIODevice *device)
{
	setDevice(device);

	while(!atEnd()){
		readNext();

		if(isStartElement()){
			if(name() == "users")
				readUsers();
			else
				raiseError(QObject::tr("parse error"));
		}
	}

	return !error();
}

void XmlReaderExtUsers::readUnknownElement()
{
	while(!atEnd()){
		readNext();

		if(isEndElement())
			break;

		if(isStartElement())
			readUnknownElement();
	}
}

void XmlReaderExtUsers::readUsers()
{
	while(!atEnd()){
		readNext();

		if(isEndElement())
			break;

		if(isStartElement()){
			if(name() == "user"){
				readUser();
				m_users << userInfo();
			} else {
				readUnknownElement();
			}
		}
	}
}