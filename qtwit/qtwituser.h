/* Copyright (c) 2008, Antonie Jovanoski
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

#ifndef QTWITUSER_H
#define QTWITUSER_H

#include <QString>

/* QTwitUser class
 *
 * \remark Not fully implemented and used
 */
class QTwitUser
{
public:
	QTwitUser() {};

	void setId(int id) { m_id = id; }
	int getId() const { return m_id; }

	void setScreenName(QString &screenName) { m_screenName = screenName; }
	QString getScreenName() const { return m_screenName; }

	void setProfileImageUrl(QString &imageUrl) { m_profileImageUrl = imageUrl; }
	QString getProfileImageUrl() const { return m_profileImageUrl; }

private:
	int m_id;
	QString m_name;				//not implemented
	QString m_screenName;
	QString m_location;			//not implemented
	QString m_description;		//not implemented
	QString m_profileImageUrl;	
	QString m_url;				//not implemented
	bool m_protected;			//not implemented
	int m_followersCount;		//not implemented
};

#endif //QTWITUSER_H