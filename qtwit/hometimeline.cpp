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

#include "hometimeline.h"

const int maxCount = 200;
//twitter API returns under 200 statuses, and this number is reasonable assumption 
//that if you have less then 150 statuses there are no new subsequent statuses  
const int possibleCount = 150;

/*!
    Constructor
 */
HomeTimeline::HomeTimeline(QObject *parent)
    :   QTwitHomeTimeline(parent)
{
}

/*!
    Gets tweets of home timeline
    \return List of tweets
 */
QList<QTwitStatus> HomeTimeline::statuses() const
{
    return m_statuses;
}

/*!
    Starts fetching home timeline
    \param sinceid Fetch timeline tweets from sinceid tweet (newer from sinceid tweet)
    \remarks Clears previous fetched tweets
 */
void HomeTimeline::timeline(qint64 sinceid)
{
    m_sinceid = sinceid;
    m_statuses.clear();

    disconnect(SIGNAL(finishedHomeTimeline(QList<QTwitStatus>)));
    connect(this, SIGNAL(finishedHomeTimeline(QList<QTwitStatus>)), this, SLOT(finishedFirstRequest(QList<QTwitStatus>)));

    update(sinceid, 0, maxCount, 0);
}

void HomeTimeline::finishedFirstRequest(const QList<QTwitStatus>& statuses)
{
    m_statuses.append(statuses);

    if (m_statuses.count() > possibleCount) {
        //get maxid
        m_maxid = m_statuses.last().id();

        disconnect(SIGNAL(finishedHomeTimeline(QList<QTwitStatus>)));
        connect(this, SIGNAL(finishedHomeTimeline(QList<QTwitStatus>)), this, SLOT(finishedSubsequentRequest(QList<QTwitStatus>)));
        update(m_sinceid, m_maxid, maxCount, 0);
    } else {
        emit finishedTimeline();
    }
}

void HomeTimeline::finishedSubsequentRequest(const QList<QTwitStatus>& statuses)
{
    if (statuses.isEmpty()) {
        emit finishedTimeline();
        return;
    }

    //copy
    QList<QTwitStatus> newStatuses(statuses);
    //remove front status (maxid)
    newStatuses.removeFirst();

    if (!newStatuses.isEmpty()) {
        m_statuses.append(newStatuses);

        //get new maxid
        m_maxid = m_statuses.last().id();

        update(m_sinceid, m_maxid, maxCount, 0);
    } else {
        emit finishedTimeline();
    }
}
