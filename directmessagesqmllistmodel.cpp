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
#include <QRegExp>
#include "qtweetlib/qtweetdirectmessages.h"
#include "directmessagesqmllistmodel.h"
#include "qtweetlib/qtweetdmstatus.h"
#include "qtweetlib/qtweetuser.h"

const int maxTweetsPerView = 200;

static QString SinceTimeString(const QDateTime& from)
{
    int passedSeconds = from.secsTo(QDateTime::currentDateTimeUtc());

    if (passedSeconds < 0)
        return QString("now");

    if (passedSeconds < 60)
        return QString("%1 s").arg(passedSeconds);

    if (passedSeconds < 3600)
        return QString("%1 m").arg(passedSeconds / 60);

    if (passedSeconds < 86400)
        return QString("%1 h").arg(passedSeconds / 3600);

    return QString("%1 d").arg(passedSeconds / 86400);
}

/**
 *  Constructor
 */
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

/**
 *  Constructor
 */
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

/**
 *  @reimp
 */
int DirectMessagesQmlListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_directMessages.count();
}

void DirectMessagesQmlListModel::setOAuthTwitter(OAuthTwitter *oauthTwitter)
{
    m_oauthTwitter = oauthTwitter;
}

/**
 *  @reimp
 */
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

/**
 *  Sets id for the authenticated user
 */
void DirectMessagesQmlListModel::setUserID(qint64 id)
{
    m_userid = id;
}

/**
 *  Returns the number of new DM
 */
int DirectMessagesQmlListModel::numNewDirectMessages() const
{
    return m_numNewDirectMessages;
}

/**
 *  Resets to 0 number of DM
 */
void DirectMessagesQmlListModel::resetNumNewDirectMessages()
{
    m_numNewDirectMessages = 0;
}

/**
 *  Stream receiver for DM
 */
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

    QTweetDMStatus copyDirectMessage(directMessage);
    QString textWithTags = addTags(directMessage.text());
    copyDirectMessage.setText(textWithTags);

    m_newDirectMessages.prepend(copyDirectMessage);

    m_numNewDirectMessages = m_newDirectMessages.count();
    emit numNewDirectMessagesChanged();
}

/**
 *  Show new DM's in the model
 */
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

/**
 *  Loads DM's from database, used at the start up of the application
 */
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
        st.setText(addTags(query.value(1).toString()));
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

/**
 *  Fetches the last DM
 */

//## TODO: Rename?
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

                QTweetDMStatus copyDirectMessage(s);
                QString textWithTags = addTags(s.text());
                copyDirectMessage.setText(textWithTags);

                m_newDirectMessages.prepend(copyDirectMessage);
            }
            query.exec("COMMIT;");

            m_numNewDirectMessages = m_newDirectMessages.count();
            emit numNewDirectMessagesChanged();
        }
        directMessages->deleteLater();
    }
}

/**
 *  Clears the model
 */
void DirectMessagesQmlListModel::clear()
{
    if (m_directMessages.count()) {
        beginRemoveRows(QModelIndex(), 0, m_directMessages.count() - 1);
        m_directMessages.clear();
        endRemoveRows();
    }

    m_newDirectMessages.clear();

    m_numNewDirectMessages = 0;
    m_numUnreadDirectMessages = 0;

    emit numNewDirectMessagesChanged();
}

void DirectMessagesQmlListModel::errorFetchingDirectMessages()
{
    QTweetDirectMessages *directMessages = qobject_cast<QTweetDirectMessages*>(sender());

    if (directMessages)
        directMessages->deleteLater();
}

QString DirectMessagesQmlListModel::addTags(const QString &text)
{
    QString tweet(text);
    QString mentions = tweet.replace(QRegExp("(@[a-zA-Z0-9_]+)"), "<a href=\"mention://\\1\">\\1</a>");
    QString httpLinks = mentions.replace(QRegExp("(\\b(https?|ftp)://[-A-Z0-9+&@#/%?=~_|!:,.;]*[-A-Z0-9+&@#/%=~_|])", Qt::CaseInsensitive),
                                        "<a href='\\1'>\\1</a>");
    QString hashtags = httpLinks.replace(QRegExp("([#]+[A-Za-z0-9-_]+)"), "<a href=\"tag://\\1\">\\1</a>");
    return hashtags;
}

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
