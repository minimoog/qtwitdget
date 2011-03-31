/* Copyright (c) 2011, Antonie Jovanoski
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
#include "shorturlsitem.h"
#include <QNetworkAccessManager>
#include <QRegExp>
#include "shortenedurl.h"

/**
 *  Constructor
 */
ShortUrlsItem::ShortUrlsItem(QObject *parent) :
    QObject(parent), m_netManager(new QNetworkAccessManager(this))
{
}

/**
 *  Shorts urls within the text
 *  @param textWithUrls text which contains urls to shorten
 */
void ShortUrlsItem::shortUrls(const QString &textWithUrls)
{
    m_textWithUrls = textWithUrls;

    QRegExp rx("\\(?\\bhttp://[-A-Za-z0-9+&@#/%?=~_()|!:,.;]*[-A-Za-z0-9+&@#/%=~_()|]", Qt::CaseInsensitive, QRegExp::RegExp2);

    int pos = 0;

    while (pos >= 0) {
        pos = rx.indexIn(textWithUrls, pos);

        if (pos >= 0) {
            QString matchedUrl = textWithUrls.mid(pos, rx.matchedLength());

            if (!matchedUrl.contains("http://bit.ly/")) {
                ShortenedUrl *shortenedUrl = new ShortenedUrl(m_netManager, this);
                shortenedUrl->shortUrl(matchedUrl);
                connect(shortenedUrl, SIGNAL(finishedShortingUrl(QString,QString)),
                        this, SLOT(finishedShortingUrl(QString,QString)));
            }
        }
        pos += rx.matchedLength();
    }
}

void ShortUrlsItem::finishedShortingUrl(const QString &shortUrl, const QString &longUrl)
{
    ShortenedUrl *shortenedUrl = qobject_cast<ShortenedUrl*>(sender());

    if (shortenedUrl) {
        if (!shortUrl.isEmpty()) {
            m_textWithUrls.replace(longUrl, shortUrl);
            emit shortedText(m_textWithUrls);
        }

        shortenedUrl->deleteLater();
    }
}
