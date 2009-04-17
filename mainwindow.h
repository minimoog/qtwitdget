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
 * Contact e-mail: Antonie Jovanoski <minimoog77@gmail.com>
 */
 
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QtSql>
#include <QNetworkAccessManager>
#include "ui_mainwindowform.h"
#include "qtwit/qtwitfriendstimeline.h"
#include "qtwit/qtwitdestroy.h"
#include "qtwit/qtwitupdate.h"
#include "oauth/oauthtwitter.h"
#include "imagedownloader.h"
#include "twittabgroup.h"

class QTwitStatus;

/*! Class MainWindow */
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	/*! Constructor */
	MainWindow();
	
private slots:
	void authorize();
	/*! Called when getting the friends timeline (statuses) is finished */
	void finishedFriendsTimeline();
	/*! Periodically called for updating timeline */
	void updateTimeline();
	/*! Called when user request to destroy (delete) status */
	/*! \param id ID of the status */
	void statusDestroyed(int id);
	void updateButtonClicked();
	void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
	void languageChanged();
	void loadStyleSheet();
	void showTab(int i);
	void closeTab(int i);

protected:
	void closeEvent(QCloseEvent *e);
	void changeEvent(QEvent *e);

private:
	void setupTrayIcon();
	void startUp();
	void createDatabase(const QString& databaseName);
	void updateCurrentPage();
	void createTwitGroups();
	void createTabs();
	bool isDatabaseEmpty();
	void refreshPages();

	Ui::MainWindowForm ui;

	QNetworkAccessManager *m_netManager;
	OAuthTwitter *m_oauthTwitter;
	QTwitFriendsTimeline *m_twitFriendsTimeline;
	QTwitUpdate *m_twitUpdate;
	QTwitDestroy *m_twitDestroy;
	ImageDownloader *m_imageDownloader;
	QTimer *m_timer;

	QList<QTwitStatus> m_statuses;
	bool m_firstRun;

	QList<TwitTabGroup> m_twitTabGroups;
	int m_lastStatusId;

	QSqlDatabase m_database;

	QSystemTrayIcon *m_trayIcon;
	QMenu *m_trayIconMenu;
};

#endif //MAINWINDOW_H