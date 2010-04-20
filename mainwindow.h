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
 
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QtSql>
#include "qtwit/qtwitdmstatus.h"
#include "ui_mainwindowform.h"

class QTwitStatus;
class TweetListModel;
class QNetworkAccessManager;
class QTimer;
class OAuthTwitter;
class HomeTimeline;
class QTwitUpdate;
class QTwitDestroy;
class QTwitFavorites;
class QTwitRetweet;
class Mentions;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
    ~MainWindow();
	void startUp();
    static QNetworkAccessManager* networkAccessManager();

private slots:
	void finishedFriendsTimeline();
    void finishedDM(const QList<QTwitDMStatus>& messages);
	void updateTimeline();
	void statusDestroyed(qint64 id);
    void finishedMentions();
	void updateButtonClicked();
	void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
	void createGrouping();
	void loadStyleSheet();
	void closeTab(int i);
	void nextStatuses();
    void markAllStatusesRead();
    void gotoNextUnread();
    void languageChanged();
    void authorize();

	void favorited(qint64 statusId);
    void reqDelete(qint64 statusId);
    void retweet(qint64 statusId);
    void retweetFinished();

protected:
	void closeEvent(QCloseEvent *e);
	void changeEvent(QEvent *e);

private:
	void setupTrayIcon();
	void createDatabase(const QString& databaseName);
	void updateCurrentPage();
    void createDefaultTabs();
    QString createUserQueryString(const QList<int>& usersId);
    void updateTab(int i);
    void addTimelineTab(const QString& query, const QString& tabName, bool unread = false);
    void setTabTextUnreadStatuses(int indexTab);
	bool isDatabaseEmpty();
    void readSettings();
    void writeSettings();
    void createUserDefinedTabs();
    void setTweetIdReadDatabase(qint64 id);
    qint64 getLastStatusId();
    qint64 getLastMentionId();

	Ui::MainWindowForm ui;

	QNetworkAccessManager *m_netManager;
    static QNetworkAccessManager *m_s_netManager;
	OAuthTwitter *m_oauthTwitter;
	HomeTimeline *m_homeTimeline;
    Mentions *m_mentions;
	QTwitUpdate *m_twitUpdate;
	QTwitDestroy *m_twitDestroy;
    QTwitFavorites *m_twitFavorite;
    QTwitRetweet *m_twitRetweet;
	QTimer *m_timer;

	bool m_firstRun;

	qint64 m_lastStatusId;
    qint64 m_lastMentionId;
    qint64 m_lastMarkedReadStatus;
    QList<TweetListModel*> m_models;
	int m_userId;

	QSqlDatabase m_database;

	QSystemTrayIcon *m_trayIcon;
	QMenu *m_trayIconMenu;
};

#endif //MAINWINDOW_H
