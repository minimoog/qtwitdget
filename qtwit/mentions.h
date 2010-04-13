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

#ifndef MENTIONS_H
#define MENTIONS_H

#include "qtwitmentions.h"

/*!
    Fetches Mentions timeline
 */
class Mentions : public QTwitMentions
{
    Q_OBJECT
public:
    Mentions(QObject *parent = 0);

    void timeline(qint64 sinceid = 0);
    QList<QTwitStatus> statuses() const;

signals:
    /*! Emited when fetching is finished */
    void finishedTimeline();

private slots:
    void finishedFirstRequest(const QList<QTwitStatus>& statuses);
    void finishedSubsequentRequest(const QList<QTwitStatus>& statuses);

private:
    void moreTimeline();

    QList<QTwitStatus> m_statuses;
    qint64 m_sinceid;
    qint64 m_maxid;
};

#endif //MENTIONS_H
