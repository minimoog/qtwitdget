/* Copyright (c) 2009, Antonie Jovanoski
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

#include "qtwitbase.h"
#include "qjson/parser.h"

/*!
    Constructor
 */
QTwitBase::QTwitBase(QObject *parent)
	:	QObject(parent), m_netManager(0), m_oauthTwitter(0)	
{
}

/*!
    Constructor
    \param netManager Network Access Manager
    \param oauthTwitter OAuth Twitter
    \param parent QObject parent
 */
QTwitBase::QTwitBase(QNetworkAccessManager* netManager, OAuthTwitter* oauthTwitter, QObject *parent)
:	QObject(parent), m_netManager(netManager), m_oauthTwitter(oauthTwitter)
{
}

/*!
    Sets network access manager
    \param netManager Network access manager
 */
void QTwitBase::setNetworkAccessManager(QNetworkAccessManager* netManager)
{
	m_netManager = netManager;
}

/*!
    Gets network access manager
    \return Network access manager
 */
QNetworkAccessManager* QTwitBase::networkAccessManager() const
{
	return m_netManager;
}

/*!
    Sets OAuth Twitter authorization
    \param oauthTwitter OAuth Twitter
 */
void QTwitBase::setOAuthTwitter(OAuthTwitter* oauthTwitter)
{
	m_oauthTwitter = oauthTwitter;
}

/*!
    Gets OAuth Twitter authorization
    \return OAuth Twitter
 */
OAuthTwitter* QTwitBase::oauthTwitter() const
{
	return m_oauthTwitter;
}

QList<QTwitStatus> QTwitBase::parseStatusesListJSON(QIODevice *device)
{
    QList<QTwitStatus> qtStatuses;
    QJson::Parser parser;
    bool ok;

    QVariant jsonResponse = parser.parse(device, &ok);
    if (!ok) {
        qFatal("Json parsing error: List of statutes");
        return qtStatuses;
    }

    QList<QVariant> listStatuses = jsonResponse.toList();

    foreach(const QVariant& status, listStatuses) {
        QTwitStatus qtStatus;
        QVariantMap statusMap = status.toMap();
        //qDebug() << statusMap["created_at"].toString();

        qtStatus.setId(statusMap["id"].toLongLong());
        qtStatus.setText(statusMap["text"].toString());
        qtStatus.setReplyToStatusId(statusMap["in_reply_to_status_id"].toLongLong());
        qtStatus.setReplyToUserId(statusMap["in_reply_to_user_id"].toInt());
        qtStatus.setFavorited(statusMap["favorited"].toBool());
        qtStatus.setReplyToScreenName(statusMap["in_reply_to_screen_name"].toString());

        QVariantMap user = statusMap["user"].toMap();
        qtStatus.setUserId(user["id"].toInt());
        qtStatus.setName(user["name"].toString());
        qtStatus.setScreenName(user["screen_name"].toString());
        qtStatus.setLocation(user["location"].toString());
        qtStatus.setDescription(user["description"].toString());
        qtStatus.setProfileImageUrl(user["profile_image_url"].toString());
        qtStatus.setUrl(user["url"].toString());

        qtStatuses.append(qtStatus);
    }

    return qtStatuses;
}

QTwitStatus QTwitBase::parseStatusJSON(QIODevice *device)
{
    QTwitStatus status;
    QJson::Parser parser;
    bool ok;

    QVariant jsonResponse = parser.parse(device, &ok);
    if (!ok) {
        qFatal("Json parsing error: Single status");
        return status;
    }

    QVariantMap statusMap = jsonResponse.toMap();
    //qDebug() << statusMap["created_at"].toString();

    status.setId(statusMap["id"].toLongLong());
    status.setText(statusMap["text"].toString());
    status.setReplyToStatusId(statusMap["in_reply_to_status_id"].toLongLong());
    status.setReplyToUserId(statusMap["in_reply_to_user_id"].toInt());
    status.setFavorited(statusMap["favorited"].toBool());
    status.setReplyToScreenName(statusMap["in_reply_to_screen_name"].toString());

    QVariantMap user = statusMap["user"].toMap();
    status.setUserId(user["id"].toInt());
    status.setName(user["name"].toString());
    status.setScreenName(user["screen_name"].toString());
    status.setLocation(user["location"].toString());
    status.setDescription(user["description"].toString());
    status.setProfileImageUrl(user["profile_image_url"].toString());
    status.setUrl(user["url"].toString());

    return status;
}

QTwitExtUserInfo QTwitBase::parseExtUserInfoJSON(QIODevice *device)
{
    QTwitExtUserInfo extInfo;
    QJson::Parser parser;
    bool ok;

    QVariantMap userMap = parser.parse(device, &ok).toMap();
    if (!ok) {
        qFatal("Json parsing error: Extended User Info");
        return extInfo;
    }

    extInfo.setId(userMap["id"].toInt());
    extInfo.setName(userMap["name"].toString());
    extInfo.setScreenName(userMap["screen_name"].toString());
    extInfo.setLocation(userMap["location"].toString());
    extInfo.setDescription(userMap["description"].toString());
    extInfo.setProfileImageUrl(userMap["profile_image_url"].toString());

    return extInfo;
}
