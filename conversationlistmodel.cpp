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

#include "conversationlistmodel.h"
#include <QDebug>
#include <QDateTime>
#include <QSqlQuery>
#include "qtweetlib/oauthtwitter.h"
#include "qtweetlib/qtweetstatus.h"
#include "qtweetlib/qtweetuser.h"
#include "qtweetlib/qtweetstatusshow.h"

// ### TODO: Inheritance from TweetQmlListModel?

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

ConversationListModel::ConversationListModel(QObject *parent) :
    QAbstractListModel(parent)
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

ConversationListModel::ConversationListModel(OAuthTwitter *oauthTwitter, QObject *parent) :
    QAbstractListModel(parent),
    m_oauthTwitter(oauthTwitter)
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

ConversationListModel::~ConversationListModel()
{
}

void ConversationListModel::setOAuthTwitter(OAuthTwitter *oauthTwitter)
{
    m_oauthTwitter = oauthTwitter;
}

void ConversationListModel::setUserID(qint64 userid)
{
    m_userid = userid;
}

int ConversationListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_statuses.count();
}

QVariant ConversationListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > m_statuses.count())
        return QVariant();

    const QTweetStatus& st = m_statuses.at(index.row());

    if (role == ScreenNameRole)
        return st.user().screenName();
    else if (role == StatusTextRole)
        return st.text();
    else if (role == AvatarUrlRole)
        return st.user().profileImageUrl();
    else if (role == StatusIdRole)
        return QString::number(st.id());
    else if (role == OwnTweetRole)
        if (m_userid != st.user().id())
            return false;
        else
            return true;
    else if (role == NewTweetRole)
        return false;
    else if (role == SinceTimeRole)
        return SinceTimeString(st.createdAt());

    return QVariant();
}

void ConversationListModel::followConversation(qint64 statusID)
{
    beginResetModel();
    m_statuses.clear();
    endResetModel();

    fetchConversation(statusID);
}

void ConversationListModel::followConversation(const QString &statusID)
{
    bool ok;

    qint64 id = statusID.toLongLong(&ok);

    if (ok)
        followConversation(id);
}

void ConversationListModel::fetchConversation(qint64 statusID)
{
    QTweetStatus status = findInDatabase(statusID);

    if (status.id() == 0) {
        //not found in db
        //fetch from twitter
        QTweetStatusShow *statusShow = new QTweetStatusShow(m_oauthTwitter);
        statusShow->fetch(statusID);
        connect(statusShow, SIGNAL(parsedStatus(QTweetStatus)),
                this, SLOT(onParsedStatus(QTweetStatus)));
    } else {
        beginInsertRows(QModelIndex(), m_statuses.count(), m_statuses.count());
        m_statuses.append(status);
        endInsertRows();

        qint64 replyID = status.inReplyToStatusId();

        if (replyID)
            fetchConversation(replyID);
    }
}

QTweetStatus ConversationListModel::findInDatabase(qint64 id)
{
    QSqlQuery query;
    query.prepare("SELECT id, text, screenName, profileImageUrl, userId, created, replyToStatusId "
                  "FROM status "
                  "WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();

    if (query.next()) {
        QTweetStatus st;
        st.setId(query.value(0).toLongLong());
        st.setText(query.value(1).toString());

        QDateTime tempTime = query.value(5).toDateTime();
        QDateTime utcTime(tempTime.date(), tempTime.time(), Qt::UTC);
        st.setCreatedAt(utcTime);

        QTweetUser userinfo;
        userinfo.setScreenName(query.value(2).toString());
        userinfo.setprofileImageUrl(query.value(3).toString());
        userinfo.setId(query.value(4).toLongLong());

        st.setInReplyToStatusId(query.value(6).toLongLong());

        st.setUser(userinfo);

        return st;
    }
    return QTweetStatus();
}

void ConversationListModel::onParsedStatus(const QTweetStatus &status)
{
    QTweetStatusShow *statusShow = qobject_cast<QTweetStatusShow*>(sender());

    if (statusShow) {
        beginInsertRows(QModelIndex(), m_statuses.count(), m_statuses.count());
        m_statuses.append(status);
        endInsertRows();

        qint64 replyID = status.inReplyToStatusId();

        if (replyID)
            fetchConversation(replyID);

        statusShow->deleteLater();
    }
}
