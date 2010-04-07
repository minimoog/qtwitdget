/* Copyright (c) 2010, Antonie Jovanoski
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

#ifndef QTWITUSER_H
#define QTWITUSER_H

#include <QSharedDataPointer>
#include "qtwituserdata.h"

/*!
    Data class for storing information on twitter user
 */
class QTwitUser
{
public:
    QTwitUser() : d(new QTwitUserData) { }
    void setId(int id) { d->id = id; }
    int id() const { return d->id; }
    void setName(const QString& name) { d->name = name; }
    QString name() const { return d->name; }
    void setLocation(const QString& location) { d->location = location; }
    QString location() const { return d->location; }
    void setDescription(const QString& desc) { d->description = desc; }
    QString description() const { return d->description; }
    void setProfileImageUrl(const QString& url) { d->profileImageUrl = url; }
    QString profileImageUrl() const { return d->profileImageUrl; }
    void setUrl(const QString& url) { d->url = url; }
    QString url() const { return d->url; }

private:
    QSharedDataPointer<QTwitUserData> d;
};

#endif // QTWITUSER_H
