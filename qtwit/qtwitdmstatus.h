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

#ifndef QTWITDMSTATUS_H
#define QTWITDMSTATUS_H

#include <QMetaType>
#include "qtwitdmstatusdata.h"

class QTwitDMStatus
{
public:
    QTwitDMStatus() : d(new QTwitDMStatusData) {}

    void setId(qint64 id) { d->id = id; }
    qint64 id() const { return d->id; }
    void setSenderId(int id) { d->senderId = id; }
    int senderId() const { return d->senderId; }
    void setText(const QString& text) { d->text = text; }
    QString text() const { return d->text; }
    void setRecipientId(int id) { d->recipientId = id; }
    int recipientId() const { return d->recipientId; }
    void setCreatedAt(const QDateTime& datetime) { d->createdAt = datetime; }
    QDateTime createdAt() const { return d->createdAt; }
    void setSenderScreenName(const QString& screenName) { d->senderScreenName = screenName; }
    QString senderScreenName() const { return d->senderScreenName; }
    void setRecipientScreenName(const QString& screenName) { d->recipientScreenName = screenName; }
    QString recipientScreenName() const { return d->recipientScreenName; }

private:
    QSharedDataPointer<QTwitDMStatusData> d;
};

Q_DECLARE_METATYPE(QTwitDMStatus)

#endif // QTWITDMSTATUS_H
