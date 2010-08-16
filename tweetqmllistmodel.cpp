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

#include <QSqlQuery>
#include "tweetqmllistmodel.h"

TweetQmlListModel::TweetQmlListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[ScreenNameRole] = "screenNameRole";
    roles[StatusTextRole] = "statusTextRole";
    roles[AvatarUrlRole] = "avatarUrlRole";
    setRoleNames(roles);

    QSqlQuery query;
    query.exec("SELECT id, text, screenName, profileImageUrl FROM status ORDER BY id DESC LIMIT 20");

    while (query.next()) {
        QTwitStatus st;
        st.setId(query.value(0).toLongLong());
        st.setText(query.value(1).toString());
        st.setScreenName(query.value(2).toString());
        st.setProfileImageUrl(query.value(3).toString());
        m_statuses.append(st);
    }
}

void TweetQmlListModel::update()
{
    qint64 topStatusId = 0;

    if (!m_statuses.isEmpty())
        topStatusId = m_statuses.at(0).id();

    QSqlQuery query;
    query.prepare("SELECT id, text, screenName, profileImageUrl "
                  "FROM status "
                  "WHERE id > :id "
                  "ORDER BY id DESC "
                  "LIMIT 20 ");
    query.bindValue(":id", topStatusId);
    query.exec();

    QList<QTwitStatus> newStatuses;

    while (query.next()) {
        QTwitStatus st;
        st.setId(query.value(0).toLongLong());
        st.setText(query.value(1).toString());
        st.setScreenName(query.value(2).toString());
        st.setProfileImageUrl(query.value(3).toString());
        newStatuses.prepend(st);
    }

    if (newStatuses.count()) {

        beginInsertRows(QModelIndex(), 0, newStatuses.count() - 1);

        foreach (const QTwitStatus& s, newStatuses)
            m_statuses.prepend(s);

        endInsertRows();


        if (m_statuses.count() > 20) {
            int oldCount = m_statuses.count();

            beginRemoveRows(QModelIndex(), 20, oldCount - 1);

            for (int i = 0; i < oldCount - 20; ++i)
                m_statuses.removeLast();

            endRemoveRows();
        }
    }
}

int TweetQmlListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_statuses.count();
}

QVariant TweetQmlListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > m_statuses.count())
        return QVariant();

    const QTwitStatus &st = m_statuses.at(index.row());

    if (role == ScreenNameRole)
        return st.screenName();
    else if (role == StatusTextRole)
        return st.text();
    else if (role == AvatarUrlRole)
        return st.profileImageUrl();

    return QVariant();
}
