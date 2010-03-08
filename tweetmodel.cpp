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

#include "tweetmodel.h"

TweetModel::TweetModel(QObject *parent)
    : QtListModelInterface(parent)
{

}

int TweetModel::count() const
{
    return m_statuses.count();
}

/*
"FontRole"
"TextAlignmentRole"
"ForegroundRole"
"CheckStateRole"
"DecorationRole"
"DisplayRole"
"SizeHintRole"
*/

QHash<QByteArray,QVariant> TweetModel::data(int index, const QList<QByteArray> &roles) const
{
    QHash<QByteArray,QVariant> hash;
    if (index >= 0 && index < m_statuses.count()) {
        for (int i = 0; i < roles.count(); ++i) {
            if (roles.at(i) == "DisplayRole") {
                QVariant v;
                v.setValue(m_statuses.at(index));
                hash.insert("DisplayRole", v);
            }
        }
    }

    return hash;
}

void TweetModel::fetchNewTweets()
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

TweetModel::~TweetModel()
{
}
