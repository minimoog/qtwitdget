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

/*
 * TODO: Error notification and detection
 */

#ifndef OAUTHTWITTER_H
#define OAUTHTWITTER_H

#include <QNetworkAccessManager>
#include "oauth.h"

#define TWITTER_REQUEST_TOKEN_URL "http://twitter.com/oauth/request_token"
#define TWITTER_ACCESS_TOKEN_URL "http://twitter.com/oauth/access_token"
#define TWITTER_AUTHORIZE_URL "http://twitter.com/oauth/authorize"

class OAuthTwitter : public OAuth
{
	Q_OBJECT
public:
	/*! Constructor */
	OAuthTwitter(QObject *parent = 0);
	/*! Sets network access manager 
	 * \remark Must be set
	 */
	void setNetworkAccessManager(QNetworkAccessManager* netManager);
	/*! Gets network access manager */
	QNetworkAccessManager* networkAccessManager() const;
	/*! OAuth Authentication flow to get access tokens */
	void authorize();
protected:
	/*! Reimplement this if you want other widget to get confirm of the authorization */ 
	int authorizationWidget();

private slots:
	void error();

private:
	void requestToken();
	void requestAuthorization();
	void requestAccessToken(int pin);

	QNetworkAccessManager *m_netManager;
};	

#endif //OAUTHTWITTER_H