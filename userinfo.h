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

#ifndef USERINFO_H
#define USERINFO_H

#include <QObject>
#include "qtweetlib/qtweetuser.h"

class OAuthTwitter;
class QTweetStatus;

/**
 *  Stores, fetches twitter user information.
 */
class UserInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString screenName READ screenName NOTIFY userInfoChanged)
    Q_PROPERTY(QString description READ description  NOTIFY userInfoChanged)
    Q_PROPERTY(QString url READ url NOTIFY userInfoChanged)
    Q_PROPERTY(QString avatarUrl READ avatarUrl NOTIFY userInfoChanged)
    Q_PROPERTY(QString location READ location NOTIFY userInfoChanged)
    Q_PROPERTY(QString userid READ userid NOTIFY userInfoChanged)
    Q_PROPERTY(QString name READ name NOTIFY userInfoChanged)
    Q_PROPERTY(bool isFriend READ isFriend NOTIFY isFriendChanged)
public:
    explicit UserInfo(QObject *parent = 0);
    /** returns user screenname */
    QString screenName() const { return m_userinfo.screenName(); }
    /** returns user description */
    QString description() const { return m_userinfo.description(); }
    /** returns user url */
    QString url() const { return m_userinfo.url(); }
    /** returns user avatar url */
    QString avatarUrl() const { return m_userinfo.profileImageUrl(); }
    /** returns user location */
    QString location() const { return m_userinfo.location(); }
    /** returns user id in string */
    QString userid() const { return QString::number(m_userinfo.id()); }
    /** returns user name */
    QString name() const { return m_userinfo.name(); }
    /** true if authenticated user is following the user */
    bool isFriend() const { return m_isFriend; }

    void setOAuthTwitter(OAuthTwitter* oauthTwitter) { m_oauthTwitter = oauthTwitter; }
    void fetch(qint64 userid);
    Q_INVOKABLE void fetch(const QString& userid);
    Q_INVOKABLE void fetchByName(const QString& screenName);
    Q_INVOKABLE void followUser(const QString& screenName);
    Q_INVOKABLE void unfollowUser(const QString& screenName);
    Q_INVOKABLE void createFavorite(const QString& id);

public slots:
    void onUserStreamFriendsList(const QList<qint64> friends);

signals:
    /** emited when user information is changed */
    void userInfoChanged();
    void isFriendChanged();

private slots:
    void finishedFetch(const QTweetUser& userInfo);
    void finishedFollowUser(const QTweetUser& user);
    void finishedUnfollowUser(const QTweetUser& user);
    void finishedCreateFavorite(const QTweetStatus& status);

private:
    QTweetUser m_userinfo;
    OAuthTwitter* m_oauthTwitter;
    QList<qint64> m_friends;
    bool m_isFriend;
};

#endif // USERINFO_H
