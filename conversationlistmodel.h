/* Copyright (c) 2011, Antonie Jovanoski
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

#ifndef CONVERSATIONLISTMODEL_H
#define CONVERSATIONLISTMODEL_H

#include <QAbstractListModel>

class OAuthTwitter;
class QTweetStatus;

class ConversationListModel : public QAbstractListModel
{
    Q_OBJECT
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

    explicit ConversationListModel(QObject *parent = 0);
    ConversationListModel(OAuthTwitter *oauthTwitter, QObject *parent = 0);
    virtual ~ConversationListModel();
    void setOAuthTwitter(OAuthTwitter *oauthTwitter);
    void setUserID(qint64 userid);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    void followConversation(qint64 statusID);
    Q_INVOKABLE void followConversation(const QString& statusID);

private slots:
    void onParsedStatus(const QTweetStatus& status);

private:
    void fetchConversation(qint64 statusID);
    QTweetStatus findInDatabase(qint64 id);

    OAuthTwitter* m_oauthTwitter;
    QList<QTweetStatus> m_statuses;
    qint64 m_userid;
};

#endif // CONVERSATIONLISTMODEL_H
