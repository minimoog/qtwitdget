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

#ifndef QTWITMENTIONS_H
#define QTWITMENTIONS_H

#include <QList>
#include "qtwitbase.h"
#include "qtwitstatus.h"

/*! class QTwitMentions */
class QTwitMentions : public QTwitBase
{
    Q_OBJECT
public:
    /*! Constructor */
    QTwitMentions(QObject *parent = 0);
    /*! Gets mentions
     * \remark Set parameters to zero or default to NOT used them in url query
     * \remark Clears all previous statuses
     */
    void update(qint64 sinceid = 0, qint64 maxid = 0, int count = 0, int page = 0);
    QList<QTwitStatus> getStatuses() const;

private slots:
    void reply();
    void error();

private:
    QList<QTwitStatus> m_statuses;
};

#endif //QTWITMENTIONS_H