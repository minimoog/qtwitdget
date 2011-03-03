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

void UserInfo::finishedFetch(const QTweetUser &userInfo)
{
    QTweetUserShow* userShow = qobject_cast<QTweetUserShow*>(sender());

    if (userShow) {
        m_userinfo = userInfo;
        emit userInfoChanged();
        userShow->deleteLater();
    }
}
