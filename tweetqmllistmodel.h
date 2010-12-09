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

class QTweetStatus;
class QTweetNetBase;
class OAuthTwitter;

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
        OwnTweetRole,
        NewTweetRole,
        SinceTimeRole
    };

    TweetQmlListModel(QObject *parent = 0);
    TweetQmlListModel(OAuthTwitter* oauthTwitter, QObject *parent = 0);
    virtual ~TweetQmlListModel();
    void setOAuthTwitter(OAuthTwitter* oauthTwitter);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void setUserID(qint64 userid);
    qint64 userID() const;
    Q_INVOKABLE void destroyTweet(const QString& tweetid);

    int numNewTweets() const;
    void resetNumNewTweets();

    Q_INVOKABLE void showNewTweets();
    virtual void loadTweetsFromDatabase();
signals:
    void numNewTweetsChanged();

public slots:
    virtual void onStatusesStream(const QTweetStatus& status);

private slots:
    void finishedDestroyTweet(const QTweetStatus& status);

protected:
    QList<QTweetStatus> m_statuses;
    QList<QTweetStatus> m_newStatuses; //doesn't show in the model
    int m_numNewTweets;
    int m_numUnreadTweets;

private:
    OAuthTwitter* m_oauthTwitter;
    qint64 m_userid;
};

#endif // TWEETQMLLISTMODEL_H
