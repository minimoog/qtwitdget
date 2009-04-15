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

#ifndef QTWITVERIFYCREDENTIALS_H
#define QTWITVERIFYCREDENTIALS_H
 
#include "qtwitbase.h"
#include "qtwitextuserinfo.h"
 
/*! class QTwitVerifyCredentials */
class QTwitVerifyCredentials : public QTwitBase
{
	Q_OBJECT
	
public:
	/*! Constructor */
	QTwitVerifyCredentials(QObject *parent = 0);
	/*! Verify credentials */
	void verify();
	/*! Gets user info 
	 * \remark Only after verifying 
	 */
	QTwitExtUserInfo userInfo() const;

signals:
	/*! Emitted when verifying credentials is finished, true if are ok, false otherwise */
	void finished(bool valid);
	
private slots:
	void reply();
	void error();

private:
	QTwitExtUserInfo m_userinfo;
};

#endif //QTWITVERIFYCREDENTIALS_H