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
 */

#include <QtDebug>
#include <QNetworkReply>
#include "qtwitfriends.h"

QTwitFriends::QTwitFriends(QObject *parent)
	:	QTwitBase(parent), m_paging(false)
{
}

QTwitFriends::QTwitFriends(QNetworkAccessManager* netManager, OAuthTwitter* oauthTwitter, QObject *parent)
:	QTwitBase(netManager, oauthTwitter, parent), m_paging(false)
{
}

QList<QTwitUser> QTwitFriends::getFriends() const
{
	return m_friends;
}

void QTwitFriends::updateFriends(int id, int userId, const QString& screenName, const QString& cursor)
{
	Q_ASSERT(networkAccessManager() != 0);

    if (!m_paging)
	    m_friends.clear();

    QUrl url("http://api.twitter.com/1/statuses/friends.json");

    if (id != 0) {
		QString strId = QString("%1").arg(id);
		url.addQueryItem("id", strId);
	}

    if (userId != 0) {
		QString strUserId = QString("%1").arg(userId);
		url.addQueryItem("user_id", strUserId);
	}

    if (!screenName.isEmpty()) {
		url.addQueryItem("screen_name", screenName);
	}

    if (!cursor.isEmpty()) {
        url.addQueryItem("cursor", cursor);
        m_paging = true;
    }

	QNetworkRequest req(url);
	QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
	req.setRawHeader("Authorization", oauthHeader);
	QNetworkReply *reply = networkAccessManager()->get(req);
	connect(reply, SIGNAL(finished()), this, SLOT(reply()));
	connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTwitFriends::reply()
{
	QNetworkReply *netReply = qobject_cast<QNetworkReply*>(sender());
	if(netReply){
//        XmlReadUserList xmlUserList;
//        xmlUserList.read(netReply);
//
//        m_friends.append(xmlUserList.users());
//
//        if (xmlUserList.nextCursor() != QString("0")) {
//            m_paging = true;
//            updateFriends(0, 0, QString(), xmlUserList.nextCursor());
//        } else {
//            m_paging = false;
//            emit finished();
//        }

        QString nextCursor;
        QString prevCursor;

        QList<QTwitUser> users = parseUserListJSON(netReply, nextCursor, prevCursor);

        m_friends.append(users);

        if (nextCursor != QString("0")) {
            m_paging = true;
            updateFriends(0, 0, QString(), nextCursor);
        } else {
            m_paging = false;
            emit finished();
        }

        netReply->deleteLater();
	}
}


void QTwitFriends::error()
{
	qDebug() << "QTwitFriends error";
}
