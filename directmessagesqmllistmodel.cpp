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
#include <QTimer>
#include <QDateTime>
#include "oauthtwitter.h"
#include "qtweetdirectmessages.h"
#include "directmessagesqmllistmodel.h"
#include "qtweetdmstatus.h"
#include "qtweetuser.h"


DirectMessagesQmlListModel::DirectMessagesQmlListModel(QObject *parent) :
    QAbstractListModel(parent),
    m_numNewDirectMessages(0),
    m_numOldDirectMessages(0),
    m_timer(new QTimer(this))
{
    QHash<int, QByteArray> roles;
    roles[ScreenNameRole] = "screenNameRole";
    roles[StatusTextRole] = "statusTextRole";
    roles[AvatarUrlRole] = "avatarUrlRole";
    roles[StatusIdRole] = "statusIdRole";
    setRoleNames(roles);

    m_timer->setSingleShot(true);
    m_timer->setInterval(300000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateTimeline()));
}

void DirectMessagesQmlListModel::setOAuthTwitter(OAuthTwitter *oauthTwitter)
{
    m_oauthTwitter = oauthTwitter;
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

    return QVariant();
}

void DirectMessagesQmlListModel::setUserID(int userid)
{
    m_userid = userid;
}

int DirectMessagesQmlListModel::numNewDirectMessages() const
{
    return m_numNewDirectMessages;
}

void DirectMessagesQmlListModel::resetNumNewDirectMessages()
{
    m_numNewDirectMessages = 0;
}

void DirectMessagesQmlListModel::updateTimeline()
{
    qDebug() << "Update direct messages";

    QTweetDirectMessages *directMessages = new QTweetDirectMessages;
    directMessages->setOAuthTwitter(m_oauthTwitter);

    if (m_directMessages.isEmpty() && m_newDirectMessages.isEmpty())
        directMessages->fetch(0, 0, 200);
    else if (m_newDirectMessages.isEmpty())
        directMessages->fetch(m_directMessages.at(0).id(), 0, 200);
    else
        directMessages->fetch(m_newDirectMessages.at(0).id(), 0, 200);

    connect(directMessages, SIGNAL(parsedDirectMessages(QList<QTweetDMStatus>)),
            this, SLOT(finishedTimeline(QList<QTweetDMStatus>)));
    connect(directMessages, SIGNAL(parsedDirectMessages(QList<QTweetDMStatus>)),
            this, SLOT(error()));
}

void DirectMessagesQmlListModel::finishedTimeline(const QList<QTweetDMStatus> &statuses)
{
    qDebug() << "Finished update direct messages";

    QTweetDirectMessages *directMessages = qobject_cast<QTweetDirectMessages*>(sender());
    if (directMessages) {
        if (!statuses.isEmpty()) {

            qDebug() << "New direct messages: " << statuses.count();

            //store to database
            QSqlQuery query;

            query.exec("BEGIN;");

            query.prepare("INSERT OR REPLACE INTO directmessages "
                          "(id, senderId, text, recipientId, created, "
                          "senderScreenName, recipientScreenName, senderProfileImageUrl) "
                          "VALUES (:id, :senderId, :text, :recipientId, "
                          ":created, :senderScreenName, :recipientScreenName, :senderProfileImageUrl);");

            QListIterator<QTweetDMStatus> i(statuses);
            i.toBack();
            while(i.hasPrevious()){
                QTweetDMStatus s = i.previous();
                query.bindValue(":id", s.id());
                query.bindValue(":senderId", s.senderId());
                query.bindValue(":text", s.text());
                query.bindValue(":recipientId", s.recipientId());
                query.bindValue(":created", s.createdAt());
                query.bindValue(":senderScreenName", s.senderScreenName());
                query.bindValue(":recipientScreenName", s.recipientScreenName());
                query.bindValue(":senderProfileImageUrl", s.sender().profileImageUrl());

                query.exec();

                m_newDirectMessages.prepend(s);

                m_numNewDirectMessages = m_newDirectMessages.count();
                emit numNewDirectMessagesChanged();
            }

            query.exec("COMMIT;");
        }

        directMessages->deleteLater();
    }

    m_timer->start();
}

void DirectMessagesQmlListModel::error()
{
    m_timer->start();
}

void DirectMessagesQmlListModel::showNewTweets()
{
    if (m_newDirectMessages.count()) {
        int numTweets = m_directMessages.count();

        //prepend new statuses
        beginInsertRows(QModelIndex(), 0, m_newDirectMessages.count() - 1);

        QListIterator<QTweetDMStatus> iter(m_newDirectMessages);
        iter.toBack();
        while (iter.hasPrevious())
            m_directMessages.prepend(iter.previous());

        endInsertRows();

        //remove old statutes
        if (m_numOldDirectMessages) {
            beginRemoveRows(QModelIndex(), numTweets + m_numNewDirectMessages - m_numOldDirectMessages, numTweets + m_numNewDirectMessages - 1);

            for (int i = 0; i < m_numOldDirectMessages; ++i)
                m_directMessages.removeLast();

            endRemoveRows();
        }

        m_numOldDirectMessages = m_directMessages.count() - m_numNewDirectMessages;

        qDebug() << "Num old direct messages: " << m_numOldDirectMessages;

        m_numNewDirectMessages = 0;
        emit numNewDirectMessagesChanged();

        m_newDirectMessages.clear();
    }
}

void DirectMessagesQmlListModel::loadTweetsFromDatabase()
{
    QSqlQuery query;
    query.prepare("SELECT id, text, senderId, senderScreenName, senderProfileImageUrl "
                  "FROM directmessages "
                  "ORDER BY id DESC "
                  "LIMIT 20 ");
    query.exec();

    //remove/clear all statuses
    beginResetModel();

    m_directMessages.clear();
    m_numOldDirectMessages = 0;

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

void DirectMessagesQmlListModel::startUpdateTimelines()
{
    updateTimeline();
}

