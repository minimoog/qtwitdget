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

#include "usertimelinelistmodel.h"
#include <QDateTime>
#include <QRegExp>
#include "qtweetlib/oauthtwitter.h"
#include "qtweetlib/qtweetusertimeline.h"

static QString SinceTimeString(const QDateTime& from)
{
    int passedSeconds = from.secsTo(QDateTime::currentDateTimeUtc());

    if (passedSeconds < 0)
        return QString("now");

    if (passedSeconds < 60)
        return QString("%1 s").arg(passedSeconds);

    if (passedSeconds < 3600)
        return QString("%1 m").arg(passedSeconds / 60);

    if (passedSeconds < 86400)
        return QString("%1 h").arg(passedSeconds / 3600);

    return QString("%1 d").arg(passedSeconds / 86400);
}

/**
 * Constructor
 */
UserTimelineListModel::UserTimelineListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[StatusTextRole] = "statusTextRole";
    roles[SinceTimeRole] = "sinceTimeRole";
    roles[StatusIdRole] = "statusIdRole";
    setRoleNames(roles);
}

/**
 *  Constructor
 */
UserTimelineListModel::UserTimelineListModel(OAuthTwitter *oauthTwitter, QObject *parent) :
    QAbstractListModel(parent),
    m_oauthTwitter(oauthTwitter)
{
    QHash<int, QByteArray> roles;
    roles[StatusTextRole] = "statusTextRole";
    roles[SinceTimeRole] = "sinceTimeRole";
    roles[StatusIdRole] = "statusIdRole";
    setRoleNames(roles);
}

/**
 *  Sets oauth twitter object
 */
void UserTimelineListModel::setOAuthTwitter(OAuthTwitter *oauthTwitter)
{
    m_oauthTwitter = oauthTwitter;
}

/**
 *  @reimp
 */
int UserTimelineListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_statuses.count();
}

/**
 *  @reimp
 */
QVariant UserTimelineListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > m_statuses.count())
        return QVariant();

    const QTweetStatus& st = m_statuses.at(index.row());

    if (role == StatusTextRole)
        return st.text();
    else if (role == SinceTimeRole)
        return SinceTimeString(st.createdAt());
    else if (role == StatusIdRole)
        return QString::number(st.id());

    return QVariant();
}

/**
 *  Fetches user timeline
 *  @param userid id of the user to fetch the timeline
 */
void UserTimelineListModel::fetch(qint64 userid)
{
    QTweetUserTimeline *userTimeline = new QTweetUserTimeline(m_oauthTwitter, this);
    userTimeline->fetch(userid, QString(), 0, 0, 20);
    connect(userTimeline, SIGNAL(parsedStatuses(QList<QTweetStatus>)),
            this, SLOT(finishedFetching(QList<QTweetStatus>)));
    connect(userTimeline, SIGNAL(error(ErrorCode,QString)),
            this, SLOT(errorFetching()));
}

/**
 *  Fetches user timeline
 *  @param screenname screenname of the user to fetch the timeline
 */
void UserTimelineListModel::fetch(const QString &screenname)
{
    QTweetUserTimeline *userTimeline = new QTweetUserTimeline(m_oauthTwitter, this);
    userTimeline->fetch(0, screenname, 0, 0, 20);
    connect(userTimeline, SIGNAL(parsedStatuses(QList<QTweetStatus>)),
            this, SLOT(finishedFetching(QList<QTweetStatus>)));
    connect(userTimeline, SIGNAL(error(ErrorCode,QString)),
            this, SLOT(errorFetching()));
}

/**
 *  Called when fetching user timeline is finished
 */
void UserTimelineListModel::finishedFetching(const QList<QTweetStatus> &statuses)
{
    QTweetUserTimeline *userTimeline = qobject_cast<QTweetUserTimeline*>(sender());

    if (userTimeline) {

        if (!m_statuses.isEmpty()) {
            beginRemoveRows(QModelIndex(), 0, m_statuses.count() - 1);
            m_statuses.clear();
            endRemoveRows();
        }

        if (!statuses.isEmpty()) {
            beginInsertRows(QModelIndex(), 0, statuses.count() - 1);

            foreach (QTweetStatus status, statuses) {
                QString textWithTags = addTags(status.text());
                status.setText(textWithTags);
                m_statuses.append(status);
            }

            endInsertRows();
        }

        userTimeline->deleteLater();
    }
}

void UserTimelineListModel::errorFetching()
{
    QTweetUserTimeline *userTimeline = qobject_cast<QTweetUserTimeline*>(sender());

    if (userTimeline)
        userTimeline->deleteLater();
}

// ### TODO: Make it global
QString UserTimelineListModel::addTags(const QString &text)
{
    QString tweet(text);
    QString mentions = tweet.replace(QRegExp("(@[a-zA-Z0-9_]+)"), "<a href=\"mention://\\1\">\\1</a>");
    QString httpLinks = mentions.replace(QRegExp("(\\b(https?|ftp)://[-A-Z0-9+&@#/%?=~_|!:,.;]*[-A-Z0-9+&@#/%=~_|])", Qt::CaseInsensitive),
                                        "<a href='\\1'>\\1</a>");
    return httpLinks.replace(QRegExp("([#]+[A-Za-z0-9-_]+)"), "<a href=\"tag://\\1\">\\1</a>");
}
