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

#include <QtDebug>
#include <QUrl>
#include <QDesktopServices>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>
#include <QNetworkAccessManager>
#include "oauthtwitter.h"
#include "pindialog.h"
#include "signalwaiter.h"
#include "userpassdialog.h"

/*!
    Constructor
 */
OAuthTwitter::OAuthTwitter(QObject *parent)
    :	OAuth(parent), m_netManager(0)
{
}

/*!
    Sets network access manager
    \remarks Must be set to work properly
 */
void OAuthTwitter::setNetworkAccessManager(QNetworkAccessManager* netManager)
{
	m_netManager = netManager;
}

/*!
    Gets network access manager
 */
QNetworkAccessManager* OAuthTwitter::networkAccessManager() const
{
	return m_netManager;
}

void OAuthTwitter::requestAccessTokenXAuth(const QString &username, const QString &password)
{
    Q_ASSERT(m_netManager != 0);

    QUrl url(TWITTER_ACCESS_TOKEN_XAUTH_URL);
    url.addQueryItem("x_auth_username", username);
    url.addQueryItem("x_auth_password", password);
    url.addQueryItem("x_auth_mode", "client_auth");

    QByteArray oauthHeader = generateAuthorizationHeader(url, OAuth::POST);

    QNetworkRequest req(url);
    req.setRawHeader("Authorization", oauthHeader);

    QNetworkReply *reply = m_netManager->post(req, QByteArray());
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));

    SignalWaiter sigWait(reply, SIGNAL(finished()));

    if (sigWait.wait(5000)) {
        QByteArray response = reply->readAll();
        parseTokens(response);
        reply->deleteLater();
    } else {
        qDebug() << "Timeout";
    }
}

void OAuthTwitter::authorizeXAuth()
{
    UserPassDialog userpassdialog;

    if (userpassdialog.exec()) {
        requestAccessTokenXAuth(userpassdialog.username(), userpassdialog.password());
    }
}

void OAuthTwitter::error()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
	if(reply){
		qDebug() << "OAuth Twitter error";
		qDebug() << reply->errorString();
	}
}
