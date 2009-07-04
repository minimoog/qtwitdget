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
#include "oauthtwitter.h"
#include "pindialog.h"

OAuthTwitter::OAuthTwitter(QObject *parent)
	:	OAuth(parent), m_netManager(0)
{
}

void OAuthTwitter::setNetworkAccessManager(QNetworkAccessManager* netManager)
{
	m_netManager = netManager;
}

QNetworkAccessManager* OAuthTwitter::networkAccessManager() const
{
	return m_netManager;
}

void OAuthTwitter::requestToken()
{
	Q_ASSERT(m_netManager != 0);

	QUrl url(TWITTER_REQUEST_TOKEN_URL);

	QByteArray oauthHeader = generateAuthorizationHeader(url, OAuth::POST);

	QNetworkRequest req(url);
	req.setRawHeader("Authorization", oauthHeader);

	QEventLoop q;
	QTimer tT;
	tT.setSingleShot(true);
	connect(&tT, SIGNAL(timeout()), &q, SLOT(quit()));

	QNetworkReply *reply = m_netManager->post(req, QByteArray());
	connect(reply, SIGNAL(finished()), &q, SLOT(quit()));
	connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));

	tT.start(5000);
	q.exec();

	if(tT.isActive()){
		tT.stop();
		QByteArray response = reply->readAll();
		parseTokens(response);

		reply->deleteLater();
		requestAuthorization();

        int pin = authorizationWidget();
        if (pin) {
            requestAccessToken(pin);
        }
	} else {
		qDebug() << "Timeout";
	}
}

void OAuthTwitter::requestAuthorization()
{
	QUrl authorizeUrl(TWITTER_AUTHORIZE_URL);
	authorizeUrl.addEncodedQueryItem("oauth_token", oauthToken());

	QDesktopServices::openUrl(authorizeUrl);
}

void OAuthTwitter::requestAccessToken(int pin)
{
	Q_ASSERT(m_netManager != 0);

	QUrl url(TWITTER_ACCESS_TOKEN_URL);
    url.addEncodedQueryItem("oauth_verifier", QByteArray::number(pin));

	QByteArray oauthHeader = generateAuthorizationHeader(url, OAuth::POST);

	QEventLoop q;
	QTimer tT;
	tT.setSingleShot(true);

	connect(&tT, SIGNAL(timeout()), &q, SLOT(quit()));

	QNetworkRequest req(url);
	req.setRawHeader("Authorization", oauthHeader);

	QNetworkReply *reply = m_netManager->post(req, QByteArray());
	connect(reply, SIGNAL(finished()), &q, SLOT(quit()));
	connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));

	tT.start(5000);
	q.exec();

	if(tT.isActive()){
		QByteArray response = reply->readAll();
		parseTokens(response);
		reply->deleteLater();
	} else {
		qDebug() << "Timeout";
	}
}

int OAuthTwitter::authorizationWidget()
{
	//PIN based http://apiwiki.twitter.com/Authentication
    PinDialog pinDialog;

    if (pinDialog.exec()) {
        return pinDialog.getPin();
    } else {
        return 0;
    }    
}

void OAuthTwitter::authorize()
{
	requestToken();
}

void OAuthTwitter::error()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
	if(reply){
		qDebug() << "OAuth Twitter error";
		qDebug() << reply->errorString();
	}
}