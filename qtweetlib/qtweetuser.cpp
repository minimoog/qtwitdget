/* Copyright (c) 2010, Antonie Jovanoski
 *
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#include <QSharedData>
#include <QDateTime>
#include "qtweetuser.h"
#include "qtweetstatus.h"

class QTweetUserData : public QSharedData
{
public:
    QTweetUserData() : id(0) {}

    QString description;
    int favoritesCount;
    bool followRequestSent;
    int followersCount;
    int friendsCount;
    bool geoEnabled;
    qint64 id;
    int listedCount;
    QString location;
    QString name;
    QString profileImageUrl;
    QString screenName;
    int statusesCount;
    QString url;
    //avoid recursion with QTweetStatus
    qint64 statusId;
    QString statusText;
    QDateTime statusCreatedAt;
    qint64 statusInReplyToUserId;
    QString statusInReplyToScreenName;
    qint64 statusInReplyToStatusId;
    bool statusFavorited;
    QString statusSource;
};

QTweetUser::QTweetUser() :
        d(new QTweetUserData)
{
}

QTweetUser::QTweetUser(const QTweetUser &other) :
        d(other.d)
{
}

QTweetUser& QTweetUser::operator =(const QTweetUser& other)
{
    if (this != &other)
        d.operator =(other.d);
    return *this;
}

QTweetUser::~QTweetUser()
{
}

void QTweetUser::setId(qint64 id)
{
    d->id = id;
}

qint64 QTweetUser::id() const
{
    return d->id;
}

void QTweetUser::setListedCount(int count)
{
    d->listedCount = count;
}

int QTweetUser::listedCount() const
{
    return d->listedCount;
}

void QTweetUser::setName(const QString &name)
{
    d->name = name;
}

QString QTweetUser::name() const
{
    return d->name;
}

void QTweetUser::setScreenName(const QString &screenName)
{
    d->screenName = screenName;
}

QString QTweetUser::screenName() const
{
    return d->screenName;
}

void QTweetUser::setLocation(const QString &location)
{
    d->location = location;
}

QString QTweetUser::location() const
{
    return d->location;
}

void QTweetUser::setDescription(const QString &desc)
{
    d->description = desc;
}

QString QTweetUser::description() const
{
    return d->description;
}

void QTweetUser::setprofileImageUrl(const QString &url)
{
    d->profileImageUrl = url;
}

QString QTweetUser::profileImageUrl() const
{
    return d->profileImageUrl;
}

void QTweetUser::setUrl(const QString &url)
{
    d->url = url;
}

QString QTweetUser::url() const
{
    return d->url;
}

void QTweetUser::setFollowersCount(int count)
{
    d->followersCount = count;
}

int QTweetUser::followersCount() const
{
    return d->followersCount;
}

void QTweetUser::setFriendsCount(int count)
{
    d->friendsCount = count;
}

int QTweetUser::friendsCount() const
{
    return d->friendsCount;
}

void QTweetUser::setFavouritesCount(int count)
{
    d->favoritesCount = count;
}

int QTweetUser::favouritesCount() const
{
    return d->favoritesCount;
}

void QTweetUser::setGeoEnabled(bool isGeoEnabled)
{
    d->geoEnabled = isGeoEnabled;
}

bool QTweetUser::isGeoEnabled() const
{
    return d->geoEnabled;
}

void QTweetUser::setStatusesCount(int count)
{
    d->statusesCount = count;
}

int QTweetUser::statusesCount() const
{
    return d->statusesCount;
}

void QTweetUser::setStatus(const QTweetStatus &lastStatus)
{
    //d->status = lastStatus;
    d->statusId = lastStatus.id();
    d->statusText = lastStatus.text();
    d->statusCreatedAt = lastStatus.createdAt();
    d->statusInReplyToStatusId = lastStatus.inReplyToStatusId();
    d->statusFavorited = lastStatus.favorited();
}

QTweetStatus QTweetUser::status() const
{
    QTweetStatus lastStatus;
    lastStatus.setId(d->statusId);
    lastStatus.setText(d->statusText);
    //lastStatus.setCreatedAt();    // ### TODO FIX IT!
    lastStatus.setInReplyToStatusId(d->statusInReplyToStatusId);
    lastStatus.setFavorited(d->statusFavorited);

    return lastStatus;
}

QDateTime QTweetUser::twitterDateToQDateTime(const QString &twitterDate)
{
    //Twitter Date Format: 'Wed Sep 01 11:27:25 +0000 2010'  UTC
    QString dateString = twitterDate.left(11) + twitterDate.right(4);
    QString timeString = twitterDate.mid(11, 8);

    QDate date = QDate::fromString(dateString);
    QTime time = QTime::fromString(timeString);

    if (date.isValid() && time.isValid())
        return QDateTime(date, time, Qt::UTC);
    else
        return QDateTime();
}
