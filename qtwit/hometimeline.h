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

#ifndef HOMETIMELINE_H
#define HOMETIMELINE_H

#include "qtwithometimeline.h"

/*!
    Fetches Twitter home timeline
 */
class HomeTimeline : public QTwitHomeTimeline
{
    Q_OBJECT
public:
    HomeTimeline(QObject *parent = 0);

    void timeline(qint64 sinceid = 0);
    QList<QTwitStatus> statuses() const;

signals:
    /*! Emited when fetching of tweets is finished */
    void finishedTimeline();

private slots:
    void finishedFirstRequest();
    void finishedSubsequentRequest();

private:
    void moreTimeline();

    QList<QTwitStatus> m_statuses;
    qint64 m_sinceid;
    qint64 m_maxid;
};

#endif //HOMETIMELINE_H
