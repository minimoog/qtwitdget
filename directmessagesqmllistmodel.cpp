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
#include <QSqlQuery>
#include <QDateTime>
#include "qtweetdirectmessages.h"
#include "directmessagesqmllistmodel.h"
#include "qtweetdmstatus.h"
#include "qtweetuser.h"

const int maxTweetsPerView = 200;

DirectMessagesQmlListModel::DirectMessagesQmlListModel(QObject *parent) :
    QAbstractListModel(parent),
    m_numNewDirectMessages(0),
    m_numUnreadDirectMessages(0)
{
    QHash<int, QByteArray> roles;
    roles[ScreenNameRole] = "screenNameRole";
    roles[StatusTextRole] = "statusTextRole";
    roles[AvatarUrlRole] = "avatarUrlRole";
    roles[StatusIdRole] = "statusIdRole";
    roles[OwnTweetRole] = "ownTweetRole";
    roles[NewTweetRole] = "newTweetRole";
    setRoleNames(roles);
}

int DirectMessagesQmlListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_directMessages.count();
}

QVariant DirectMessagesQmlListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > m_directMessages.count())
        return QVariant();

    const QTweetDMStatus &st = m_directMessages.at(index.row());

    if (role == ScreenNameRole)
        return st.senderScreenName();
    else if (role == StatusTextRole)
        return st.text();
    else if (role == AvatarUrlRole)
        return st.sender().profileImageUrl();
    else if (role == StatusIdRole)
        return QString::number(st.id());
    else if (role == OwnTweetRole)
        if (m_userid != st.senderId())
            return false;
        else
            return true;
    else if (role == NewTweetRole)
        if (index.row() < m_numUnreadDirectMessages)
            return true;
        else
            return false;

    return QVariant();
}

void DirectMessagesQmlListModel::setUserID(qint64 id)
{
    m_userid = id;
}

int DirectMessagesQmlListModel::numNewDirectMessages() const
{
    return m_numNewDirectMessages;
}

void DirectMessagesQmlListModel::resetNumNewDirectMessages()
{
    m_numNewDirectMessages = 0;
}

void DirectMessagesQmlListModel::onDirectMessageStream(const QTweetDMStatus &directMessage)
{
    QSqlQuery query;
    query.prepare("INSERT OR REPLACE INTO directmessages "
                  "(id, senderId, text, recipientId, created, "
                  "senderScreenName, recipientScreenName, senderProfileImageUrl) "
                  "VALUES (:id, :senderId, :text, :recipientId, "
                  ":created, :senderScreenName, :recipientScreenName, :senderProfileImageUrl);");
    query.bindValue(":id", directMessage.id());
    query.bindValue(":senderId", directMessage.senderId());
    query.bindValue(":text", directMessage.text());
    query.bindValue(":recipientId", directMessage.recipientId());
    query.bindValue(":created", directMessage.createdAt());
    query.bindValue(":senderScreenName", directMessage.senderScreenName());
    query.bindValue(":recipientScreenName", directMessage.recipientScreenName());
    query.bindValue(":senderProfileImageUrl", directMessage.sender().profileImageUrl());
    query.exec();

    m_newDirectMessages.prepend(directMessage);

    m_numNewDirectMessages = m_newDirectMessages.count();
    emit numNewDirectMessagesChanged();
}

void DirectMessagesQmlListModel::showNewTweets()
{
    if (m_newDirectMessages.count()) {

        int numReadDirectMessages = m_numUnreadDirectMessages;
        m_numUnreadDirectMessages = m_newDirectMessages.count();

        //prepend new statuses
        beginInsertRows(QModelIndex(), 0, m_newDirectMessages.count() - 1);

        QListIterator<QTweetDMStatus> iter(m_newDirectMessages);
        iter.toBack();
        while (iter.hasPrevious())
            m_directMessages.prepend(iter.previous());

        endInsertRows();

        if (m_directMessages.count() > maxTweetsPerView) {
            beginRemoveRows(QModelIndex(), maxTweetsPerView, m_directMessages.count());

            for (int i = 0; i < m_directMessages.count() - maxTweetsPerView; ++i)
                m_directMessages.removeLast();

            endRemoveRows();
        }

        m_numNewDirectMessages = 0;
        emit numNewDirectMessagesChanged();

        m_newDirectMessages.clear();

        QModelIndex first = index(0);
        QModelIndex last = index(m_numUnreadDirectMessages + numReadDirectMessages);
        emit dataChanged(first, last);
    }
}

void DirectMessagesQmlListModel::loadTweetsFromDatabase()
{
    QSqlQuery query;
    query.prepare("SELECT id, text, senderId, senderScreenName, senderProfileImageUrl "
                  "FROM directmessages "
                  "ORDER BY id DESC "
                  "LIMIT 100 ");
    query.exec();

    //remove/clear all statuses
    beginResetModel();

    m_directMessages.clear();
    m_numUnreadDirectMessages = 0;

    endResetModel();

    QList<QTweetDMStatus> newStatuses;

    while (query.next()) {
        QTweetDMStatus st;
        st.setId(query.value(0).toLongLong());
        st.setText(query.value(1).toString());
        st.setSenderId(query.value(2).toLongLong());
        st.setSenderScreenName(query.value(3).toString());

        //quick fix
        QTweetUser user;
        user.setprofileImageUrl(query.value(4).toString());
        st.setSender(user);

        newStatuses.append(st);
    }

    if (newStatuses.count()) {

        beginInsertRows(QModelIndex(), 0, newStatuses.count() - 1);

        m_directMessages.append(newStatuses);

        endInsertRows();
    }
}

