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
 
#ifndef QTWITFAVORITES_H
#define QTWITFAVORITES_H

#include "qtwitbase.h"

/*!
    Favorites/unfavorites tweet with given id
 */
class QTwitFavorites : public QTwitBase
{
	Q_OBJECT
public:
	QTwitFavorites(QObject *parent = 0);
	void create(qint64 id);
	void destroy(qint64 id);

signals:
    /*!
        Emited when done creating/destroing favoriting
        \param id id of the tweet
        \param created true if is favorited, false if is unfavorited
     */
	void finished(qint64 id, bool created);

private slots:
	void reply();
	void error();
};

#endif //QTWITFAVORITES_H
