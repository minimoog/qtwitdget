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

const int pagelimit = 16;
const int maxCount = 200;

HomeTimeline::HomeTimeline(QObject *parent)
    :   QTwitHomeTimeline(parent), m_page(0)
{
    connect(this, SIGNAL(finished()), this, SLOT(finishedPage()));
}

QList<QTwitStatus> HomeTimeline::statuses() const
{
    return m_statuses;
}

void HomeTimeline::timeline(qint64 sinceid)
{
    m_sinceid = sinceid;
    m_page = 0;
    m_statuses.clear();

    update(sinceid, 0, maxCount, m_page);
}

void HomeTimeline::finishedPage()
{
    QList<QTwitStatus> newStatuses = getStatuses();
    m_statuses.append(newStatuses);

    if (newStatuses.count() == maxCount && m_page < pagelimit) {
        m_page += 1;
        update(m_sinceid, 0, maxCount, m_page);
    } else {
        emit finishedTimeline();
    }
}

