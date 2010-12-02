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
#include "oauthtwitter.h"
#include "qtweetstatus.h"
#include "qtweetuser.h"
#include "qtweetstatusdestroy.h"
#include "tweetqmllistmodel.h"
#include "qtweetentityurl.h"
#include "qtweetentityusermentions.h"
#include "qtweetentityhashtag.h"

const int maxTweetsPerView = 200;

TweetQmlListModel::TweetQmlListModel(QObject *parent) :
    QAbstractListModel(parent),
    m_numNewTweets(0),
    m_numUnreadTweets(0)
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

int TweetQmlListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_statuses.count();
}

QVariant TweetQmlListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > m_statuses.count())
        return QVariant();

    const QTweetStatus &st = m_statuses.at(index.row());

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
        if (index.row() < m_numUnreadTweets)
            return true;
        else
            return false;

    return QVariant();
}

void TweetQmlListModel::setUserID(qint64 userid)
{
    m_userid = userid;
}

void TweetQmlListModel::destroyTweet(const QString &tweetid)
{
    qDebug() << "Destroing tweet: " << tweetid;

    bool ok;

    qint64 id = tweetid.toLongLong(&ok);

    if (ok) {
        QTweetStatusDestroy *tweetDestroy = new QTweetStatusDestroy;
        //tweetDestroy->setOAuthTwitter(m_oauthTwitter);
        tweetDestroy->destroy(id);
        connect(tweetDestroy, SIGNAL(deletedStatus(QTweetStatus)), this, SLOT(finishedDestroyTweet(QTweetStatus)));
    }
}

void TweetQmlListModel::finishedDestroyTweet(const QTweetStatus& status)
{
    qDebug() << "Finished destroying: " << status.id();

    QTweetStatusDestroy *tweetDestroy = qobject_cast<QTweetStatusDestroy*>(sender());
    if (tweetDestroy) {
        int i;
        for (i = 0; i < m_statuses.count(); ++i) {
            if (m_statuses.at(i).id() == status.id())
                break;
        }

        beginRemoveRows(QModelIndex(), i, i);

        m_statuses.removeAt(i);

        QSqlQuery query;
        query.prepare("DELETE FROM status WHERE id = :id");
        query.bindValue(":id", status.id());
        query.exec();

        endRemoveRows();

        tweetDestroy->deleteLater();
    }
}

int TweetQmlListModel::numNewTweets() const
{
    return m_numNewTweets;
}

void TweetQmlListModel::resetNumNewTweets()
{
    m_numNewTweets = 0;
}

void TweetQmlListModel::showNewTweets()
{
    if (m_newStatuses.count()) {

        int numReadTweets = m_numUnreadTweets;
        m_numUnreadTweets = m_newStatuses.count();

        //prepend new statuses
        beginInsertRows(QModelIndex(), 0, m_newStatuses.count() - 1);

        QListIterator<QTweetStatus> iter(m_newStatuses);
        iter.toBack();
        while (iter.hasPrevious())
            m_statuses.prepend(iter.previous());

        endInsertRows();

        //remove old statutes
        if (m_statuses.count() > maxTweetsPerView) {
            beginRemoveRows(QModelIndex(), maxTweetsPerView, m_statuses.count());

            for (int i = 0; i < m_statuses.count() - maxTweetsPerView; ++i)
                m_statuses.removeLast();

            endRemoveRows();
        }

        m_numNewTweets = 0;
        emit numNewTweetsChanged();

        m_newStatuses.clear();

        //exp
        QModelIndex first = index(0);
        QModelIndex last = index(m_numUnreadTweets + numReadTweets);
        emit dataChanged(first, last);
    }
}

void TweetQmlListModel::onStatusesStream(const QTweetStatus &status)
{
    QSqlQuery query;

    query.prepare("INSERT OR ABORT INTO status "
                  "(id, text, screenName, profileImageUrl, userId) "
                  "VALUES "
                  "(:id, :text, :screenName, :profileImageUrl, :userId);");
    query.bindValue(":id", status.id());
    query.bindValue(":text", status.text());
    query.bindValue(":userId", status.userid());
    query.bindValue(":screenName", status.user().screenName());
    query.bindValue(":profileImageUrl", status.user().profileImageUrl());
    query.exec();

    m_newStatuses.prepend(status);

    m_numNewTweets = m_newStatuses.count();
    emit numNewTweetsChanged();
}

void TweetQmlListModel::loadTweetsFromDatabase()
{
    QSqlQuery query;
    query.prepare("SELECT id, text, screenName, profileImageUrl, userId "
                  "FROM status "
                  "ORDER BY id DESC "
                  "LIMIT 100 ");
    //query.bindValue(":id", topStatusId);
    query.exec();

    //remove/clear all statuses
    beginResetModel();

    m_statuses.clear();
    m_numUnreadTweets = 0;

    endResetModel();

    QList<QTweetStatus> newStatuses;

    while (query.next()) {
        QTweetStatus st;
        st.setId(query.value(0).toLongLong());
        st.setText(query.value(1).toString());

        QTweetUser userinfo;
        userinfo.setScreenName(query.value(2).toString());
        userinfo.setprofileImageUrl(query.value(3).toString());
        userinfo.setId(query.value(4).toInt());

        st.setUser(userinfo);

        newStatuses.append(st);
    }

    if (newStatuses.count()) {

        beginInsertRows(QModelIndex(), 0, newStatuses.count() - 1);

        m_statuses.append(newStatuses);

        endInsertRows();
    }
}

