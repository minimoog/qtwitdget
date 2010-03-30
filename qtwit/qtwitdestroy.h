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
 
#ifndef QTWITDESTROY_H
#define QTWITDESTROY_H

#include "qtwitbase.h"
 
/*!
    Deletes tweets with given id
 */
class QTwitDestroy : public QTwitBase
{
	Q_OBJECT	
public:
	QTwitDestroy(QObject *parent = 0);

public slots:
	void deleteStatus(qint64 id);
	
signals:
	/*! emitted when status with id is deleted */
	void destroyed(qint64 id);

private slots:
	void reply();
	void error();

private:
	qint64 m_id;
};
 
#endif //QTWITDESTROY_H
