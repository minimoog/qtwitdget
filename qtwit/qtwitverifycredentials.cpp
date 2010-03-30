/* Copyright (c) 2008, Antonie Jovanoski
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
#include "qtwitverifycredentials.h"
#include "xml/xmlreaderextuserinfo.h"

/*!
    Constructor
 */
QTwitVerifyCredentials::QTwitVerifyCredentials(QObject *parent)
	:	QTwitBase(parent)
{
}

/*!
    Verify credentials
 */
void QTwitVerifyCredentials::verify()
{
	Q_ASSERT(networkAccessManager() != 0);

    QUrl url("http://api.twitter.com/1/account/verify_credentials.xml");

	QByteArray oauthHeader = oauthTwitter()->generateAuthorizationHeader(url, OAuth::GET);
	QNetworkRequest req(url);
	req.setRawHeader("Authorization", oauthHeader);

	QNetworkReply *netReply = networkAccessManager()->get(req);
	connect(netReply, SIGNAL(finished()), this, SLOT(reply()));
	connect(netReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
}

void QTwitVerifyCredentials::reply()
{
	QNetworkReply *netReply = qobject_cast<QNetworkReply*>(sender());
	if(netReply){
		XmlReaderExtUserInfo xr;
		if(xr.read(netReply))
			m_userinfo = xr.userInfo();

		netReply->deleteLater();
	}

	emit finished(true);
}

void QTwitVerifyCredentials::error()
{
	emit finished(false);
}

/*!
    Gets user extended info
    \return Extended user info
    \remark Only after verifying user info is valid
 */
QTwitExtUserInfo QTwitVerifyCredentials::userInfo() const
{
	return m_userinfo;
}
