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

#include <QtDebug>
#include "tweetlistmodel.h"

TweetListModel::TweetListModel(QObject *parent) :
    QObject(parent)
{
}

int TweetListModel::count() const
{
    return m_statuses.count();
}

QVariant TweetListModel::data(int index)
{
    QVariant s;
    if (index >= 0 && index < m_statuses.count()) {
        s.setValue(m_statuses.at(index));
    }

    return s;
}

void TweetListModel::update()
{
    qint64 topStatusId = 0;

    if (!m_statuses.isEmpty())
        topStatusId = m_statuses.at(0).id();

    QSqlQuery query;
    QString qr = QString("SELECT id, text, favorited, userId, screenName, profileImageUrl, isRead "
                         "FROM status "
                         "WHERE id > %1 AND %2"
                         "ORDER BY id DESC "
                         "LIMIT 20").arg(topStatusId).arg(m_additionalQuery);
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
        newStatuses.prepend(st);    //reverse order
    }

    //batch inserting and removing
    if (newStatuses.count()) {
        foreach (const QTwitStatus& s, newStatuses)
            m_statuses.prepend(s);

        emit itemsInserted(0, newStatuses.count());

        if (m_statuses.count() > 20) {
            int oldCount = m_statuses.count();

            for (int i = 0; i < m_statuses.count() - 20; ++i)
                m_statuses.removeLast();
        
            emit itemsRemoved(20, oldCount - 20);
        }
    }
}

void TweetListModel::replyDeleteClicked(int index)
{
    if (m_statuses.at(index).userId() != m_userid) {
        //reply
        emit requestReply(m_statuses.at(index).id(), m_statuses.at(index).screenName());
    } else {
        //delete own
        emit requestDelete(m_statuses.at(index).id());
        m_statuses.removeAt(index);
        emit itemsRemoved(index, 1);
    }
}

void TweetListModel::retweetClicked(int index)
{
    emit requestRetweet(m_statuses.at(index).id());
}

void TweetListModel::favoritedClicked(int index)
{
    emit requestFavorited(m_statuses.at(index).id());
}

void TweetListModel::setUserid(int userid)
{
    m_userid = userid;
}

int TweetListModel::userid() const
{
    return m_userid;
}

void TweetListModel::setAdditionalQuery(const QString &query)
{
    m_additionalQuery = query;
}

QString TweetListModel::additionalQuery() const
{
    return m_additionalQuery;
}


