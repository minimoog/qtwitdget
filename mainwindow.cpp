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
#include <QDeclarativeEngine>
#include <QDeclarativeComponent>
#include <QDeclarativeContext>
#include <QGraphicsObject>
#include <QDeclarativeProperty>
#include <QDeclarativeView>
#include "mainwindow.h"
#include "oauth/oauthtwitter.h"
#include "qtwit/hometimeline.h"
#include "qtwit/qtwitupdate.h"
#include "qtwit/qtwitdestroy.h"
#include "qtwit/qtwitfavorites.h"
#include "qtwit/qtwitfriends.h"
#include "qtwit/qtwitdirectmessages.h"
#include "qtwit/qtwitnewdirectmessage.h"
#include "langchangedialog.h"
#include "qtwit/qtwitverifycredentials.h"
#include "shortenedurl.h"
#include "groupdialog.h"
#include "signalwaiter.h"
#include "tweetqmllistmodel.h"

MainWindow::MainWindow()
:	m_netManager(new QNetworkAccessManager(this)),
	m_oauthTwitter(new OAuthTwitter(this)),
	m_homeTimeline(new HomeTimeline(this)),
	m_twitUpdate(new QTwitUpdate(this)),
	m_twitDestroy(new QTwitDestroy(this)),
    m_twitFavorite(new QTwitFavorites(this)),
	m_timer(new QTimer(this)),
    m_lastStatusId(0),
    m_lastDirectMessageId(0),
    m_lastMarkedReadStatus(0)
{
	m_oauthTwitter->setNetworkAccessManager(m_netManager);
	m_homeTimeline->setNetworkAccessManager(m_netManager);
	m_twitUpdate->setNetworkAccessManager(m_netManager);
	m_twitDestroy->setNetworkAccessManager(m_netManager);
    m_twitFavorite->setNetworkAccessManager(m_netManager);

	m_homeTimeline->setOAuthTwitter(m_oauthTwitter);
	m_twitUpdate->setOAuthTwitter(m_oauthTwitter);
	m_twitDestroy->setOAuthTwitter(m_oauthTwitter);
    m_twitFavorite->setOAuthTwitter(m_oauthTwitter);

	ui.setupUi(this);
	ui.updateEdit->setLimit(140);
	
	qApp->setOrganizationName("QTwitdget");

	//connect signals
	connect(ui.updateButton, SIGNAL(clicked()), SLOT(updateButtonClicked()));
	connect(m_homeTimeline, SIGNAL(finishedTimeline()), SLOT(finishedFriendsTimeline()));
    connect(m_homeTimeline, SIGNAL(finishedTimeline()), m_timer, SLOT(start()));
    //on error just start again the timer
    connect(m_homeTimeline, SIGNAL(networkError(QString)), m_timer, SLOT(start()));
	connect(ui.updateEdit, SIGNAL(overLimit(bool)), ui.updateButton, SLOT(setDisabled(bool)));
	connect(ui.updateEdit, SIGNAL(returnPressed()), ui.updateButton, SLOT(click()));
	connect(m_twitDestroy, SIGNAL(destroyed(qint64)), SLOT(statusDestroyed(qint64)));
    connect(ui.shortUrlsButton, SIGNAL(clicked()), ui.updateEdit, SLOT(shortUrls()));
    connect(ui.shortUrlsDMPushButton, SIGNAL(clicked()), ui.directMessageEdit, SLOT(shortUrls()));
	connect(ui.moreButton, SIGNAL(clicked()), this, SLOT(nextStatuses()));
    connect(ui.userpassButtonBox, SIGNAL(accepted()), this, SLOT(authorize()));
    connect(ui.userpassButtonBox, SIGNAL(rejected()), this, SLOT(cancelUserPassDirectMessage()));
    connect(ui.cancelDMPushButton, SIGNAL(clicked()), this, SLOT(cancelUserPassDirectMessage()));
    connect(ui.sendDMPushButton, SIGNAL(clicked()), this, SLOT(sendDirectMessage()));
    connect(ui.usernameLineEdit, SIGNAL(returnPressed()), this, SLOT(authorize()));
    connect(ui.passwordLineEdit, SIGNAL(returnPressed()), this, SLOT(authorize()));	
	connect(ui.actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
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
            m_lastDirectMessageId = getLastDirectMessageId();
		}

        createDefaultTabs();
        createUserDefinedTabs();

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
        dm->directMessages(0);
	} else {
        m_homeTimeline->timeline(m_lastStatusId);
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

        // ### TODO: Refresh?
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

        query.exec("COMMIT;");
    }
    friendsReply->deleteLater();
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

    // ### right place????
    //qmlRegisterType<TweetQmlListModel>("TweetLib", 1, 0, "TweetListModel");
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

void MainWindow::updateDeclarativeView()
{
    m_tweetListModel->update();
}

void MainWindow::createDefaultTabs()
{
    addTimelineTab(" 1 == 1 ", tr("Friends"));
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

void MainWindow::addTimelineTab(const QString& query, const QString& tabName, bool unread, bool closable)
{
    m_tweetListModel = new TweetQmlListModel();
    ui.declarativeView->rootContext()->setContextProperty("tweetListModel", m_tweetListModel);
    ui.declarativeView->rootContext()->setContextProperty("viewWidth", 500);

    ui.declarativeView->setSource(QUrl::fromLocalFile("qml/TweetList.qml"));
}

void MainWindow::nextStatuses()
{
    //QTwitView* statusView = qobject_cast<QTwitView *>(ui.tabWidget->currentWidget());

    //if (!statusView)
    //	return;
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

    // ### TODO: refresh tabs?
}

void MainWindow::retweet(qint64 statusId)
{
    //m_twitRetweet->retweet(statusId);

    // ### TODO: Temp solution?

    QSqlQuery query;
    query.prepare("SELECT text, screenName FROM status WHERE id = :id");
    query.bindValue(":id", statusId);
    query.exec();

    if (query.next())
        ui.updateEdit->setRetweet(query.value(0).toString(), query.value(1).toString());
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

MainWindow::~MainWindow()
{

}
