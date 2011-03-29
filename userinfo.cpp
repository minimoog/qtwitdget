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

#include "userinfo.h"
#include "qtweetlib/qtweetusershow.h"
#include "qtweetlib/qtweetfriendshipcreate.h"
#include "qtweetlib/qtweetfriendshipdestroy.h"
#include "qtweetlib/qtweetfavoritescreate.h"

UserInfo::UserInfo(QObject *parent) :
    QObject(parent)
{
}

void UserInfo::fetch(qint64 userid)
{
    QTweetUserShow* userShow = new QTweetUserShow(m_oauthTwitter, this);
    userShow->fetch(userid);
    connect(userShow, SIGNAL(parsedUserInfo(QTweetUser)), this, SLOT(finishedFetch(QTweetUser)));
}

void UserInfo::fetch(const QString &userid)
{
    bool ok;
    qint64 id = userid.toLongLong(&ok);

    if (ok) {
        QTweetUserShow* userShow = new QTweetUserShow(m_oauthTwitter, this);
        userShow->fetch(id);
        connect(userShow, SIGNAL(parsedUserInfo(QTweetUser)), this, SLOT(finishedFetch(QTweetUser)));
    }
}

void UserInfo::fetchByName(const QString &screenName)
{
    QTweetUserShow* userShow = new QTweetUserShow(m_oauthTwitter, this);
    userShow->fetch(screenName);
    connect(userShow, SIGNAL(parsedUserInfo(QTweetUser)), this, SLOT(finishedFetch(QTweetUser)));
}

void UserInfo::createFavorite(const QString &id)
{
    bool ok;
    qint64 tweetid = id.toLongLong(&ok);

    if (ok) {
        QTweetFavoritesCreate* favorited = new QTweetFavoritesCreate(m_oauthTwitter, this);
        favorited->create(tweetid);
        connect(favorited, SIGNAL(parsedStatus(QTweetStatus)),
                this, SLOT(finishedCreateFavorite(QTweetStatus)));
    }
}

void UserInfo::finishedFetch(const QTweetUser &userInfo)
{
    QTweetUserShow* userShow = qobject_cast<QTweetUserShow*>(sender());

    if (userShow) {
        m_userinfo = userInfo;
        emit userInfoChanged();
        userShow->deleteLater();

        //check if is in friends list
        bool isInFriendsList = m_friends.contains(m_userinfo.id());

        if (isInFriendsList != m_isFriend) {
            m_isFriend = isInFriendsList;
            emit isFriendChanged();
        }
    }
}

void UserInfo::onUserStreamFriendsList(const QList<qint64> friends)
{
    m_friends = friends;
}

void UserInfo::followUser(const QString &screenName)
{
    QTweetFriendshipCreate *createFriend = new QTweetFriendshipCreate(m_oauthTwitter, this);
    createFriend->create(screenName);
    connect(createFriend, SIGNAL(parsedUser(QTweetUser)), this, SLOT(finishedFollowUser(QTweetUser)));
}

void UserInfo::finishedFollowUser(const QTweetUser &user)
{
    QTweetFriendshipCreate *createFriend = qobject_cast<QTweetFriendshipCreate*>(sender());

    if (createFriend) {
        createFriend->deleteLater();

        m_friends.append(user.id());

        if (m_userinfo.id() == user.id()) {
            if (!m_isFriend) {
                m_isFriend = true;
                emit isFriendChanged();
            }
        }
    }
}

void UserInfo::unfollowUser(const QString &screenName)
{
    QTweetFriendshipDestroy *destroyFriend = new QTweetFriendshipDestroy(m_oauthTwitter, this);
    destroyFriend->unfollow(screenName);
    connect(destroyFriend, SIGNAL(parsedUser(QTweetUser)), this, SLOT(finishedUnfollowUser(QTweetUser)));
}

void UserInfo::finishedUnfollowUser(const QTweetUser &user)
{
    QTweetFriendshipDestroy *destroyFriend = qobject_cast<QTweetFriendshipDestroy*>(sender());

    if (destroyFriend) {
        destroyFriend->deleteLater();

        //remove from friend list
        m_friends.removeOne(user.id());

        //if it's the same user info changed the isFriend property
        if (m_userinfo.id() == user.id()) {
            if (m_isFriend) {
                m_isFriend = false;
                emit isFriendChanged();
            }
        }
    }
}

void UserInfo::finishedCreateFavorite(const QTweetStatus &status)
{
    QTweetFavoritesCreate *favorited = qobject_cast<QTweetFavoritesCreate*>(sender());

    if (favorited) {
        favorited->deleteLater();

        // ### TODO: Some kind of confirmation that tweet was favorited
    }
}
