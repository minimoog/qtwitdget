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
#include <QHash>
#include "qtwit/qtwitdmstatus.h"
#include "qtwit/qtwituser.h"
#include "ui_mainwindowform.h"

class QTwitStatus;
class TweetListModel;
class QNetworkAccessManager;
class QDeclarativeView;
class QTimer;
class OAuthTwitter;
class QTwitUpdate;
class QTwitFavorites;
class QDeclarativeComponent;
class TweetQmlListModel;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
    ~MainWindow();
	void startUp();
    static QNetworkAccessManager* networkAccessManager();

Q_INVOKABLE void authorize(const QString& username, const QString& password);
Q_INVOKABLE void updateButtonClicked(const QString& id, const QString& text);

private slots:
    void finishedDM(const QList<QTwitDMStatus>& messages);
    void finishedFriends(const QList<QTwitUser>& friends);
    void finishedSendingDirectMessage();
	void updateTimeline();
	void statusDestroyed(qint64 id);
	void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void languageChanged();
    void changeUserPass();
    void showDirectMessageEdit();
    void sendDirectMessage();

	void favorited(qint64 statusId);

protected:
	void closeEvent(QCloseEvent *e);
	void changeEvent(QEvent *e);

private:
	void setupTrayIcon();
	void createDatabase(const QString& databaseName);
	void updateCurrentPage();
    void createDeclarativeView();
	bool isDatabaseEmpty();
    void readSettings();
    void writeSettings();
    qint64 getLastStatusId();
    qint64 getLastDirectMessageId();

	Ui::MainWindowForm ui;

	QNetworkAccessManager *m_netManager;
    static QNetworkAccessManager *m_s_netManager;
	OAuthTwitter *m_oauthTwitter;
	QTwitUpdate *m_twitUpdate;
    QTwitFavorites *m_twitFavorite;

    TweetQmlListModel* m_tweetListModel;

	bool m_firstRun;

	qint64 m_lastStatusId;
    qint64 m_lastDirectMessageId;
    qint64 m_lastMarkedReadStatus;
    int m_userId;   // ### TODO: Change to 64 bits

	QSqlDatabase m_database;

	QSystemTrayIcon *m_trayIcon;
	QMenu *m_trayIconMenu;
};

#endif //MAINWINDOW_H
