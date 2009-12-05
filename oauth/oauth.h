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

#ifndef OAUTH_H
#define OAUTH_H

#include <QObject>
#include <QByteArray>
#include <QLibrary>
#include <QUrl>
#include <openssl/hmac.h>
#include <openssl/evp.h>

typedef unsigned char * (*FPHMAC)(const EVP_MD *, const void *, int,
		                        const unsigned char *, size_t, unsigned char *,
		                        unsigned int *);

typedef const EVP_MD * (*FPEVPSHA1)(void);

/*! \class OAuth
 *	\brief Base OAuth class
 */
class OAuth : public QObject
{
	Q_OBJECT

public:
	/*! Constructor */
	OAuth(QObject *parent = 0);
	
	enum HttpMethod {GET, POST, PUT, DELETE};
	/*! Parses oauth_token and oauth_token_secret from response of the service provider
	 *  and sets m_oauthToken and m_oauthTokenSecret accordingly 
	 * \param response Response from service provider 
	 */
	void parseTokens(const QByteArray& response);
	/*! Generates Authorization Header 
	 * \remark If HttpMethod is POST put query items in url (QUrl::addEncodedQueryItem)
	 */
	QByteArray generateAuthorizationHeader(const QUrl& url, HttpMethod method);
	/*! Sets oauth_token */
	void setOAuthToken(const QByteArray& token);
	/*! Sets oauth_token_secret */
	void setOAuthTokenSecret(const QByteArray& tokenSecret);
	/*! Gets oauth_token */
	QByteArray oauthToken() const;
	/*! Gets oauth_token_secret */
	QByteArray oauthTokenSecret() const;
	
private:
	QByteArray generateSignatureHMACSHA1(const QByteArray& signatureBase);
	QByteArray generateSignatureBase(const QUrl& url, HttpMethod method, const QByteArray& timestamp, const QByteArray& nonce);

	QByteArray m_oauthToken;
	QByteArray m_oauthTokenSecret;

    //dynamic linking and function pointers
    QLibrary m_libraryOpenssl;
    FPHMAC HMAC_fp;
    FPEVPSHA1 EVP_sha1_fp;
};

#endif //OAUTH_H