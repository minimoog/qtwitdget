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

#include <QtDebug>
#include <QMessageBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QTimer>
#include "mainwindow.h"
#include "oauth/oauthtwitter.h"
#include "qtwit/hometimeline.h"
#include "qtwit/qtwitupdate.h"
#include "qtwit/qtwitdestroy.h"
#include "qtwit/qtwitfavorites.h"
#include "qtwit/qtwitretweet.h"
#include "qtwit/qtwitfriends.h"
#include "qtwit/mentions.h"
#include "qtwit/qtwitdirectmessages.h"
#include "qtwit/qtwitnewdirectmessage.h"
#include "langchangedialog.h"
#include "qtwit/qtwitverifycredentials.h"
#include "shortenedurl.h"
#include "qtwitview.h"
#include "groupdialog.h"
#include "signalwaiter.h"
#include "tweetlistmodel.h"
#include "tweetlistmodelunread.h"
#include "tweetlistview.h"

MainWindow::MainWindow()
:	m_netManager(new QNetworkAccessManager(this)),
	m_oauthTwitter(new OAuthTwitter(this)),
	m_homeTimeline(new HomeTimeline(this)),
    m_mentions(new Mentions(this)),
	m_twitUpdate(new QTwitUpdate(this)),
	m_twitDestroy(new QTwitDestroy(this)),
    m_twitFavorite(new QTwitFavorites(this)),
    m_twitRetweet(new QTwitRetweet(this)),
	m_timer(new QTimer(this)),
    m_lastStatusId(0),
    m_lastMentionId(0),
    m_lastDirectMessageId(0),
    m_lastMarkedReadStatus(0)
{
	m_oauthTwitter->setNetworkAccessManager(m_netManager);
	m_homeTimeline->setNetworkAccessManager(m_netManager);
    m_mentions->setNetworkAccessManager(m_netManager);
	m_twitUpdate->setNetworkAccessManager(m_netManager);
	m_twitDestroy->setNetworkAccessManager(m_netManager);
    m_twitFavorite->setNetworkAccessManager(m_netManager);
    m_twitRetweet->setNetworkAccessManager(m_netManager);

	m_homeTimeline->setOAuthTwitter(m_oauthTwitter);
    m_mentions->setOAuthTwitter(m_oauthTwitter);
	m_twitUpdate->setOAuthTwitter(m_oauthTwitter);
	m_twitDestroy->setOAuthTwitter(m_oauthTwitter);
    m_twitFavorite->setOAuthTwitter(m_oauthTwitter);
    m_twitRetweet->setOAuthTwitter(m_oauthTwitter);

	ui.setupUi(this);
	ui.updateEdit->setLimit(140);
	
	qApp->setOrganizationName("QTwitdget");

	//connect signals
	connect(ui.updateButton, SIGNAL(clicked()), SLOT(updateButtonClicked()));
	connect(m_homeTimeline, SIGNAL(finishedTimeline()), SLOT(finishedFriendsTimeline()));
    connect(m_homeTimeline, SIGNAL(finishedTimeline()), m_timer, SLOT(start()));
    //on error just start again the timer
    connect(m_homeTimeline, SIGNAL(networkError(QString)), m_timer, SLOT(start()));
    connect(m_mentions, SIGNAL(finishedTimeline()), SLOT(finishedMentions()));
	connect(ui.updateEdit, SIGNAL(overLimit(bool)), ui.updateButton, SLOT(setDisabled(bool)));
	connect(ui.updateEdit, SIGNAL(returnPressed()), ui.updateButton, SLOT(click()));
	connect(m_twitDestroy, SIGNAL(destroyed(qint64)), SLOT(statusDestroyed(qint64)));
    connect(m_twitRetweet, SIGNAL(finished()), SLOT(retweetFinished()));
    connect(ui.shortUrlsButton, SIGNAL(clicked()), ui.updateEdit, SLOT(shortUrls()));
    connect(ui.shortUrlsDMPushButton, SIGNAL(clicked()), ui.directMessageEdit, SLOT(shortUrls()));
	connect(ui.moreButton, SIGNAL(clicked()), this, SLOT(nextStatuses()));
    connect(ui.actionMarkAllRead, SIGNAL(triggered()), this, SLOT(markAllStatusesRead()));
    connect(ui.actionGotoToNextUnread, SIGNAL(triggered()), this, SLOT(gotoNextUnread()));
    connect(ui.userpassButtonBox, SIGNAL(accepted()), this, SLOT(authorize()));
    connect(ui.userpassButtonBox, SIGNAL(rejected()), this, SLOT(cancelUserPassDirectMessage()));
    connect(ui.cancelDMPushButton, SIGNAL(clicked()), this, SLOT(cancelUserPassDirectMessage()));
    connect(ui.sendDMPushButton, SIGNAL(clicked()), this, SLOT(sendDirectMessage()));
    connect(ui.usernameLineEdit, SIGNAL(returnPressed()), this, SLOT(authorize()));
    connect(ui.passwordLineEdit, SIGNAL(returnPressed()), this, SLOT(authorize()));	
    //connect(ui.tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
	connect(ui.actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(ui.actionChangeStyleSheet, SIGNAL(triggered()), SLOT(loadStyleSheet()));
    connect(ui.actionChangeUserPass, SIGNAL(triggered()), this, SLOT(changeUserPass()));
    connect(ui.actionSendDirectMessage, SIGNAL(triggered()), this, SLOT(showDirectMessageEdit()));
	connect(ui.actionCreateGroup, SIGNAL(triggered()), SLOT(createGrouping()));
    
    //timer is single shot, avoid conflict with HomeTimeline
    m_timer->setSingleShot(true);
    m_timer->setInterval(60000);

	m_database = QSqlDatabase::addDatabase("QSQLITE");
	m_firstRun = false;
	
    MainWindow::m_s_netManager = m_netManager;

	setupTrayIcon();

    readSettings();
}

/*!
    Returns network access manager
 */
QNetworkAccessManager* MainWindow::networkAccessManager()
{
    return m_s_netManager;
}

/*!
    Start authorization process
 */
void MainWindow::authorize()
{
    //disconnect timer (fetching tweets)
    // ### TODO: Disconnect other signals ???
    disconnect(m_timer, SIGNAL(timeout()), 0, 0);

    //xAuth flow
    m_oauthTwitter->clearTokens();
    m_oauthTwitter->authorizeXAuth(ui.usernameLineEdit->text(), ui.passwordLineEdit->text());
    ui.usernameLineEdit->clear();
    ui.passwordLineEdit->clear();

	//verify credentials
	QTwitVerifyCredentials vc;
	vc.setNetworkAccessManager(m_netManager);
	vc.setOAuthTwitter(m_oauthTwitter);

    SignalWaiter sigWait(&vc, SIGNAL(finished(bool)));
    vc.verify();
    statusBar()->showMessage(tr("Verifying Twitter credentials."), 2000);

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QTwitdget", "QTwitdget");
    if (sigWait.wait(60000)) {
        QTwitExtUserInfo extUserInfo = vc.userInfo();
        //store settings
        settings.setValue("oauth_token", m_oauthTwitter->oauthToken());
        settings.setValue("oauth_token_secret", m_oauthTwitter->oauthTokenSecret());
        settings.setValue("user_id", extUserInfo.id());

        statusBar()->showMessage("Credentials ok.", 2000);

        while (ui.tabWidget->count())
            closeTab(0);

        startUp();
    } else {
        qDebug() << "Verify credentials timeout";
        settings.remove("oauth_token");
        settings.remove("oauth_token_secret");
        settings.remove("user_id");
    }
}

void MainWindow::changeUserPass()
{
    ui.stackedWidget->setCurrentIndex(1);
    ui.usernameLineEdit->setFocus();
}

void MainWindow::showDirectMessageEdit()
{
    ui.stackedWidget->setCurrentIndex(2);
    ui.directMessageEdit->setFocus();

    // ### TODO: Icons/Avatars

    //fill combo box if it isn't filled
    if (!ui.friendsComboBox->count()) {
        QSqlQuery query;
        query.exec("SELECT screenName FROM friends");

        while (query.next()) {
            ui.friendsComboBox->addItem(query.value(0).toString());
        }
    }
}

void MainWindow::cancelUserPassDirectMessage()
{
    ui.usernameLineEdit->clear();
    ui.passwordLineEdit->clear();
    ui.directMessageEdit->clear();
    ui.stackedWidget->setCurrentIndex(0);
}

void MainWindow::sendDirectMessage()
{
    if (!ui.directMessageEdit->toPlainText().isEmpty()) {
        QTwitNewDirectMessage *dm = new QTwitNewDirectMessage(m_netManager, m_oauthTwitter);
        connect(dm, SIGNAL(finished()), this, SLOT(finishedSendingDirectMessage()));

        dm->sendMessage(ui.friendsComboBox->currentText(), ui.directMessageEdit->toPlainText());
    }
}

void MainWindow::finishedSendingDirectMessage()
{
    QTwitNewDirectMessage *dm = qobject_cast<QTwitNewDirectMessage*>(sender());
    if (dm) {
        ui.statusbar->showMessage(tr("Direct message sent."));
        dm->deleteLater();
    }

    cancelUserPassDirectMessage();
}

void MainWindow::startUp()
{
	//read settings
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QTwitdget", "QTwitdget");
	m_userId = settings.value("user_id", 0).toInt();
	QString oauthToken = settings.value("oauth_token").toString();
	QString oauthTokenSecret = settings.value("oauth_token_secret").toString();

	if(m_userId != 0 && !oauthToken.isEmpty() && !oauthTokenSecret.isEmpty()){
		m_oauthTwitter->setOAuthToken(oauthToken.toUtf8());
		m_oauthTwitter->setOAuthTokenSecret(oauthTokenSecret.toUtf8());

        ui.stackedWidget->setCurrentIndex(0);

		//create or change database according to user id
		createDatabase(QString::number(m_userId));

		if(isDatabaseEmpty()){
			m_firstRun = true;
		} else {
            m_lastStatusId = getLastStatusId();
            m_lastMentionId = getLastMentionId();
            m_lastDirectMessageId = getLastDirectMessageId();
		}

        createDefaultTabs();
        createUserDefinedTabs();

		for(int i = 0; i < ui.tabWidget->count(); ++i)
			updateTab(i);

        //get friends
        QTwitFriends *friends = new QTwitFriends(m_netManager, m_oauthTwitter);
        connect(friends, SIGNAL(finishedFriends(QList<QTwitUser>)), this, SLOT(finishedFriends(QList<QTwitUser>)));
        friends->updateFriends(0, 0, QString(), QString("-1"));

        //start fetching tweets
        connect(m_timer, SIGNAL(timeout()), this, SLOT(updateTimeline()));
		updateTimeline();
    } else {
        changeUserPass();
    }
}

qint64 MainWindow::getLastStatusId()
{
    QSqlQuery query;
    query.exec("SELECT id FROM status ORDER BY id DESC LIMIT 1");

    if (query.next()) {
        return query.value(0).toLongLong();
    }

    return 0;
}

qint64 MainWindow::getLastMentionId()
{
    QSqlQuery query;
    query.exec("SELECT id FROM status WHERE mention == 1 ORDER BY id DESC LIMIT 1");

    if (query.next()) {
        return query.value(0).toLongLong();
    }

    return 0;
}

qint64 MainWindow::getLastDirectMessageId()
{
    QSqlQuery query;
    query.exec("SELECT id FROM directmessages ORDER BY id DESC LIMIT 1");

    if (query.next()) {
        return query.value(0).toLongLong();
    }

    return 0;
}

void MainWindow::updateTimeline()
{
    statusBar()->showMessage(tr("Updating timelines."), 1000);

    QTwitDirectMessages *dm = new QTwitDirectMessages(this);
    dm->setNetworkAccessManager(m_netManager);
    dm->setOAuthTwitter(m_oauthTwitter);
    connect(dm, SIGNAL(finishedDirectMessages(QList<QTwitDMStatus>)),
            this, SLOT(finishedDM(QList<QTwitDMStatus>)));

	if(m_firstRun){
		m_firstRun = false;
        m_homeTimeline->timeline(0);
        m_mentions->timeline(0);
        dm->directMessages(0);
	} else {
        m_homeTimeline->timeline(m_lastStatusId);
        m_mentions->timeline(m_lastMentionId);
        dm->directMessages(m_lastDirectMessageId);
	}
}

void MainWindow::updateButtonClicked()
{
	if(!ui.updateEdit->toPlainText().isEmpty()){
		//trim to 140 characters
		QString updateText = ui.updateEdit->toPlainText().left(140);
		m_twitUpdate->setUpdate(updateText, ui.updateEdit->statusId());
		ui.updateEdit->clear();
	}
}

void MainWindow::createGrouping()
{
	GroupDialog groupDialog;
	groupDialog.exec();

    QString query = createUserQueryString(groupDialog.getGroupList());
    addTimelineTab(query, groupDialog.getGroupName());

    //if save is checked, save group to settings
    if (groupDialog.isSaveGroupingChecked()) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QTwitdget", "QTwitdget");
        int size = settings.beginReadArray("groups");
        settings.endArray();

        settings.beginWriteArray("groups");
        settings.setArrayIndex(size);
        settings.setValue("tabName", groupDialog.getGroupName());
        settings.setValue("query", query);

        settings.endArray();
    }

	ui.tabWidget->setCurrentIndex(ui.tabWidget->count() - 1);
	updateTab(ui.tabWidget->count() - 1);
}

void MainWindow::finishedFriendsTimeline()
{
	QList<QTwitStatus> lastStatuses = m_homeTimeline->statuses();

	if(!lastStatuses.isEmpty()){
        statusBar()->showMessage(QString(tr("New %1 tweets")).arg(lastStatuses.count()));
		//get last status id
		m_lastStatusId = lastStatuses.at(0).id();

		QSqlQuery query;

        query.exec("BEGIN;");

		query.prepare("INSERT OR ABORT INTO status "
			"(created, "
			"id, "
			"text, "
			"replyToStatusId, "
			"replyToUserId, "
			"favorited, "
			"replyToScreenName, "
			"userId, "
			"name, "
			"screenName, "
			"location, "
			"description, "
			"profileImageUrl, "
			"url, "
            "mention, " 
            "isRead) "
			"VALUES "
			"(:created, "
			":id, "
			":text, "
			":replyToStatusId, "
			":replyToUserId, "
			":favorited, "
			":replyToScreenName, "
			":userId, "
			":name, "
			":screenName, "
			":location, "
			":description, "
			":profileImageUrl, "
			":url, "
            ":mention, "
            ":isRead);");

		//it doesn't have to be backwards, I like it this way
		QListIterator<QTwitStatus> i(lastStatuses);
		i.toBack();
		while(i.hasPrevious()){
			QTwitStatus s = i.previous();
			query.bindValue(":created", s.created().toString(Qt::ISODate));
			query.bindValue(":id", s.id());
			query.bindValue(":text", s.text());
			query.bindValue(":replyToStatusId", s.replyToStatusId());
			query.bindValue(":replyToUserId", s.replyToUserId());
			query.bindValue(":favorited", static_cast<int>(s.favorited()));
			query.bindValue(":replyToScreenName", s.replyToScreenName());
			query.bindValue(":userId", s.userId());
			query.bindValue(":name", s.name());
			query.bindValue(":screenName", s.screenName());
			query.bindValue(":location", s.location());
			query.bindValue(":description", s.description());
			query.bindValue(":profileImageUrl", s.profileImageUrl());
			query.bindValue(":url", s.url());
            query.bindValue(":mention", 0); //not a mention
            query.bindValue(":isRead", 0); //not readed
			
			query.exec();
		}

        query.exec("COMMIT;");

		//refresh all tabs
		for(int i = 0; i < ui.tabWidget->count(); ++i)
			updateTab(i);
	}
}

void MainWindow::finishedDM(const QList<QTwitDMStatus> &messages)
{
    QTwitDirectMessages *dm = qobject_cast<QTwitDirectMessages*>(sender());
    if (dm) {
        if (!messages.isEmpty()) {
            m_lastDirectMessageId = messages.at(0).id();

            QSqlQuery query;
            query.exec("BEGIN;");

            query.prepare("INSERT OR REPLACE INTO directmessages "
                          "(id, senderId, text, recipientId, created, "
                          "senderScreenName, recipientScreenName) "
                          "VALUES (:id, :senderId, :text, :recipientId, "
                          ":created, :senderScreenName, :recipientScreenName);");

            foreach (const QTwitDMStatus& msg, messages) {
                query.bindValue(":id", msg.id());
                query.bindValue(":senderId", msg.senderId());
                query.bindValue(":text", msg.text());
                query.bindValue(":recipientId", msg.recipientId());
                query.bindValue(":created", msg.createdAt());
                query.bindValue(":senderScreenName", msg.senderScreenName());
                query.bindValue(":recipientScreenName", msg.recipientScreenName());

                query.exec();
            }
            query.exec("COMMIT;");
        }
        dm->deleteLater();
    }
}

void MainWindow::finishedFriends(const QList<QTwitUser> &friends)
{
    QTwitFriends *friendsReply = qobject_cast<QTwitFriends*>(sender());
    if (friendsReply) {
        QSqlQuery query;
        query.exec("BEGIN;");

        query.prepare("INSERT OR REPLACE INTO friends "
                      "(id, name, screenName, location, description, "
                      "profileImageUrl, url) "
                      "VALUES (:id, :name, :screenName, :location, :description, "
                      ":profileImageUrl, :url);");

        foreach (const QTwitUser& user, friends) {
            query.bindValue(":id", user.id());
            query.bindValue(":name", user.name());
            query.bindValue(":screenName", user.screenName());
            query.bindValue(":location", user.location());
            query.bindValue(":description", user.description());
            query.bindValue(":profileImageUrl", user.profileImageUrl());
            query.bindValue(":url", user.url());

            query.exec();
        }

        query.exec("COMMTI;");
    }
    friendsReply->deleteLater();
}

void MainWindow::finishedMentions()
{
    QList<QTwitStatus> mentions = m_mentions->statuses();

	if(!mentions.isEmpty()){
		//get last status id
		m_lastMentionId = mentions.at(0).id();

		QSqlQuery query;

        query.exec("BEGIN;");

        //replace strategy
		query.prepare("INSERT OR REPLACE INTO status "
			"(created, "
			"id, "
			"text, "
			"replyToStatusId, "
			"replyToUserId, "
			"favorited, "
			"replyToScreenName, "
			"userId, "
			"name, "
			"screenName, "
			"location, "
			"description, "
			"profileImageUrl, "
			"url, "
            "mention, "
            "isRead) "
			"VALUES "
			"(:created, "
			":id, "
			":text, "
			":replyToStatusId, "
			":replyToUserId, "
			":favorited, "
			":replyToScreenName, "
			":userId, "
			":name, "
			":screenName, "
			":location, "
			":description, "
			":profileImageUrl, "
			":url, "
            ":mention, "
            ":isRead);");

		//it doesn't have to be backwards, I like it this way
		QListIterator<QTwitStatus> i(mentions);
		i.toBack();
		while(i.hasPrevious()){
			QTwitStatus s = i.previous();
			query.bindValue(":created", s.created().toString(Qt::ISODate));
			query.bindValue(":id", s.id());
			query.bindValue(":text", s.text());
			query.bindValue(":replyToStatusId", s.replyToStatusId());
			query.bindValue(":replyToUserId", s.replyToUserId());
			query.bindValue(":favorited", static_cast<int>(s.favorited()));
			query.bindValue(":replyToScreenName", s.replyToScreenName());
			query.bindValue(":userId", s.userId());
			query.bindValue(":name", s.name());
			query.bindValue(":screenName", s.screenName());
			query.bindValue(":location", s.location());
			query.bindValue(":description", s.description());
			query.bindValue(":profileImageUrl", s.profileImageUrl());
			query.bindValue(":url", s.url());
            query.bindValue(":mention", 1); //a mention
            query.bindValue(":isRead", 0); //not read
			
			query.exec();
		}

        query.exec("COMMIT;");

		//refresh all tabs
		for(int i = 0; i < ui.tabWidget->count(); ++i)
			updateTab(i);
	}

    // ### WHAT TO DO WITH TIMER???
    //start 60 seconds timer
    //m_timer->start(60000);
}

void MainWindow::statusDestroyed(qint64 id)
{
	QSqlQuery query;
	QString qs = QString("DELETE FROM status WHERE id = %1").arg(id);
	query.exec(qs);
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
    writeSettings();
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
				"url TEXT, "
                "mention INTEGER, "
                "isRead INTEGER, "
                "UNIQUE (id));");
				
    query.exec("CREATE TABLE IF NOT EXISTS directmessages "
               "(key INTEGER PRIMARY KEY, "
               "id INTEGER, "
               "senderId INTEGER, "
               "text TEXT, "
               "recipientId INTEGER, "
               "created DATETIME, "
               "senderScreenName TEXT, "
               "recipientScreenName TEXT, "
               "UNIQUE (id));");

    //temp table for friends
    query.exec("CREATE TEMPORARY TABLE IF NOT EXISTS friends "
               "(key INTEGER PRIMARY KEY, "
               "id INTEGER, "
               "name TEXT, "
               "screenName TEXT, "
               "location TEXT, "
               "description TEXT, "
               "profileImageUrl TEXT, "
               "url TEXT, "
               "UNIQUE (id));");

	query.exec("CREATE TABLE IF NOT EXISTS images (imageName TEXT NOT NULL, image BLOB, UNIQUE (imageName));");
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

void MainWindow::updateTab(int i)
{
    if (i == -1)
        return;

    m_models.at(i)->update();

    //setTabTextUnreadStatuses(i);
}

void MainWindow::closeTab(int i)
{
    //first delete model/view
    TweetListModel* model = m_models.takeAt(i);
    TweetListView* view = model->view();

    delete view;
    delete model;

    //delete scene/view
    QTwitView* statusView = qobject_cast<QTwitView *>(ui.tabWidget->widget(i));
    QGraphicsScene* scene = statusView->scene();

    delete scene;
    delete statusView;

	ui.tabWidget->removeTab(i);
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

void MainWindow::createDefaultTabs()
{
	//default tabs
    addTimelineTab(QString(), tr("Unread"), true);
    addTimelineTab(" 1 == 1 ", tr("Friends"));
    addTimelineTab(QString(" userId == %1 ").arg(m_userId), tr("My twits"));
    addTimelineTab(QString(" mention == 1 "), tr("Mentions"));
}

QString MainWindow::createUserQueryString(const QList<int>& usersId)
{
	//build up query
	QString query;
	foreach(const int& id, usersId){
		query += QString(" userId == %1 OR").arg(id);
	}
	query.chop(2); //remove last OR

    return query;
}

void MainWindow::addTimelineTab(const QString& query, const QString& tabName, bool unread)
{
    QGraphicsScene *statusScene = new QGraphicsScene(this);
    QTwitView *statusView = new QTwitView(this);
    statusView->setScene(statusScene);
    statusView->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    ui.tabWidget->addTab(statusView, tabName);

    TweetListModel *model;

    if (unread)
        model = new TweetListModelUnread(this);
    else
        model = new TweetListModel(this);

    m_models << model;
    TweetListView *viewlist = new TweetListView;
    viewlist->setModel(model);
    statusScene->addItem(viewlist);
    model->setUserid(m_userId);
    model->setAdditionalQuery(query);
    model->update();
    viewlist->resizeWidth(statusView->viewport()->width());

    connect(model, SIGNAL(requestReply(qint64,QString)), ui.updateEdit, SLOT(setReply(qint64,QString)));
    connect(model, SIGNAL(requestReply(qint64,QString)), ui.updateEdit, SLOT(setFocus()));
    connect(model, SIGNAL(requestDelete(qint64)), this, SLOT(reqDelete(qint64)));
    connect(model, SIGNAL(requestRetweet(qint64)), this, SLOT(retweet(qint64)));
    connect(model, SIGNAL(requestFavorited(qint64)), this, SLOT(favorited(qint64)));

    connect(statusView, SIGNAL(resizeWidth(int)), viewlist, SLOT(resizeWidth(int)));
    connect(statusView, SIGNAL(scrollBarMaxPos(bool)), ui.moreButton, SLOT(setEnabled(bool)));
}

void MainWindow::nextStatuses()
{
	int i = ui.tabWidget->currentIndex();

	if(i == -1)
		return;

    m_models.at(i)->nextPage();
}

void MainWindow::favorited(qint64 statusId)
{
    //first check if status is already favorited
    QSqlQuery query;
    QString sq = QString("SELECT favorited FROM status WHERE id = %1;").arg(statusId);
    query.exec(sq);

    //hopefully returns one record
    if (query.next()) {
        if (query.value(0).toBool()) { //already favorited
            m_twitFavorite->destroy(statusId);
            QString sqf = QString("UPDATE status SET favorited = 0 WHERE id = %1;").arg(statusId);
            query.exec(sqf);

            //for (int i = 0; i < ui.tabWidget->count(); ++i) 
                //m_twitScenes.at(i)->setFavorited(statusId, false);

        } else {            //set favorited
            m_twitFavorite->create(statusId);
            QString sqf = QString("UPDATE status SET favorited = 1 WHERE id = %1;").arg(statusId);
            query.exec(sqf);

            //for (int i = 0; i < ui.tabWidget->count(); ++i) 
                //m_twitScenes.at(i)->setFavorited(statusId, true);
        }
    }
}

void MainWindow::reqDelete(qint64 statusId)
{
    m_twitDestroy->deleteStatus(statusId);

    //remove status from other tabs
    for (int i = 0; i < ui.tabWidget->count(); ++i) {
        //m_twitScenes.at(i)->removeStatus(statusId);
    }
}

void MainWindow::retweet(qint64 statusId)
{
    m_twitRetweet->retweet(statusId);
}

void MainWindow::retweetFinished()
{
    QTwitStatus rs = m_twitRetweet->getRetweetedStatus();

    if (rs.id() == 0)
        return;

    ////!!!REFACTOR THIS//////

    QSqlQuery query;

	query.prepare("INSERT OR ABORT INTO status "
        "(created, id, text, replyToStatusId, replyToUserId, "
        "favorited, replyToScreenName, userId, name, screenName, "
        "location, description, profileImageUrl, url, mention) "
        "VALUES (:created, :id, :text, :replyToStatusId, "
        ":replyToUserId, :favorited, :replyToScreenName, :userId, "
        ":name, :screenName, :location, :description, :profileImageUrl, "
        ":url, :mention);");

		query.bindValue(":created", rs.created().toString(Qt::ISODate));
		query.bindValue(":id", rs.id());
		query.bindValue(":text", rs.text());
		query.bindValue(":replyToStatusId", rs.replyToStatusId());
		query.bindValue(":replyToUserId", rs.replyToUserId());
		query.bindValue(":favorited", static_cast<int>(rs.favorited()));
		query.bindValue(":replyToScreenName", rs.replyToScreenName());
		query.bindValue(":userId", rs.userId());
		query.bindValue(":name", rs.name());
		query.bindValue(":screenName", rs.screenName());
		query.bindValue(":location", rs.location());
		query.bindValue(":description", rs.description());
		query.bindValue(":profileImageUrl", rs.profileImageUrl());
		query.bindValue(":url", rs.url());
        query.bindValue(":mention", 0); //not a mention

        query.exec();

        //refresh all tabs
		for(int i = 0; i < ui.tabWidget->count(); ++i)
			updateTab(i);
}

void MainWindow::readSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QTwitdget", "QTwitdget");
    QPoint pos = settings.value("pos", QPoint(200, 50)).toPoint();
    QSize size = settings.value("size", QSize(480, 880)).toSize();
    resize(size);
    move(pos);
}

void MainWindow::writeSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QTwitdget", "QTwitdget");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

void MainWindow::createUserDefinedTabs()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QTwitdget", "QTwitdget");
    int size = settings.beginReadArray("groups");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);

        addTimelineTab(settings.value("query").toString(), settings.value("tabName").toString());
    }

    settings.endArray();
}

void MainWindow::setTweetIdReadDatabase(qint64 id)
{
    QSqlQuery query;
    query.prepare("UPDATE status SET isRead = 1 WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();

    //should return true or false
}

void MainWindow::setTabTextUnreadStatuses(int index)
{
    /*
    TwitTabGroup group = m_twitTabGroups.at(index);
    int numUnread = 0;

    //TODO: move to QTwitScene
    QSqlQuery query;
    QString qr = QString("SELECT COUNT(id) FROM status WHERE isRead = 0 AND %1").arg(group.query());
    query.exec(qr);

    if (query.next())
        numUnread = query.value(0).toInt();

    if (numUnread) {
        QString currentTabText = group.tabName();
        QString numText = QString(" (%1)").arg(numUnread);
        currentTabText += numText;

        ui.tabWidget->setTabText(index, currentTabText);
    } else {
        ui.tabWidget->setTabText(index, group.tabName());
    }
    */
}

void MainWindow::markAllStatusesRead()
{
    QSqlQuery query;
    query.exec("UPDATE status SET isRead = 1 WHERE isRead = 0");

    //update all tabs
    for (int i = 0; i < ui.tabWidget->count(); ++i) {
        m_models.at(i)->markAllRead();
    }
}

void MainWindow::gotoNextUnread()
{
    qint64 idTweet = m_models.at(ui.tabWidget->currentIndex())->nextUnread();
    setTweetIdReadDatabase(idTweet);

    //mark unread on all tabs
    for (int i = 0; i < m_models.count(); ++i)
        m_models.at(i)->markRead(idTweet);

}

MainWindow::~MainWindow()
{

}
