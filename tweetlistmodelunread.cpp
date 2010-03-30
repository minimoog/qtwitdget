/* Copyright (c) 2010, Antonie Jovanoski
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

#include "tweetlistmodelunread.h"

TweetListModelUnread::TweetListModelUnread(QObject *parent) :
    TweetListModel(parent)
{
}

void TweetListModelUnread::update()
{
    qint64 topStatusId = 0;

    if (!m_statuses.isEmpty())
        topStatusId = m_statuses.at(0).id();

    QSqlQuery query;
    //bigger but reasonable limit
    QString qr = QString("SELECT id, text, favorited, userId, screenName, profileImageUrl "
                         "FROM status "
                         "WHERE id > %1 AND isRead = 0 "
                         "ORDER BY id ASC "
                         "LIMIT 50").arg(topStatusId);
    query.exec(qr);

    QList<QTwitStatus> newStatuses;

    while (query.next()) {
        QTwitStatus st;
        st.setId(query.value(0).toLongLong());
        st.setText(query.value(1).toString());
        st.setFavorited(query.value(2).toBool());
        st.setUserId(query.value(3).toInt());
        st.setScreenName(query.value(4).toString());
        st.setProfileImageUrl(query.value(5).toString());
        st.setRead(query.value(6).toInt());
        newStatuses.append(st);
    }

    //batch inserting and removing
    if (newStatuses.count()) {
        foreach (const QTwitStatus& s, newStatuses)
            m_statuses.prepend(s);

        emit itemsInserted(0, newStatuses.count());

        if (m_statuses.count() > 50) {
            int oldCount = m_statuses.count();

            for (int i = 0; i < oldCount - 50; ++i)
                m_statuses.removeLast();

            emit itemsRemoved(50, oldCount - 50);
        }
    }
}

void TweetListModelUnread::nextPage()
{
    //nothing
}

qint64 TweetListModelUnread::nextUnread() const
{
    if (m_statuses.count())
        return m_statuses.last().id();

    return 0;
}

bool TweetListModelUnread::markRead(qint64 id)
{
    // ### TODO: Hashing or Map
    for (int i = 0; i < m_statuses.count(); ++i) {
        if (m_statuses.at(i).id() == id) {
            //don't change just remove it
            m_statuses.removeAt(i);
            emit itemsRemoved(i, 1);

            qint64 topStatusId = 0;

            if (!m_statuses.isEmpty())
                topStatusId = m_statuses.at(0).id();

            QSqlQuery query;
            QString qr = QString("SELECT id, text, favorited, userId, screenName, profileImageUrl "
                                     "FROM status "
                                     "WHERE id > %1 AND isRead = 0 "
                                     "ORDER BY id ASC "
                                     "LIMIT 1").arg(topStatusId);
            query.exec(qr);

            if (query.next()) {
                QTwitStatus st;
                st.setId(query.value(0).toLongLong());
                st.setText(query.value(1).toString());
                st.setFavorited(query.value(2).toBool());
                st.setUserId(query.value(3).toInt());
                st.setScreenName(query.value(4).toString());
                st.setProfileImageUrl(query.value(5).toString());
                m_statuses.prepend(st);
                emit itemsInserted(0, 1);
            }
            return true;
        }
    }
    return false;
}

void TweetListModelUnread::markAllRead()
{
    //remove all
    if (m_statuses.count()) {
        int nRemove = m_statuses.count();
        m_statuses.clear();
        emit itemsRemoved(0, nRemove);
    }
}
