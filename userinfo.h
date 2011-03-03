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

#ifndef USERINFO_H
#define USERINFO_H

#include <QObject>
#include "qtweetlib/qtweetuser.h"

class OAuthTwitter;

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
    Q_PROPERTY(int numTweets READ numTweets NOTIFY userInfoChanged)
    Q_PROPERTY(int numFollowers READ numFollowers NOTIFY userInfoChanged)
    Q_PROPERTY(int numFollowing READ numFollowing NOTIFY userInfoChanged)
    Q_PROPERTY(int numFavorites READ numFavorites NOTIFY userInfoChanged)
public:
    explicit UserInfo(QObject *parent = 0);
    QString screenName() const { return m_userinfo.screenName(); }
    QString description() const { return m_userinfo.description(); }
    QString url() const { return m_userinfo.url(); }
    QString avatarUrl() const { return m_userinfo.profileImageUrl(); }
    QString location() const { return m_userinfo.location(); }
    QString userid() const { return QString::number(m_userinfo.id()); }
    QString name() const { return m_userinfo.name(); }
    int numTweets() const { return m_userinfo.statusesCount(); }
    int numFollowers() const { return m_userinfo.followersCount(); }
    int numFollowing() const { return m_userinfo.friendsCount(); }
    int numFavorites() const { return m_userinfo.favouritesCount(); }

    void setOAuthTwitter(OAuthTwitter* oauthTwitter) { m_oauthTwitter = oauthTwitter; }
    void fetch(qint64 userid);
    Q_INVOKABLE void fetch(const QString& userid);
    Q_INVOKABLE void fetchByName(const QString& screenName);

signals:
    void userInfoChanged();

private slots:
    void finishedFetch(const QTweetUser& userInfo);

private:
    QTweetUser m_userinfo;
    OAuthTwitter* m_oauthTwitter;
};

#endif // USERINFO_H
