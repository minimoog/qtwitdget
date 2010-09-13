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

#ifndef TWEETQMLLISTMODEL_H
#define TWEETQMLLISTMODEL_H

#include <QAbstractListModel>
#include "qtwit/qtwitstatus.h"

class QTwitDestroy;
class QNetworkAccessManager;
class OAuthTwitter;
class QTimer;

class TweetQmlListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int numNewTweets READ numNewTweets RESET resetNumNewTweets NOTIFY numNewTweetsChanged)
public:
    enum TweetRoles {
        ScreenNameRole = Qt::UserRole + 1,
        StatusTextRole,
        AvatarUrlRole,
        StatusIdRole,
        OwnTweetRole
    };

    TweetQmlListModel(QObject *parent = 0);
    void setNetworkAccessManager(QNetworkAccessManager* netManager); // ### TODO: FIX
    void setOAuthTwitter(OAuthTwitter* oauthTwitter);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void update();
    void setUserID(int userid);
    Q_INVOKABLE void destroyTweet(const QString& tweetid);

    int numNewTweets() const;
    void resetNumNewTweets();

    Q_INVOKABLE void showNewTweets();
    void loadTweetsFromDatabase();

    void startUpdateTimelines();

signals:
    void numNewTweetsChanged();

private slots:
    void updateHomeTimeline();
    void finishedDestroyTweet(qint64 id);
    void finishedHomeTimeline(const QList<QTwitStatus>& statuses);

private:
    QNetworkAccessManager* m_netManager;
    OAuthTwitter* m_oauthTwitter;
    QTimer* m_timer;
    QList<QTwitStatus> m_statuses;
    QList<QTwitStatus> m_newStatuses; //doesn't show in the model
    int m_userid;
    int m_numNewTweets;
    int m_numOldTweets;
};

#endif // TWEETQMLLISTMODEL_H
