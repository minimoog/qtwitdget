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

#include <QMessageBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QEventLoop>
#include "mainwindow.h"
#include "langchangedialog.h"
#include "qtwit/qtwitverifycredentials.h"
#include "qtwitscene.h"
#include "shortenedurl.h"
#include "qtwitview.h"

MainWindow::MainWindow()
:	m_netManager(new QNetworkAccessManager(this)),
	m_oauthTwitter(new OAuthTwitter(this)),
	m_twitFriendsTimeline(new QTwitFriendsTimeline(this)),
	m_twitUpdate(new QTwitUpdate(this)),
	m_twitDestroy(new QTwitDestroy(this)),
	m_imageDownloader(new ImageDownloader(this)),
	m_timer(new QTimer(this))
{
	m_oauthTwitter->setNetworkAccessManager(m_netManager);
	m_twitFriendsTimeline->setNetworkAccessManager(m_netManager);
	m_twitUpdate->setNetworkAccessManager(m_netManager);
	m_twitDestroy->setNetworkAccessManager(m_netManager);
	m_imageDownloader->setNetworkAccessManager(m_netManager);

	m_twitFriendsTimeline->setOAuthTwitter(m_oauthTwitter);
	m_twitUpdate->setOAuthTwitter(m_oauthTwitter);
	m_twitDestroy->setOAuthTwitter(m_oauthTwitter);

	ui.setupUi(this);
	ui.groupWidget->hide();
	ui.updateEdit->setLimit(140);
	
	//remove tab that Designer creates
	QWidget* tab1Widget = ui.tabWidget->widget(0);
	ui.tabWidget->clear();
	delete tab1Widget;

	qApp->setOrganizationName("QTwitdget");

	//connect signals
	connect(ui.updateButton, SIGNAL(clicked()), SLOT(updateButtonClicked()));
	connect(m_twitFriendsTimeline, SIGNAL(finished()), SLOT(finishedFriendsTimeline()));
	connect(ui.updateEdit, SIGNAL(overLimit(bool)), ui.updateButton, SLOT(setDisabled(bool)));
	connect(ui.updateEdit, SIGNAL(returnPressed()), ui.updateButton, SLOT(click()));
	connect(m_twitDestroy, SIGNAL(destroyed(int)), SLOT(statusDestroyed(int)));
	
	connect(ui.tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

	connect(m_timer, SIGNAL(timeout()), this, SLOT(updateTimeline()));

	connect(ui.actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(ui.actionChangeStyleSheet, SIGNAL(triggered()), SLOT(loadStyleSheet()));
	connect(ui.actionAuthorize, SIGNAL(triggered()), SLOT(authorize()));

	m_database = QSqlDatabase::addDatabase("QSQLITE");
	m_firstRun = false;
	
	setupTrayIcon();
}

void MainWindow::authorize()
{
	//oauth flow
	m_oauthTwitter->authorize();

	//verify credentials
	QTwitVerifyCredentials vc;
	vc.setNetworkAccessManager(m_netManager);
	vc.setOAuthTwitter(m_oauthTwitter);

	QEventLoop q;
	QTimer tT;
	tT.setSingleShot(true);

	connect(&tT, SIGNAL(timeout()), &q, SLOT(quit()));
	connect(&vc, SIGNAL(finished(bool)), &q, SLOT(quit()));

	vc.verify();
	tT.start(60000);
	q.exec();

	if(tT.isActive()){
		tT.stop();
		QTwitExtUserInfo extUserInfo = vc.userInfo();
		//store settings
		QSettings settings;
		settings.setValue("oauth_token", m_oauthTwitter->oauthToken());
		settings.setValue("oauth_token_secret", m_oauthTwitter->oauthTokenSecret());
		settings.setValue("user_id", extUserInfo.id());

		startUp();
	} else {
		//timeout;
	}
}

void MainWindow::startUp()
{
	//read settings
	QSettings settings;
	int userId = settings.value("user_id", 0).toInt();
	QString oauthToken = settings.value("oauth_token").toString();
	QString oauthTokenSecret = settings.value("oauth_token_secret").toString();

	if(userId != 0 && !oauthToken.isEmpty() && !oauthTokenSecret.isEmpty()){
		m_oauthTwitter->setOAuthToken(oauthToken.toUtf8());
		m_oauthTwitter->setOAuthTokenSecret(oauthTokenSecret.toUtf8());

		//create or change database according to user id
		createDatabase(QString::number(userId));

		if(isDatabaseEmpty()){
			m_firstRun = true;
		} else {
			//determine last status id from which to update
			QSqlQuery query;
			query.exec("SELECT id FROM status ORDER BY created DESC LIMIT 1");
			if(query.next())
				m_lastStatusId = query.value(0).toInt();
		}

		createTwitGroups();
		createTabs();

		//ui.twitsWidget->setUserid(userId);

		for(int i = 0; i < ui.tabWidget->count(); ++i)
			refreshTab(i);

		//start update timer
		m_timer->start(60000);

		updateTimeline();
	}
}

void MainWindow::updateTimeline()
{
	if(m_firstRun){
		m_firstRun = false;
		m_twitFriendsTimeline->update(0, 0, 200);
	} else {
		m_twitFriendsTimeline->update(m_lastStatusId, 0, 200);
	}
}

void MainWindow::updateButtonClicked()
{
	if(!ui.updateEdit->toPlainText().isEmpty()){
		//trim to 140 characters
		QString updateText = ui.updateEdit->toPlainText().left(140);
		m_twitUpdate->setUpdate(updateText);
		ui.updateEdit->clear();
	}
}

void MainWindow::finishedFriendsTimeline()
{
	QList<QTwitStatus> lastStatuses = m_twitFriendsTimeline->getStatuses();

	if(!lastStatuses.isEmpty()){
		//get last status id
		m_lastStatusId = lastStatuses.at(0).id();

		QSqlQuery query;
		query.prepare("INSERT INTO status "
			"(created, id, text, screenName, profileImageUrl, source, replyToStatusId, replyToUserId, replyToScreenName) "
			"VALUES "
			"(:created, :id, :text, :screenName, :profileImageUrl, :source, :replyToStatusId, :replyToUserId, :replyToScreenName)");

		//it doesn't have to be backwards, I like it this way
		QListIterator<QTwitStatus> i(lastStatuses);
		i.toBack();
		while(i.hasPrevious()){
			QTwitStatus s = i.previous();
			query.bindValue(":created", s.created().toString(Qt::ISODate));
			query.bindValue(":id", s.id());
			query.bindValue(":text", s.text());
			query.bindValue(":screenName", s.screenName());
			query.bindValue(":profileImageUrl", s.profileImageUrl());
			query.bindValue(":source", s.source());
			query.bindValue(":replyToStatusId", s.replyToStatusId());
			query.bindValue(":replyToUserId", s.replyToUserId());
			query.bindValue(":replyToScreenName", s.replyToScreenName());
			query.exec();
		}

		query.finish();

		//refresh all tabs
		for(int i = 0; i < ui.tabWidget->count(); ++i)
			refreshTab(i);
	}
}

void MainWindow::statusDestroyed(int id)
{
	QSqlQuery query;
	QString qs = QString("DELETE FROM status WHERE id = %1").arg(id);
	query.exec(qs);

	refreshTab(ui.tabWidget->currentIndex());
}

void MainWindow::languageChanged()
{
	LangChangeDialog lcd(this);
	lcd.exec();
}

void MainWindow::setupTrayIcon()
{
	connect(ui.actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

	m_trayIconMenu = new QMenu(this);
	m_trayIconMenu->addAction(ui.actionMinimize);
	m_trayIconMenu->addAction(ui.actionRestore);
	m_trayIconMenu->addSeparator();
	m_trayIconMenu->addAction(ui.actionQuit);

	m_trayIcon = new QSystemTrayIcon(this);
	m_trayIcon->setIcon(QIcon(":/images/twitter_32.ico"));
	m_trayIcon->setContextMenu(m_trayIconMenu);
	m_trayIcon->show();

	connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch(reason){
		case QSystemTrayIcon::Trigger:
		case QSystemTrayIcon::DoubleClick:
			showNormal();
			break;
		default:
			;
	}
}

void MainWindow::closeEvent(QCloseEvent *e)
{
	hide();
	e->ignore();
}

void MainWindow::changeEvent(QEvent *e)
{
	if(e->type() == QEvent::LanguageChange)
		ui.retranslateUi(this);

	QMainWindow::changeEvent(e);
}

void MainWindow::createDatabase(const QString& databaseName)
{
	QString workdir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);

	QDir dir;
	dir.setPath(workdir);
	if(!dir.exists())
		dir.mkpath(".");

	//close any previous open database
	if(m_database.isOpen())
		m_database.close();
	
	m_database.setDatabaseName(workdir + databaseName + ".sqlite");
	m_database.open();

	QSqlQuery query;
	query.exec("CREATE TABLE IF NOT EXISTS status " 
				"(key INTEGER PRIMARY KEY, " 
				"created DATETIME, "
				"id INTEGER, "
				"text TEXT, "
				"source TEXT, "
				"replyToStatusId INTEGER, "
				"replyToUserId INTEGER, "
				"favorited INTEGER, "
				"replyToScreenName TEXT, "
				"userId INTEGER, "
				"name TEXT, "
				"screenName TEXT, "
				"location TEXT, "
				"description TEXT, "
				"profileImageUrl TEXT, "
				"url TEXT, ",
				"followersCount INTEGER);");
				
	query.exec("CREATE INDEX IF NOT EXISTS index_id ON status (id);");
}

bool MainWindow::isDatabaseEmpty()
{
	QSqlQuery query;
	query.exec("SELECT key FROM status LIMIT 1");
	while(query.next()){
		query.value(0).toInt();
		return false;
	}

	return true;
}

void MainWindow::refreshTab(int i)
{
	if(i == -1)
		return;

	TwitTabGroup tg = m_twitTabGroups.at(i);

	QSqlQuery query;
	QString sq = QString("SELECT created, id, text, screenName, profileImageUrl, source "
		"FROM status "
		"WHERE %1 "
		"ORDER BY id DESC "
		"LIMIT %2;").arg(tg.query()).arg(tg.numStatuses());

	query.exec(sq);

	m_statuses.clear();

	while(query.next()){
		QTwitStatus s;
		s.setCreated(query.value(0).toDateTime());
		s.setId(query.value(1).toInt());
		s.setText(query.value(2).toString());
		s.setScreenName(query.value(3).toString());
		s.setProfileImageUrl(query.value(4).toString());
		s.setSource(query.value(5).toString());
		m_statuses.append(s);		
	}

	QTwitScene *statusScene = m_twitScenes.at(i);
	statusScene->setStatuses(m_statuses);
}

void MainWindow::closeTab(int i)
{
	QWidget *widget = ui.tabWidget->widget(i);
	ui.tabWidget->removeTab(i);
	delete widget;

	m_twitTabGroups.removeAt(i);
}

void MainWindow::loadStyleSheet()
{
	QString fnStyleSheet = QFileDialog::getOpenFileName(this);

	if(!fnStyleSheet.isNull()){
		QFile file(fnStyleSheet);
		if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
			return;

		QTextStream in(&file);
		QString styleSheet = in.readAll();
		
		qApp->setStyleSheet(styleSheet);
	}
}

void MainWindow::createTwitGroups()
{
	m_twitTabGroups.clear();

	//just testing
	TwitTabGroup allfriends;
	allfriends.setTabName(tr("Friends"));
	allfriends.setQuery(" screenName != \"minimoog77\" ");

	TwitTabGroup myTwits;
	myTwits.setTabName(tr("My twits"));
	myTwits.setQuery(" screenName == \"minimoog77\" ");

	m_twitTabGroups.append(allfriends);
	m_twitTabGroups.append(myTwits);
}

void MainWindow::createTabs()
{	
	//add tabs
	QListIterator<TwitTabGroup> iter(m_twitTabGroups);
	while(iter.hasNext()){
		TwitTabGroup tg = iter.next();
		
		QTwitScene *statusScene = new QTwitScene(this);
		statusScene->setImageDownloader(m_imageDownloader);
		m_twitScenes << statusScene;

		//connect(statusesWidget, SIGNAL(requestReplyStatus(const QString&, int)), 
		//	m_twitUpdate, SLOT(setUpdate(const QString&, int)));
		//connect(statusesWidget, SIGNAL(requestDeleteStatus(int)), m_twitDestroy, SLOT(deleteStatus(int)));

		QTwitView *statusView = new QTwitView;
		statusView->setScene(statusScene);
		connect(statusView, SIGNAL(scrollBarMaxPos()), this, SLOT(nextStatuses()));
		ui.tabWidget->addTab(statusView, tg.tabName());
	}
}

void MainWindow::nextStatuses()
{
	int i = ui.tabWidget->currentIndex();

	if(i == -1)
		return;

	m_twitTabGroups[i].increasePage();

	refreshTab(i);
}