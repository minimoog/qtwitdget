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
#include "qtweetlib/qtweetdirectmessages.h"
#include "directmessagesqmllistmodel.h"
#include "qtweetlib/qtweetdmstatus.h"
#include "qtweetlib/qtweetuser.h"

const int maxTweetsPerView = 200;

static QString SinceTimeString(const QDateTime& from)
{
    int passedSeconds = from.secsTo(QDateTime::currentDateTimeUtc());

    if (passedSeconds < 0)
        return QString("Time travel!");

    if (passedSeconds < 60)
        return QString("%1 seconds ago").arg(passedSeconds);

    if (passedSeconds < 3600)
        return QString("%1 minutes ago").arg(passedSeconds / 60);

    if (passedSeconds < 86400)
        return QString("%1 hours ago").arg(passedSeconds / 3600);

    return QString("%1 days ago").arg(passedSeconds / 86400);
}

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
    roles[SinceTimeRole] = "sinceTimeRole";
    setRoleNames(roles);
}

DirectMessagesQmlListModel::DirectMessagesQmlListModel(OAuthTwitter *oauthTwitter, QObject *parent) :
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
    roles[SinceTimeRole] = "sinceTimeRole";
    setRoleNames(roles);

    m_oauthTwitter = oauthTwitter;
}

int DirectMessagesQmlListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_directMessages.count();
}

void DirectMessagesQmlListModel::setOAuthTwitter(OAuthTwitter *oauthTwitter)
{
    m_oauthTwitter = oauthTwitter;
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
    else if (role == SinceTimeRole)
        return SinceTimeString(st.createdAt());

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
    query.prepare("SELECT id, text, senderId, senderScreenName, senderProfileImageUrl, created "
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

        //Datetime is stored in UTC
        QDateTime tempTime = query.value(5).toDateTime();
        QDateTime utcTime(tempTime.date(), tempTime.time(), Qt::UTC);
        st.setCreatedAt(utcTime);

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

void DirectMessagesQmlListModel::fetchLastTweets()
{
    qint64 lastDirectMessageID = 0;

    QSqlQuery query;
    query.exec("SELECT id FROM directmessages ORDER BY id DESC LIMIT 1");

    if (query.next())
        lastDirectMessageID = query.value(0).toLongLong();

    QTweetDirectMessages *directMessages = new QTweetDirectMessages(m_oauthTwitter);
    directMessages->fetch(lastDirectMessageID, 0, 200);

    connect(directMessages, SIGNAL(parsedDirectMessages(QList<QTweetDMStatus>)),
            this, SLOT(finishedFetchDirectMessages(QList<QTweetDMStatus>)));
    connect(directMessages, SIGNAL(error(ErrorCode,QString)),
            this, SLOT(errorFetchingDirectMessages()));
}

void DirectMessagesQmlListModel::finishedFetchDirectMessages(const QList<QTweetDMStatus> &dmStatuses)
{
    QTweetDirectMessages *directMessages = qobject_cast<QTweetDirectMessages*>(sender());
    if (directMessages) {
        if (!dmStatuses.isEmpty()) {
            QSqlQuery query;

            query.exec("BEGIN;");
            query.prepare("INSERT OR REPLACE INTO directmessages "
                          "(id, senderId, text, created, senderScreenName, senderProfileImageUrl) "
                          "VALUES "
                          "(:id, :senderId, :text, :created, :senderScreenName,:senderProfileImageUrl);");

            QListIterator<QTweetDMStatus> i(dmStatuses);
            i.toBack();
            while(i.hasPrevious()){
                QTweetDMStatus s = i.previous();
                query.bindValue(":id", s.id());
                query.bindValue(":senderId", s.senderId());
                query.bindValue(":text", s.text());
                query.bindValue(":created", s.createdAt());
                query.bindValue(":senderScreenName", s.senderScreenName());
                query.bindValue(":senderProfileImageUrl", s.sender().profileImageUrl());
                query.exec();

                m_newDirectMessages.prepend(s);
            }
            query.exec("COMMIT;");

            m_numNewDirectMessages = m_newDirectMessages.count();
            emit numNewDirectMessagesChanged();
        }
        directMessages->deleteLater();
    }
}

void DirectMessagesQmlListModel::errorFetchingDirectMessages()
{
    QTweetDirectMessages *directMessages = qobject_cast<QTweetDirectMessages*>(sender());

    if (directMessages)
        directMessages->deleteLater();
}
