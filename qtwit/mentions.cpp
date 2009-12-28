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

#include "mentions.h"

//just copy pasted from HomeTimeline

const int maxCount = 200;
const int possibleCount = 150;

Mentions::Mentions(QObject *parent)
:   QTwitMentions(parent)
{
}

QList<QTwitStatus> Mentions::statuses() const
{
    return m_statuses;
}

void Mentions::timeline(qint64 sinceid)
{
    m_sinceid = sinceid;
    m_statuses.clear();

    disconnect(SIGNAL(finished()));
    connect(this, SIGNAL(finished()), this, SLOT(finishedFirstRequest()));

    update(sinceid, 0, maxCount, 0);
}

void Mentions::finishedFirstRequest()
{
    QList<QTwitStatus> newStatuses = getStatuses();
    m_statuses.append(newStatuses);

    if (m_statuses.count() > possibleCount) {
        m_maxid = m_statuses.last().id();

        disconnect(SIGNAL(finished()));
        connect(this, SIGNAL(finished()), this, SLOT(finishedSubsequentRequest()));
        update(m_sinceid, m_maxid, maxCount, 0);
    } else {
        emit finishedTimeline();
    }
}

void Mentions::finishedSubsequentRequest()
{
    QList<QTwitStatus> newStatuses = getStatuses();

    if (newStatuses.isEmpty()) {
        emit finishedTimeline();
        return;
    }

    newStatuses.removeFirst();

    if (!newStatuses.isEmpty()) {
        m_statuses.append(newStatuses);

        m_maxid = m_statuses.last().id();

        update(m_sinceid, m_maxid, maxCount, 0);
    } else {
        emit finishedTimeline();
    }
}