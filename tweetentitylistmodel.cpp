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

#include "tweetentitylistmodel.h"
#include <QtDebug>
#include <QRegExp>
#include <QSqlQuery>
#include "namsingleton.h"
#include "qtweetlib/oauthtwitter.h"
#include "qtweetlib/qtweetstatus.h"
#include "qtweetlib/qtweetstatusshow.h"

/**
 *  Constructor
 */
TweetEntityListModel::TweetEntityListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

/**
 *  @reimp
 */
int TweetEntityListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_entities.count();
}

int TweetEntityListModel::count() const
{
    return m_entities.count();
}

/**
 *  @reimp
 */
QVariant TweetEntityListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > m_entities.count())
        return QVariant();

    if (role == Qt::DisplayRole)
        return m_entities.at(index.row());

    return QVariant();
}

/**
 *  Sets tweet id for which tweet entity model list represents
 */
void TweetEntityListModel::setTweetid(const QString &id)
{
    loadEntities(id);
}

/**
 *  Loads entities (from local sqlite database or network)
 */
void TweetEntityListModel::loadEntities(const QString &tweetid)
{
    //clear model
    beginResetModel();
    m_entities.clear();
    endResetModel();

    bool ok;

    qint64 id = tweetid.toLongLong(&ok);

    if (ok) {
        //try to find tweet in database
        QSqlQuery query;
        query.prepare("SELECT text FROM status WHERE id = :id ");
        query.bindValue(":id", id);
        query.exec();

        if (query.next()) {
            QString tweetText = query.value(0).toString();

            captureEntities(tweetText);
        } else {
            //load from network
            OAuthTwitter *oauthTwitter = new OAuthTwitter(this);
            oauthTwitter->setNetworkAccessManager(NAMSingleton::instance()->qnam());

            QTweetStatusShow *statusShow = new QTweetStatusShow(this);
            statusShow->setOAuthTwitter(oauthTwitter);
            statusShow->setAuthenticationEnabled(false);
            statusShow->fetch(id, false, true);
            connect(statusShow, SIGNAL(parsedStatus(QTweetStatus)),
                    this, SLOT(finishedStatusShow(QTweetStatus)));
        }
    }
}

/**
 *  Invoked when status was fetch from network
 */
void TweetEntityListModel::finishedStatusShow(const QTweetStatus &status)
{
    QTweetStatusShow *statusShow = qobject_cast<QTweetStatusShow*>(sender());

    if (statusShow) {
        captureEntities(status.text());

        statusShow->deleteLater();
    }
}

/**
 *   Captures regex
 */
QStringList TweetEntityListModel::captureRegex(const QString &rx, const QString &text)
{
    QStringList captures;

    QRegExp regex(rx);

    int pos = 0;

    while (pos != -1) {
        pos = regex.indexIn(text, pos);

        if (pos != -1) {
            captures.append(text.mid(pos, regex.matchedLength()));
            pos += regex.matchedLength();
        }
    }

    return captures;
}

/**
 *  Captures entities for a given text
 */

// ### TODO: Use entities which gives twitter api f
void TweetEntityListModel::captureEntities(const QString &text)
{
    m_entities.append(captureRegex("(@[a-zA-Z0-9_]+)", text));
    //m_entities.append(captureRegex("(\\b(https?|ftp)://[-A-Z0-9+&@#/%?=~_|!:,.;]*[-A-Z0-9+&@#/%=~_|])", text));
    m_entities.append(captureRegex("\\(?\\bhttp://[-A-Za-z0-9+&@#/%?=~_()|!:,.;]*[-A-Za-z0-9+&@#/%=~_()|]", text));
    m_entities.append(captureRegex("([#]+[A-Za-z0-9-_]+)", text));

    if (m_entities.count()) {
        beginInsertRows(QModelIndex(), 0, m_entities.count() - 1);
        endInsertRows();
    }

    emit countChanged();
}
