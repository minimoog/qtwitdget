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
#include "qtweetlib/qtweetstatusdestroy.h"
#include "qtweetlib/qtweetstatus.h"
#include "qtweetlib/qtweetuser.h"
#include "qtweetlib/qtweetentityusermentions.h"
#include "qtweetlib/qtweetmentions.h"
#include "mentionsqmllistmodel.h"

/**
 * Constructor
 * @param parent parent QObject
 */
MentionsQmlListModel::MentionsQmlListModel(QObject *parent) :
    TweetQmlListModel(parent)
{
}

/**
 *  Constructor
 *  @param oauthTwitter oauth twitter authorization object
 *  @param parent parent QObject
 */
MentionsQmlListModel::MentionsQmlListModel(OAuthTwitter *oauthTwitter, QObject *parent) :
    TweetQmlListModel(oauthTwitter, parent)
{

}

/**
 * This slot is connected to user stream object.
 * Called when new tweet arrives in the user stream
 */
void MentionsQmlListModel::onStatusesStream(const QTweetStatus &status)
{
    QList<QTweetEntityUserMentions> entityUserMentions = status.userMentionsEntities();

    for (int i = 0; i < entityUserMentions.count(); ++i) {
        if (entityUserMentions.at(i).userid() == userID()) {    //check if is mention
            QSqlQuery query;
            QString text;

            query.prepare("INSERT OR REPLACE INTO status "
                          "(id, text, screenName, profileImageUrl, userId, mention, created, replyToStatusId) "
                          "VALUES "
                          "(:id, :text, :screenName, :profileImageUrl, :userId, :mention, :created, :replyToStatusId);");
            query.bindValue(":id", status.id());
            query.bindValue(":userId", status.userid());
            query.bindValue(":screenName", status.screenName());
            query.bindValue(":profileImageUrl", status.profileImageUrl());
            query.bindValue(":mention", 1);
            query.bindValue(":created", status.createdAt());
            query.bindValue(":replyToStatusId", status.inReplyToStatusId());

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
            break;
        }
    }
}

/**
 *  Load last 100 tweets mentions from database
 *  @reimp
 */
void MentionsQmlListModel::loadTweetsFromDatabase()
{
    QSqlQuery query;
    query.prepare("SELECT id, text, screenName, profileImageUrl, userId, created "
                  "FROM status "
                  "WHERE mention = 1 "
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
 *  Fetches last 200 mentions.
 *  @reimp
 */
void MentionsQmlListModel::fetchLastTweets()
{
    qint64 lastMentionID = 0;

    QSqlQuery query;
    query.exec("SELECT id FROM status WHERE mention == 1 ORDER BY id DESC LIMIT 1");

    if (query.next())
        lastMentionID = query.value(0).toLongLong();

    QTweetMentions *mentions = new QTweetMentions(m_oauthTwitter);

    mentions->fetch(lastMentionID, 0, 200);

    connect(mentions, SIGNAL(parsedStatuses(QList<QTweetStatus>)),
            this, SLOT(finishedFetchTweets(QList<QTweetStatus>)));
    connect(mentions, SIGNAL(error(ErrorCode,QString)),
            this, SLOT(errorFetchingTweets()));
}

/**
 *  This slot is called when fetching mentions is finished (see fetchLastTweets())
 */
void MentionsQmlListModel::finishedFetchTweets(const QList<QTweetStatus> &statuses)
{
    QTweetMentions *mentions = qobject_cast<QTweetMentions*>(sender());

    if (mentions) {
        if (!statuses.isEmpty()) {
            qDebug() << "Fetch mentions: " << statuses.count();

            QSqlQuery query;

            query.exec("BEGIN;");
            query.prepare("INSERT OR REPLACE INTO status "
                          "(id, text, screenName, profileImageUrl, userId, mention, created, replyToStatusId) "
                          "VALUES "
                          "(:id, :text, :screenName, :profileImageUrl, :userId, :mention, :created, :replyToStatusId);");

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
                query.bindValue(":mention", 1);
                query.bindValue(":created", s.createdAt());

                if (s.isRetweet()) {
                    QString retweetedText = s.retweetedStatus().text();
                    text = "RT @" + s.retweetedStatus().screenName() + ": " + retweetedText;
                } else {
                    text = s.text();
                }

                query.bindValue(":text", text);

                query.exec();

                QTweetStatus sc(s);
                QString textWithTags = addTags(text);
                sc.setText(textWithTags);

                m_newStatuses.prepend(sc);
            }
            query.exec("COMMIT;");

            m_numNewTweets = m_newStatuses.count();
            emit numNewTweetsChanged();
        }
        mentions->deleteLater();
    }
}

/**
 *  Called when there is error fetching tweets (see fetchLastTweets())
 */
void MentionsQmlListModel::errorFetchingTweets()
{
    QTweetMentions *mentions = qobject_cast<QTweetMentions*>(sender());

    if (mentions)
        mentions->deleteLater();
}
