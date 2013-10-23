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
 
#ifndef SHORTENEDURL_H
#define SHORTENEDURL_H
 
#include <QObject>
#include <QUrl>

/**
 *   Class for shortening url
 *   @remarks Uses bit.ly services
 */
class ShortenedUrl : public QObject
{
    Q_OBJECT
public:
    ShortenedUrl(QObject *parent = 0);
    void shortUrl(const QString& url);

signals:
    /**
     * emits the short url and long url which was shorten
     * If short url is empty there was error with shortening
     */
    void finishedShortingUrl(const QString& shortUrl, const QString& longUrl);

private slots:
    void finished();

};
 
#endif //SHORTENEDEURL_H
