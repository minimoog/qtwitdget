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

#ifndef SHORTURLSITEM_H
#define SHORTURLSITEM_H

#include <QObject>

/**
 *  QML item for shortening urls in the text
 *  @remarks it is used like item in the declarative engine
 */
class ShortUrlsItem : public QObject
{
    Q_OBJECT
public:
    explicit ShortUrlsItem(QObject *parent = 0);
    Q_INVOKABLE void shortUrls(const QString& textWithUrls);

signals:
    /** Emits the text with shortened urls */
    void shortedText(const QString& text);

private slots:
    void finishedShortingUrl(const QString& shortUrl, const QString& longUrl);

private:
    QString m_textWithUrls;
};

#endif // SHORTURLSITEM_H
