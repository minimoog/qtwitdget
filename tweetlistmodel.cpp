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

void TweetListModel::fetchNewTweets()
{
    QSqlQuery query;
    QString qr = QString("SELECT id, text, favorited, userId, screenName, profileImageUrl, isRead "
                         "FROM status "
                         "WHERE id > 0 "
                         "ORDER BY id DESC "
                         "LIMIT 20");
    query.exec(qr);

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
    }

    emit itemsInserted(0, m_statuses.count());
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
    // ### TODO
    qDebug() << "retweet clicked";
}

void TweetListModel::favoritedClicked(int index)
{
    // ### TODO
    qDebug() << "favorited clicked";
}

void TweetListModel::setUserid(int userid)
{
    m_userid = userid;
}

int TweetListModel::userid() const
{
    return m_userid;
}
