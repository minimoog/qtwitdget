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
#include <QNetworkAccessManager>
#include <QTimer>
#include <oauth/oauthtwitter.h>
#include <qtwit/qtwitdestroy.h>
#include <qtwit/qtwithometimeline.h>
#include "tweetqmllistmodel.h"


TweetQmlListModel::TweetQmlListModel(QObject *parent) :
    QAbstractListModel(parent),
    m_numNewTweets(0),
    m_numOldTweets(0),
    m_timer(new QTimer(this))
{
    QHash<int, QByteArray> roles;
    roles[ScreenNameRole] = "screenNameRole";
    roles[StatusTextRole] = "statusTextRole";
    roles[AvatarUrlRole] = "avatarUrlRole";
    roles[StatusIdRole] = "statusIdRole";
    roles[OwnTweetRole] = "ownTweetRole";
    setRoleNames(roles);

    m_timer->setSingleShot(true);
    m_timer->setInterval(60000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateHomeTimeline()));
}

void TweetQmlListModel::setNetworkAccessManager(QNetworkAccessManager *netManager)
{
    m_netManager = netManager;
}

void TweetQmlListModel::setOAuthTwitter(OAuthTwitter *oauthTwitter)
{
    m_oauthTwitter = oauthTwitter;
}

void TweetQmlListModel::update()
{

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
    else if (role == StatusIdRole)
        return QString::number(st.id());
    else if (role == OwnTweetRole)
        if (m_userid != st.userId())
            return false;
        else
            return true;

    return QVariant();
}

void TweetQmlListModel::setUserID(int userid)
{
    m_userid = userid;
}

void TweetQmlListModel::destroyTweet(const QString &tweetid)
{
    qDebug() << "Destroing tweet: " << tweetid;

    bool ok;

    qint64 id = tweetid.toLongLong(&ok);

    if (ok) {
        QTwitDestroy *twitDestroy = new QTwitDestroy;
        twitDestroy->setNetworkAccessManager(m_netManager);
        twitDestroy->setOAuthTwitter(m_oauthTwitter);
        twitDestroy->deleteStatus(id);
        connect(twitDestroy, SIGNAL(destroyed(qint64)), this, SLOT(finishedDestroyTweet(qint64)));
    }
}

void TweetQmlListModel::finishedDestroyTweet(qint64 id)
{
    qDebug() << "Finished destroying: " << id;
    QTwitDestroy *twitDestroy = qobject_cast<QTwitDestroy*>(sender());

    if (twitDestroy) {
        // ### TODO: remove from model
        int i;
        for (i = 0; i < m_statuses.count(); ++i) {
            if (m_statuses.at(i).id() == id)
                break;
        }

        beginRemoveRows(QModelIndex(), i, i);

        m_statuses.removeAt(i);

        QSqlQuery query;
        query.prepare("DELETE FROM status WHERE id = :id");
        query.bindValue(":id", id);
        query.exec();

        endRemoveRows();

        twitDestroy->deleteLater();
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

void TweetQmlListModel::updateHomeTimeline()
{
    qDebug() << "Update home timeline";

    QTwitHomeTimeline *twitHomeTimeline = new QTwitHomeTimeline;
    twitHomeTimeline->setNetworkAccessManager(m_netManager);
    twitHomeTimeline->setOAuthTwitter(m_oauthTwitter);
    twitHomeTimeline->update(m_statuses.at(0).id(), 0, 200);
    connect(twitHomeTimeline, SIGNAL(finishedHomeTimeline(QList<QTwitStatus>)),
            this, SLOT(finishedHomeTimeline(QList<QTwitStatus>)));
}

void TweetQmlListModel::finishedHomeTimeline(const QList<QTwitStatus> &statuses)
{
    qDebug() << "Finished update hometimeline";

    QTwitHomeTimeline *twitHomeTimeline = qobject_cast<QTwitHomeTimeline*>(sender());

    if (twitHomeTimeline) {
        if (!statuses.isEmpty()) {

            qDebug() << "New statuses: " << statuses.count();

            //store to database
            QSqlQuery query;

            query.exec("BEGIN;");

            query.prepare("INSERT OR ABORT INTO status "
                          "(id, text, screenName, profileImageUrl, userId) "
                          "VALUES "
                          "(:id, :text, :screenName, :profileImageUrl, :userId);");

            QListIterator<QTwitStatus> i(statuses);
            i.toBack();
            while(i.hasPrevious()){
                QTwitStatus s = i.previous();
                query.bindValue(":id", s.id());
                query.bindValue(":text", s.text());
                //query.bindValue(":replyToStatusId", s.replyToStatusId());
                //query.bindValue(":replyToUserId", s.replyToUserId());
                //query.bindValue(":replyToScreenName", s.replyToScreenName());
                query.bindValue(":userId", s.userId());
                query.bindValue(":screenName", s.screenName());
                query.bindValue(":profileImageUrl", s.profileImageUrl());

                query.exec();

                m_newStatuses.prepend(s);

                m_numNewTweets = m_newStatuses.count();
                emit numNewTweetsChanged();
            }

            query.exec("COMMIT;");
        }

        twitHomeTimeline->deleteLater();
    }

    m_timer->start();   //u
}

void TweetQmlListModel::showNewTweets()
{
    if (m_newStatuses.count()) {
        int numTweets = m_statuses.count();

        //prepend new statuses
        beginInsertRows(QModelIndex(), 0, m_newStatuses.count() - 1);

        QListIterator<QTwitStatus> iter(m_newStatuses);
        iter.toBack();
        while (iter.hasPrevious())
            m_statuses.prepend(iter.previous());

        endInsertRows();

        //remove old statutes
        beginRemoveRows(QModelIndex(), numTweets + m_numNewTweets - m_numOldTweets, numTweets + m_numNewTweets - 1);

        for (int i = 0; i < m_numOldTweets; ++i)
            m_statuses.removeLast();

        endRemoveRows();

        m_numOldTweets = m_statuses.count() - m_numNewTweets;

        m_numNewTweets = 0;
        emit numNewTweetsChanged();

        m_newStatuses.clear();
    }
}

void TweetQmlListModel::loadTweetsFromDatabase()
{
    QSqlQuery query;
    query.prepare("SELECT id, text, screenName, profileImageUrl, userId "
                  "FROM status "
                  "ORDER BY id DESC "
                  "LIMIT 20 ");
    //query.bindValue(":id", topStatusId);
    query.exec();

    //remove/clear all statuses
    beginResetModel();

    m_statuses.clear();
    m_numOldTweets = 0;

    endResetModel();

    QList<QTwitStatus> newStatuses;

    while (query.next()) {
        QTwitStatus st;
        st.setId(query.value(0).toLongLong());
        st.setText(query.value(1).toString());
        st.setScreenName(query.value(2).toString());
        st.setProfileImageUrl(query.value(3).toString());
        st.setUserId(query.value(4).toInt());
        newStatuses.append(st);
    }

    if (newStatuses.count()) {

        beginInsertRows(QModelIndex(), 0, newStatuses.count() - 1);

        m_statuses.append(newStatuses);

        endInsertRows();
    }
}

void TweetQmlListModel::startUpdateTimelines()
{
    updateHomeTimeline();
}
