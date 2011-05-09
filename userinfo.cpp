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

#include "userinfo.h"
#include "qtweetlib/qtweetusershow.h"
#include "qtweetlib/qtweetfriendshipcreate.h"
#include "qtweetlib/qtweetfriendshipdestroy.h"
#include "qtweetlib/qtweetfavoritescreate.h"

/**
 *  Constructor
 */
UserInfo::UserInfo(QObject *parent) :
    QObject(parent)
{
}

/**
 *  Fetches twitter user information
 *  @param userid id of user to fetch information
 */
void UserInfo::fetch(qint64 userid)
{
    QTweetUserShow* userShow = new QTweetUserShow(m_oauthTwitter, this);
    userShow->fetch(userid);
    connect(userShow, SIGNAL(parsedUserInfo(QTweetUser)), this, SLOT(finishedFetch(QTweetUser)));
}

/**
 *  Fetches twitter user information
 *  @param userid id of user to fetch information in string
 */
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

/**
 *  Fetches twitter user information
 *  @param screenName screenname of the user to fetch information
 */
void UserInfo::fetchByName(const QString &screenName)
{
    QTweetUserShow* userShow = new QTweetUserShow(m_oauthTwitter, this);
    userShow->fetch(screenName);
    connect(userShow, SIGNAL(parsedUserInfo(QTweetUser)), this, SLOT(finishedFetch(QTweetUser)));
}

/**
 *  Favorites tweet
 *  @param id id of tweet to be favorited in string
 */

// ### TODO: remove this
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

/**
 *  Called when fetching user information is finished
 */
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

/**
 * Connected to user stream, gives the friends list
 */
void UserInfo::onUserStreamFriendsList(const QList<qint64> friends)
{
    m_friends = friends;
}

/**
 *  Follows user
 *  @param screenName user screenname to follow
 */
void UserInfo::followUser(const QString &screenName)
{
    QTweetFriendshipCreate *createFriend = new QTweetFriendshipCreate(m_oauthTwitter, this);
    createFriend->create(screenName);
    connect(createFriend, SIGNAL(parsedUser(QTweetUser)), this, SLOT(finishedFollowUser(QTweetUser)));
}

/**
 *  Called when follow user is finished
 */
void UserInfo::finishedFollowUser(const QTweetUser &user)
{
    QTweetFriendshipCreate *createFriend = qobject_cast<QTweetFriendshipCreate*>(sender());

    // append followed user to friends list
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

/**
 *  Unfollows the user
 *  @param screenName user screenname to unfollow
 */
void UserInfo::unfollowUser(const QString &screenName)
{
    QTweetFriendshipDestroy *destroyFriend = new QTweetFriendshipDestroy(m_oauthTwitter, this);
    destroyFriend->unfollow(screenName);
    connect(destroyFriend, SIGNAL(parsedUser(QTweetUser)), this, SLOT(finishedUnfollowUser(QTweetUser)));
}

/**
 *  Called when unfollowin user is finisedh
 */
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

// ### TODO: remove this
void UserInfo::finishedCreateFavorite(const QTweetStatus &status)
{
    QTweetFavoritesCreate *favorited = qobject_cast<QTweetFavoritesCreate*>(sender());

    if (favorited) {
        favorited->deleteLater();

        // ### TODO: Some kind of confirmation that tweet was favorited
    }
}
