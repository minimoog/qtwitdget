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
    setRoleNames(roles);

    QSqlQuery query;
    query.exec("SELECT text, screenName FROM status ORDER BY id DESC LIMIT 20");

    while (query.next()) {
        QTwitStatus st;
        st.setText(query.value(0).toString());
        st.setScreenName(query.value(1).toString());
        m_statuses.prepend(st);
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

    return QVariant();
}

void TweetQmlListModel::setStatuses(const QTwitStatus &statuses)
{
    //m_statuses = statuses;
}
