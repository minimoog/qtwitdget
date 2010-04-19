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

const int TweetsPerPage = 20;

/*!
    Constructor
    \param parent Object parent
 */
TweetListModel::TweetListModel(QObject *parent) :
    ListModelInterface(parent), m_pagecount(1)
{
}

/*!
    \returns number of list items
 */
int TweetListModel::count() const
{
    return m_statuses.count();
}

/*!
    \returns twitter user id
 */
int TweetListModel::userid() const
{
    return m_userid;
}

/*!
    Sets twitter user id
    \param userid user id
 */
void TweetListModel::setUserid(int userid)
{
    m_userid = userid;
}

/*!
    \returns additional query
 */
QString TweetListModel::additionalQuery() const
{
    return m_additionalQuery;
}

/*!
    Sets additional query
    \param query additional query
 */
void TweetListModel::setAdditionalQuery(const QString &query)
{
    m_additionalQuery = query;
}

/*!
    Gets data for given item index
    \param index item index in the list
    \return Data, or default QVariant if index id out of bounds
 */
QVariant TweetListModel::data(int index) const
{
    QVariant s;
    if (index >= 0 && index < m_statuses.count()) {
        s.setValue(m_statuses.at(index));
    }

    return s;
}

/*!
    Gets data for given roles of the item
    \param index Index of the item in the list
    \param roles roles List of roles
    \return data of the requested roles
 */
QHash<QByteArray, QVariant> TweetListModel::data(int index, const QList<QByteArray> &roles) const
{
    QHash<QByteArray,QVariant> hash;

    if (index >= 0 && index < m_statuses.count()) {
        for (int i = 0; i < roles.count(); ++i) {
            hash.insert("isRead", m_statuses.at(index).isRead());
        }
    }

    return hash;
}

/*!
    Updates list items
 */
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
                         "LIMIT %3").arg(topStatusId).arg(additionalQuery()).arg(TweetsPerPage * m_pagecount);
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

        //remove old statuses
        if (m_statuses.count() > TweetsPerPage * m_pagecount) {
            int oldCount = m_statuses.count();

            for (int i = 0; i < oldCount - TweetsPerPage * m_pagecount; ++i)
                m_statuses.removeLast();
        
            emit itemsRemoved(TweetsPerPage * m_pagecount, oldCount - TweetsPerPage * m_pagecount);
        }
    }
}

void TweetListModel::replyDeleteClicked(int index)
{
    if (m_statuses.at(index).userId() != userid()) {
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

/*!
    \returns id of tweet who is oldest unread tweet in the list
 */
qint64 TweetListModel::nextUnread() const
{
    //find oldest unread
    for (int i = m_statuses.count() - 1; i >= 0; --i) {
        if (!m_statuses.at(i).isRead()) {
            //m_statuses[i].setRead(true);
            //emit itemsChanged(i, 1, QList<QByteArray>() << "isRead");

            return m_statuses.at(i).id();
        }
    }
    return 0;
}

/*!
    Marks read tweei in the list
    \param id Tweet id
    \return true if tweet is in the list, false otherwise
 */
bool TweetListModel::markRead(qint64 id)
{
    // ### TODO: Hashing or Map
    for (int i = 0; i < m_statuses.count(); ++i) {
        if (m_statuses.at(i).id() == id) {
            m_statuses[i].setRead(true);
            emit itemsChanged(i, 1, QList<QByteArray>() << "isRead");
            return true;
        }
    }

    return false;
}

/*!
    Marks all read tweets in the list
 */
void TweetListModel::markAllRead()
{
    for (int i = 0; i < m_statuses.count(); ++i) {
        if (!m_statuses.at(i).isRead())
            m_statuses[i].setRead(true);

        emit itemsChanged(i, 1, QList<QByteArray>() << "isRead");
    }
}

/*!
    Add another page of tweets to the list. (Increases number of list items)
 */
void TweetListModel::nextPage()
{
    if (m_statuses.isEmpty())
        return;

    qint64 bottomStatusId = m_statuses.last().id();

    QSqlQuery query;
    QString sq = QString("SELECT id, text, favorited, userId, screenName, profileImageUrl, isRead "
                         "FROM status "
                         "WHERE id < %1 AND %2 "
                         "ORDER BY id DESC "
                         "LIMIT %3").arg(bottomStatusId).arg(additionalQuery()).arg(TweetsPerPage);
    query.exec(sq);

    int index = m_statuses.count();
    int count = 0;

    while (query.next()) {
        QTwitStatus st;
        st.setId(query.value(0).toLongLong());
        st.setText(query.value(1).toString());
        st.setFavorited(query.value(2).toBool());
        st.setUserId(query.value(3).toInt());
        st.setScreenName(query.value(4).toString());
        st.setProfileImageUrl(query.value(5).toString());
        st.setRead(query.value(6).toInt());
        m_statuses << st;
        ++count;
    }

    if (count) {
        emit itemsInserted(index, count);
        m_pagecount += 1;
    }
}
