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

#include "searchqmllistmodel.h"
#include <QDateTime>
#include <QRegExp>
#include "qtweetlib/oauthtwitter.h"
#include "qtweetlib/qtweetsearchresult.h"
#include "qtweetlib/qtweetsearch.h"

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
 *  Constructor
 */
SearchQmlListModel::SearchQmlListModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[SinceTimeRole] = "sinceTimeRole";
    roles[ScreenNameRole] = "screenNameRole";
    roles[AvatarUrlRole] = "avatarUrlRole";
    roles[StatusTextRole] = "statusTextRole";
    roles[StatusIdRole] = "statusIdRole";
    setRoleNames(roles);
}

/**
 *  Constructor
 */
SearchQmlListModel::SearchQmlListModel(OAuthTwitter *oauthTwitter, QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[SinceTimeRole] = "sinceTimeRole";
    roles[ScreenNameRole] = "screenNameRole";
    roles[AvatarUrlRole] = "avatarUrlRole";
    roles[StatusTextRole] = "statusTextRole";
    roles[StatusIdRole] = "statusIdRole";
    setRoleNames(roles);

    m_oauthTwitter = oauthTwitter;
}

/**
 *  Sets ouathtwitter object
 */
void SearchQmlListModel::setOAuthTwitter(OAuthTwitter *oauthTwitter)
{
    m_oauthTwitter = oauthTwitter;
}

/**
 *  @reimp
 */
int SearchQmlListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_searchPageResult.results().count();
}

/**
 *  @reimp
 */
QVariant SearchQmlListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > m_searchPageResult.results().count())     // ### TODO
        return QVariant();

    const QTweetSearchResult& searchResult = m_searchPageResult.results().at(index.row());

    if (role == SinceTimeRole)
        return SinceTimeString(searchResult.createdAt());
    else if (role == ScreenNameRole)
        return searchResult.fromUser();
    else if (role == AvatarUrlRole)
        return searchResult.profileImageUrl();
    else if (role == StatusTextRole)
        return searchResult.text();
    else if (role == StatusIdRole)
        return QString::number(searchResult.id());

    return QVariant();
}

/**
 *  Starts searching
 *  @param query search query
 */
void SearchQmlListModel::startSearch(const QString &query)
{
    QTweetSearch *twitterSearch = new QTweetSearch(m_oauthTwitter, this);
    twitterSearch->setAuthenticationEnabled(false);

    twitterSearch->start(query);
    connect(twitterSearch, SIGNAL(parsedPageResults(QTweetSearchPageResults)),
            this, SLOT(finishedSearch(QTweetSearchPageResults)));
}

/**
 *  Called when twitter search is finished
 */
void SearchQmlListModel::finishedSearch(const QTweetSearchPageResults &pageResults)
{
    QTweetSearch *twitterSearch = qobject_cast<QTweetSearch*>(sender());

    if (twitterSearch) {
        beginResetModel();

        QList<QTweetSearchResult> taggedResults;
        QList<QTweetSearchResult> results = pageResults.results();

        foreach(const QTweetSearchResult& searchResult, results) {
            QString taggedText = addTags(searchResult.text());
            QTweetSearchResult copyResult(searchResult);
            copyResult.setText(taggedText);

            taggedResults.append(copyResult);
        }

        QTweetSearchPageResults copyPageResults(pageResults);
        copyPageResults.setResults(taggedResults);
        m_searchPageResult = copyPageResults;

        endResetModel();

        twitterSearch->deleteLater();
    }
}

QString SearchQmlListModel::addTags(const QString &text)
{
    QString tweet(text);
    QString mentions = tweet.replace(QRegExp("(@[a-zA-Z0-9_]+)"),
                                     "<b>\\1</b>");
    QString httpLinks = mentions.replace(QRegExp("(\\b(https?|ftp)://[-A-Z0-9+&@#/%?=~_|!:,.;]*[-A-Z0-9+&@#/%=~_|])", Qt::CaseInsensitive),
                                            "<b>\\1</b>");
    QString hashtags = httpLinks.replace(QRegExp("([#]+[A-Za-z0-9-_]+)"),
                                         "<b>\\1</b>");
    return hashtags;
}
