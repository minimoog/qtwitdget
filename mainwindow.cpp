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
#include <QCloseEvent>
#include "mainwindow.h"
#include "oauth/oauthtwitter.h"
#include "qtwit/hometimeline.h"
#include "qtwit/qtwitupdate.h"
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
	m_twitUpdate(new QTwitUpdate(this)),
    m_twitFavorite(new QTwitFavorites(this)),
    m_lastStatusId(0),
    m_lastDirectMessageId(0),
    m_lastMarkedReadStatus(0)
{
	m_oauthTwitter->setNetworkAccessManager(m_netManager);
	m_twitUpdate->setNetworkAccessManager(m_netManager);
    m_twitFavorite->setNetworkAccessManager(m_netManager);

	m_twitUpdate->setOAuthTwitter(m_oauthTwitter);
    m_twitFavorite->setOAuthTwitter(m_oauthTwitter);

	ui.setupUi(this);

	qApp->setOrganizationName("QTwitdget");

	//connect signals
	connect(ui.actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui.actionChangeUserPass, SIGNAL(triggered()), this, SLOT(changeUserPass()));

	m_database = QSqlDatabase::addDatabase("QSQLITE");
	m_firstRun = false;
	
    MainWindow::m_s_netManager = m_netManager;

	setupTrayIcon();

    readSettings();

    createDeclarativeView();
}

/*!
    Returns network access manager
 */
QNetworkAccessManager* MainWindow::networkAccessManager()
{
    return m_s_netManager;
}

void MainWindow::authorize(const QString &username, const QString &password)
{
    m_oauthTwitter->clearTokens();
    m_oauthTwitter->authorizeXAuth(username, password);

    QTwitVerifyCredentials vc;
    vc.setNetworkAccessManager(m_netManager);
    vc.setOAuthTwitter(m_oauthTwitter);

    SignalWaiter sigWait(&vc, SIGNAL(finished(bool)));
    vc.verify();

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QTwitdget", "QTwitdget");
    if (sigWait.wait(60000)) {
        QTwitExtUserInfo extUserInfo = vc.userInfo();
        //store settings
        settings.setValue("oauth_token", m_oauthTwitter->oauthToken());
        settings.setValue("oauth_token_secret", m_oauthTwitter->oauthTokenSecret());
        settings.setValue("user_id", extUserInfo.id());

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
    QGraphicsObject *obj = ui.declarativeView->rootObject();
    obj->setProperty("authed", false);
}

void MainWindow::showDirectMessageEdit()
{
    // ### TODO: Icons/Avatars

    //fill combo box if it isn't filled
//    if (!ui.friendsComboBox->count()) {
//        QSqlQuery query;
//        query.exec("SELECT screenName FROM friends");

//        while (query.next()) {
//            ui.friendsComboBox->addItem(query.value(0).toString());
//        }
//    }
}

void MainWindow::sendDirectMessage()
{
//    if (!ui.directMessageEdit->toPlainText().isEmpty()) {
//        QTwitNewDirectMessage *dm = new QTwitNewDirectMessage(m_netManager, m_oauthTwitter);
//        connect(dm, SIGNAL(finished()), this, SLOT(finishedSendingDirectMessage()));

//        dm->sendMessage(ui.friendsComboBox->currentText(), ui.directMessageEdit->toPlainText());
//    }
}

void MainWindow::finishedSendingDirectMessage()
{
//    QTwitNewDirectMessage *dm = qobject_cast<QTwitNewDirectMessage*>(sender());
//    if (dm) {
//        ui.statusbar->showMessage(tr("Direct message sent."));
//        dm->deleteLater();
//    }
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

		//create or change database according to user id
		createDatabase(QString::number(m_userId));

		if(isDatabaseEmpty()){
			m_firstRun = true;
		} else {
            m_lastStatusId = getLastStatusId();
            m_lastDirectMessageId = getLastDirectMessageId();
		}

        //get friends
        QTwitFriends *friends = new QTwitFriends(m_netManager, m_oauthTwitter);
        connect(friends, SIGNAL(finishedFriends(QList<QTwitUser>)), this, SLOT(finishedFriends(QList<QTwitUser>)));
        friends->updateFriends(0, 0, QString(), QString("-1"));

        //show/animate tweets list page
        QGraphicsObject *obj = ui.declarativeView->rootObject();
        obj->setProperty("authed", true);

        //show last tweets from database
        m_tweetListModel->loadTweetsFromDatabase();
        //start fetching
        m_tweetListModel->startUpdateTimelines();

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
    QTwitDirectMessages *dm = new QTwitDirectMessages(this);
    dm->setNetworkAccessManager(m_netManager);
    dm->setOAuthTwitter(m_oauthTwitter);
    connect(dm, SIGNAL(finishedDirectMessages(QList<QTwitDMStatus>)),
            this, SLOT(finishedDM(QList<QTwitDMStatus>)));

	if(m_firstRun){
		m_firstRun = false;
        dm->directMessages(0);
	} else {
        dm->directMessages(m_lastDirectMessageId);
	}
}

void MainWindow::updateButtonClicked(const QString &id, const QString &text)
{
    bool ok;

    qint64 tweetid = id.toLongLong(&ok);

    if (!ok)
        qDebug() << "MainWindow::updateButtonClicked : bad string id";
        
    QString updateText = text.left(140);

    if (updateText.isEmpty()) {
        qDebug() << "MainWindow::updateButtonClicked: empty update text";
        return;
    }

    m_twitUpdate->setUpdate(updateText, tweetid);
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

        friendsReply->deleteLater();
    }
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

void MainWindow::createDeclarativeView()
{
    m_tweetListModel = new TweetQmlListModel();
    m_tweetListModel->setUserID(m_userId);
    m_tweetListModel->setNetworkAccessManager(m_netManager);
    m_tweetListModel->setOAuthTwitter(m_oauthTwitter);

    ui.declarativeView->rootContext()->setContextProperty("tweetListModel", m_tweetListModel);
    ui.declarativeView->rootContext()->setContextProperty("viewWidth", 500);
    ui.declarativeView->rootContext()->setContextProperty("rootWindow", this);

    //ui.declarativeView->setSource(QUrl("qrc:/qml/TweetList.qml"));
    ui.declarativeView->setSource(QUrl::fromLocalFile("qml/MainScreen.qml"));
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

MainWindow::~MainWindow()
{

}
