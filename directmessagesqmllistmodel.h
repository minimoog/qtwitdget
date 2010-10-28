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

#ifndef DIRECTMESSAGESQMLLISTMODEL_H
#define DIRECTMESSAGESQMLLISTMODEL_H

#include <QAbstractListModel>

class OAuthTwitter;
class QTimer;
class QTweetDMStatus;

//Copy pasted from MentionsQmlListModel, maybe needs abstraction
//### TODO: Abstraction? But this is a very special case

class DirectMessagesQmlListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int numNewDirectMessages
               READ numNewDirectMessages
               RESET resetNumNewDirectMessages
               NOTIFY numNewDirectMessagesChanged)
public:
    enum TweetRoles {
        ScreenNameRole = Qt::UserRole + 1,
        StatusTextRole,
        AvatarUrlRole,
        StatusIdRole
    };

    DirectMessagesQmlListModel(QObject *parent = 0);
    void setOAuthTwitter(OAuthTwitter* oauthTwitter);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void setUserID(int userid);

    int numNewDirectMessages() const;
    void resetNumNewDirectMessages();

    Q_INVOKABLE void showNewTweets();
    void loadTweetsFromDatabase();

    void startUpdateTimelines();

signals:
    void numNewDirectMessagesChanged();

private slots:
    void updateTimeline();
    void finishedTimeline(const QList<QTweetDMStatus>& statuses);
    void error();

private:
    OAuthTwitter* m_oauthTwitter;
    QTimer* m_timer;
    QList<QTweetDMStatus> m_directMessages;
    QList<QTweetDMStatus> m_newDirectMessages; //doesn't show in the model
    int m_userid;
    int m_numNewDirectMessages;
    int m_numOldDirectMessages;
};

#endif // DIRECTMESSAGESQMLLISTMODEL_H
