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
#include "qtweetlib/oauthtwitter.h"
#include "qtweetlib/qtweetstatus.h"
#include "qtweetlib/qtweetuser.h"
#include "qtweetlib/qtweetstatusdestroy.h"
#include "tweetqmllistmodel.h"
#include "qtweetlib/qtweetentityurl.h"
#include "qtweetlib/qtweetentityusermentions.h"
#include "qtweetlib/qtweetentityhashtag.h"
#include "qtweetlib/qtweethometimeline.h"

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
 *  @param parent parent QObject
 */
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
    roles[SinceTimeRole] = "sinceTimeRole";
    setRoleNames(roles);
}

/**
 *  Constructor
 *  @param oauthTwitter oauth twitter authorization object
 *  @param parent parent QObject
 */
TweetQmlListModel::TweetQmlListModel(OAuthTwitter *oauthTwitter, QObject *parent) :
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
    roles[SinceTimeRole] = "sinceTimeRole";
    setRoleNames(roles);

    m_oauthTwitter = oauthTwitter;
}

/**
 *  Sets oauth twitter authorization object
 */
void TweetQmlListModel::setOAuthTwitter(OAuthTwitter *oauthTwitter)
{
    m_oauthTwitter = oauthTwitter;
}

/**
 *  @reimp
 */
int TweetQmlListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_statuses.count();
}

/**
 *  @reimp
 */
QVariant TweetQmlListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > m_statuses.count())
        return QVariant();

    const QTweetStatus &st = m_statuses.at(index.row());

    if (role == ScreenNameRole)
        return st.screenName();
    else if (role == StatusTextRole)
        return st.text();
    else if (role == AvatarUrlRole)
        return st.profileImageUrl();
    else if (role == StatusIdRole)
        return QString::number(st.id());
    else if (role == OwnTweetRole)
        if (m_userid != st.userid())
            return false;
        else
            return true;
    else if (role == NewTweetRole)
        if (index.row() < m_numUnreadTweets)
            return true;
        else
            return false;
    else if (role == SinceTimeRole)
        return SinceTimeString(st.createdAt());

    return QVariant();
}

/**
 *  Sets userid. Used to determine if tweet is user written
 */
void TweetQmlListModel::setUserID(qint64 userid)
{
    m_userid = userid;
}

/**
 *  Returns userid
 */
qint64 TweetQmlListModel::userID() const
{
    return m_userid;
}

/**
 *  Destroys tweet
 *  @param tweetid id number in string of what tweet to be destroyed
 */
void TweetQmlListModel::destroyTweet(const QString &tweetid)
{
    qDebug() << "Destroing tweet: " << tweetid;

    bool ok;

    qint64 id = tweetid.toLongLong(&ok);

    if (ok) {
        QTweetStatusDestroy *tweetDestroy = new QTweetStatusDestroy;
        tweetDestroy->setOAuthTwitter(m_oauthTwitter);
        tweetDestroy->destroy(id);
        connect(tweetDestroy, SIGNAL(deletedStatus(QTweetStatus)), this, SLOT(finishedDestroyTweet(QTweetStatus)));
    }
}

/**
 *  Called when Twitter API destroys the tweet
 */
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

/**
 *  Return number of new tweets
 */
int TweetQmlListModel::numNewTweets() const
{
    return m_numNewTweets;
}

/**
 *  Resets the number of new tweets (0)
 */
void TweetQmlListModel::resetNumNewTweets()
{
    m_numNewTweets = 0;
}

/**
 *  Puts (shows) the new tweets in the model.
 *  This is called when user clicks on hometimeline button to see the new arrived tweets
 */
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

/**
 *  This slot is called from user stream object when new tweet arrives
 *  New tweets are not put to the model, only after user wants to see them
 */
void TweetQmlListModel::onStatusesStream(const QTweetStatus &status)
{
    QSqlQuery query;
    QString text;

    query.prepare("INSERT OR ABORT INTO status "
                  "(id, text, screenName, profileImageUrl, userId, created, replyToStatusId) "
                  "VALUES "
                  "(:id, :text, :screenName, :profileImageUrl, :userId, :created, :replyToStatusId);");
    query.bindValue(":id", status.id());
    query.bindValue(":userId", status.userid());
    query.bindValue(":screenName", status.screenName());
    query.bindValue(":profileImageUrl", status.profileImageUrl());
    query.bindValue(":created", status.createdAt());
    query.bindValue(":replyToStatusId", status.inReplyToStatusId());

    //show/store full retweet text
    if (status.isRetweet()) {
        QString retweetedText = status.retweetedStatus().text();
        text = "RT @" + status.retweetedStatus().screenName() + ": " + retweetedText;
    } else {
        text = status.text();
    }

    query.bindValue(":text", text);

    query.exec();

    QTweetStatus copyStatus(status);
    QString textWithTags = addTags(text);
    copyStatus.setText(textWithTags);

    m_newStatuses.prepend(copyStatus);

    m_numNewTweets = m_newStatuses.count();
    emit numNewTweetsChanged();
}

/**
 * This slot is called from user stream object when tweet is deleted
 */
void TweetQmlListModel::onDeleteStatusStream(qint64 id, qint64 userid)
{
    qDebug() << "Removing tweet id: " << id;
    //search if the tweet is in the list
    for (int i = 0; i < m_statuses.count(); ++i) {
        if (m_statuses.at(i).id() == id) {
            beginRemoveRows(QModelIndex(), i, i);

            m_statuses.removeAt(i);

            QSqlQuery query;
            query.prepare("DELETE FROM status WHERE id = :id");
            query.bindValue(":id", id);
            query.exec();

            endRemoveRows();

            break;
        }
    }
}

/**
 *  Loads 100 last tweets from database
 *  Called at start of application
 */
void TweetQmlListModel::loadTweetsFromDatabase()
{
    QSqlQuery query;
    query.prepare("SELECT id, text, screenName, profileImageUrl, userId, created "
                  "FROM status "
                  "ORDER BY id DESC "
                  "LIMIT 100 ");
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
        st.setText(addTags(query.value(1).toString()));

        //Datetime is stored in UTC
        QDateTime tempTime = query.value(5).toDateTime();
        QDateTime utcTime(tempTime.date(), tempTime.time(), Qt::UTC);
        st.setCreatedAt(utcTime);

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

/**
 *  Fetches last 200 tweets
 */
void TweetQmlListModel::fetchLastTweets()
{
    qint64 lastTweetID = 0;

    QSqlQuery query;
    query.exec("SELECT id FROM status ORDER BY id DESC LIMIT 1");

    if (query.next())
        lastTweetID = query.value(0).toLongLong();

    QTweetHomeTimeline *homeTimeline = new QTweetHomeTimeline(m_oauthTwitter);
    homeTimeline->fetch(lastTweetID, 0, 200);

    connect(homeTimeline, SIGNAL(parsedStatuses(QList<QTweetStatus>)),
            this, SLOT(finishedFetchTweets(QList<QTweetStatus>)));
    connect(homeTimeline, SIGNAL(error(ErrorCode,QString)),
            this, SLOT(errorFetchingTweets()));
}

/**
 *  This slot is called when fetching tweets is finished (see fetchLastTweets())
 */
void TweetQmlListModel::finishedFetchTweets(const QList<QTweetStatus> &statuses)
{
    QTweetHomeTimeline *homeTimeline = qobject_cast<QTweetHomeTimeline*>(sender());

    if (homeTimeline) {
        if (!statuses.isEmpty()) {
            qDebug() << "New statuses: " << statuses.count();
            QSqlQuery query;

            query.exec("BEGIN;");
            query.prepare("INSERT OR ABORT INTO status "
                          "(id, text, screenName, profileImageUrl, userId, created, replyToStatusId) "
                          "VALUES "
                          "(:id, :text, :screenName, :profileImageUrl, :userId, :created, :replyToStatusId);");

            QListIterator<QTweetStatus> i(statuses);
            i.toBack();
            while (i.hasPrevious()) {
                QString text;

                QTweetStatus s = i.previous();
                query.bindValue(":id", s.id());
                query.bindValue(":replyToStatusId", s.inReplyToStatusId());
                query.bindValue(":userId", s.userid());
                query.bindValue(":screenName", s.screenName());
                query.bindValue(":profileImageUrl", s.profileImageUrl());
                query.bindValue(":created", s.createdAt());

                if (s.isRetweet()) {
                    QString retweetedText = s.retweetedStatus().text();
                    text = "RT @" + s.retweetedStatus().screenName() + ": " + retweetedText;
                } else {
                    text = s.text();
                }

                query.bindValue(":text", text);

                query.exec();

                QTweetStatus copys(s);
                QString textWithTags = addTags(text);
                copys.setText(textWithTags);

                m_newStatuses.prepend(copys);
            }
            query.exec("COMMIT;");


            m_numNewTweets = m_newStatuses.count();
            emit numNewTweetsChanged();
        }
        homeTimeline->deleteLater();
    }
}

/**
 *  Called when there is error fetching tweets (see fetchLastTweets())
 */
void TweetQmlListModel::errorFetchingTweets()
{
    QTweetHomeTimeline *homeTimeline = qobject_cast<QTweetHomeTimeline*>(sender());

    if (homeTimeline)
        homeTimeline->deleteLater();
}

void TweetQmlListModel::clear()
{
    if (m_statuses.count()) {
        beginRemoveRows(QModelIndex(), 0, m_statuses.count() - 1);
        m_statuses.clear();
        endRemoveRows();
    }

    m_newStatuses.clear();

    m_numNewTweets = 0;
    m_numUnreadTweets = 0;

    emit numNewTweetsChanged();
}

QString TweetQmlListModel::addTags(const QString &text)
{
    QString tweet(text);
    QString mentions = tweet.replace(QRegExp("(@[a-zA-Z0-9_]+)"), "<b>\\1</b>");
    QString httpLinks = mentions.replace(QRegExp("(\\b(https?|ftp)://[-A-Z0-9+&@#/%?=~_|!:,.;]*[-A-Z0-9+&@#/%=~_|])", Qt::CaseInsensitive),
                                            "<b>\\1</b>");
    QString hashtags = httpLinks.replace(QRegExp("([#]+[A-Za-z0-9-_]+)"),
                                         "<b>\\1</b>");
    return hashtags;
}

/**
 * Destructor
 */
TweetQmlListModel::~TweetQmlListModel()
{
}
