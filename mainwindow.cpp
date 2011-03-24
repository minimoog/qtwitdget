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

#include "mainwindow.h"
#include <QtDebug>
#include <QNetworkAccessManager>
#include <QSettings>
#include <QGraphicsObject>
#include <QMenu>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDir>
#include <QSqlQuery>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QApplication>
#include "qtweetlib/oauthtwitter.h"
#include "qtweetlib/qtweetuserstream.h"
#include "qtweetlib/qtweetaccountverifycredentials.h"
#include "qtweetlib/qtweetdirectmessagenew.h"
#include "qtweetlib/qtweetstatusupdate.h"
#include "qtweetlib/qtweetuser.h"
#include "qtweetlib/qtweetdmstatus.h"
#include "qtweetlib/qtweetstatus.h"
#include "tweetqmllistmodel.h"
#include "mentionsqmllistmodel.h"
#include "directmessagesqmllistmodel.h"
#include "searchqmllistmodel.h"
#include "userinfo.h"
#include "usertimelinelistmodel.h"
#include "conversationlistmodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QmlApplicationViewer(parent),
    m_netManager(new QNetworkAccessManager(this)),
    m_oauthTwitter(new OAuthTwitter(this)),
    m_userStream(new QTweetUserStream(this))
{
    m_oauthTwitter->setNetworkAccessManager(m_netManager);
    m_userStream->setOAuthTwitter(m_oauthTwitter);

    qApp->setOrganizationName("QTwitdget");

    connect(m_oauthTwitter, SIGNAL(authorizeXAuthFinished()), this, SLOT(authorizationFinished()));
    connect(m_oauthTwitter, SIGNAL(authorizeXAuthError()), this, SLOT(authorizationFailed()));

    m_database = QSqlDatabase::addDatabase("QSQLITE");

    setupTrayIcon();

    readSettings();

    createDeclarativeView();
}

void MainWindow::authorize(const QString &username, const QString &password)
{
    m_oauthTwitter->clearTokens();
    m_oauthTwitter->authorizeXAuth(username, password);
}

void MainWindow::authorizationFinished()
{
    qDebug() << "Authorization succesfull";

    QTweetAccountVerifyCredentials *tweetVerifyCredentials = new QTweetAccountVerifyCredentials;
    tweetVerifyCredentials->setOAuthTwitter(m_oauthTwitter);
    tweetVerifyCredentials->verify();
    connect(tweetVerifyCredentials, SIGNAL(parsedUser(QTweetUser)),
            this, SLOT(verifyCredentialsFinished(QTweetUser)));
}

void MainWindow::authorizationFailed()
{
    qDebug() << "Authorization failed";

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QTwitdget", "QTwitdget");
    settings.remove("oauth_token");
    settings.remove("oauth_token_secret");
    settings.remove("user_id");
    settings.remove("user_screenname");
}

void MainWindow::verifyCredentialsFinished(const QTweetUser& userinfo)
{
    qDebug() << "Verify Credential succesfull";

    QTweetAccountVerifyCredentials *tweetVerifyCredentials = qobject_cast<QTweetAccountVerifyCredentials*>(sender());
    if (tweetVerifyCredentials) {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QTwitdget", "QTwitdget");
        settings.setValue("oauth_token", m_oauthTwitter->oauthToken());
        settings.setValue("oauth_token_secret", m_oauthTwitter->oauthTokenSecret());
        settings.setValue("user_id", userinfo.id());
        settings.setValue("user_screenname", userinfo.screenName());

        m_userId = userinfo.id();
        m_userScreenName = userinfo.screenName();

        emit userScreenNameChanged();

        tweetVerifyCredentials->deleteLater();

        startUp();
    }
}

void MainWindow::changeUserPass()
{
    QGraphicsObject *obj = rootObject();
    obj->setProperty("authed", false);

    m_userStream->streamDisconnect();
    m_tweetListModel->clear();
    m_mentionsListModel->clear();

    // ### TODO Clear tokens in .ini?
}

void MainWindow::startUp()
{
    //read settings
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "QTwitdget", "QTwitdget");
    m_userId = settings.value("user_id", 0).toInt();
    m_userScreenName = settings.value("user_screenname").toString();
    QString oauthToken = settings.value("oauth_token").toString();
    QString oauthTokenSecret = settings.value("oauth_token_secret").toString();

    if (!m_userScreenName.isEmpty())
        emit userScreenNameChanged();

    if(m_userId != 0 && !oauthToken.isEmpty() && !oauthTokenSecret.isEmpty()){
        m_oauthTwitter->setOAuthToken(oauthToken.toUtf8());
        m_oauthTwitter->setOAuthTokenSecret(oauthTokenSecret.toUtf8());

        //create or change database according to user id
        createDatabase(QString::number(m_userId));

        //show/animate tweets list page
        QGraphicsObject *obj = rootObject();
        obj->setProperty("authed", true);

        //set user id in the models
        m_tweetListModel->setUserID(m_userId);
        m_mentionsListModel->setUserID(m_userId);
        m_directMessagesListModel->setUserID(m_userId);
        m_conversationListModel->setUserID(m_userId);

        //show last tweets from database
        m_tweetListModel->loadTweetsFromDatabase();
        m_mentionsListModel->loadTweetsFromDatabase();
        m_directMessagesListModel->loadTweetsFromDatabase();

        //then fetch last 200 tweets
        m_tweetListModel->fetchLastTweets();
        m_mentionsListModel->fetchLastTweets();
        m_directMessagesListModel->fetchLastTweets();

        m_userStream->startFetching();

    } else {
        changeUserPass();
    }
}

void MainWindow::updateButtonClicked(const QString &id, const QString &text, const QString& screenName)
{
    bool ok;

    //if screenName is not empty, then it's direct message
    if (!screenName.isEmpty()) {
        qDebug() << "Sending DM to " << screenName;

        QTweetDirectMessageNew *dm = new QTweetDirectMessageNew;
        dm->setOAuthTwitter(m_oauthTwitter);
        dm->post(screenName, text);

        connect(dm, SIGNAL(parsedDirectMessage(QTweetDMStatus)),
                this, SLOT(directMessageNewFinished(QTweetDMStatus)));

        return;
    }

    qint64 tweetid = id.toLongLong(&ok);

    QString updateText = text.left(140);

    if (updateText.isEmpty()) {
        qDebug() << "MainWindow::updateButtonClicked: empty update text";
        return;
    }

    QTweetStatusUpdate *statusUpdate = new QTweetStatusUpdate;
    statusUpdate->setOAuthTwitter(m_oauthTwitter);
    statusUpdate->post(text, tweetid);
    connect(statusUpdate, SIGNAL(postedStatus(QTweetStatus)),
            this, SLOT(statusUpdateFinished(QTweetStatus)));
}

void MainWindow::directMessageNewFinished(const QTweetDMStatus& directMessage)
{
    QTweetDirectMessageNew *dm = qobject_cast<QTweetDirectMessageNew*>(sender());
    if (dm) {
        qDebug() << "Direct Message sent, id: " << directMessage.id();
        dm->deleteLater();
    }
}

void MainWindow::statusUpdateFinished(const QTweetStatus &status)
{
    QTweetStatusUpdate *statusUpdate = qobject_cast<QTweetStatusUpdate*>(sender());
    if (statusUpdate) {
        qDebug() << "Sended status with id: " << status.id();

        statusUpdate->deleteLater();
    }
}

void MainWindow::setupTrayIcon()
{
    QAction *actionMinimize = new QAction(this);
    actionMinimize->setText(tr("Minimize"));
    connect(actionMinimize, SIGNAL(triggered()), this, SLOT(showMinimized()));

    QAction *actionRestore = new QAction(this);
    actionRestore->setText(tr("Restore"));
    connect(actionRestore, SIGNAL(triggered()), this, SLOT(showNormal()));

    QAction *actionQuit = new QAction(this);
    actionQuit->setText("Quit");

    QAction *actionLogout = new QAction(this);
    actionLogout->setText("Logout");
    connect(actionLogout, SIGNAL(triggered()), this, SLOT(changeUserPass()));

    m_trayIconMenu = new QMenu(this);
    m_trayIconMenu->addAction(actionMinimize);
    m_trayIconMenu->addAction(actionRestore);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(actionLogout);
    m_trayIconMenu->addAction(actionQuit);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(QIcon(":/qtwidget_icon.ico"));
    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_trayIcon->show();

    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            if (isHidden() || isMinimized()) {
                showNormal();
                activateWindow();
            }
            else
                showMinimized();
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

//void MainWindow::changeEvent(QEvent *e)
//{
//    if(e->type() == QEvent::LanguageChange)
//           // ui.retranslateUi(this);

//    QMainWindow::changeEvent(e);
//}

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
               "senderProfileImageUrl TEXT, "
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
}

void MainWindow::createDeclarativeView()
{
    m_tweetListModel = new TweetQmlListModel(m_oauthTwitter);
    //connect user stream to tweet home timeline
    connect(m_userStream, SIGNAL(statusesStream(QTweetStatus)),
            m_tweetListModel, SLOT(onStatusesStream(QTweetStatus)));
    connect(m_userStream, SIGNAL(deleteStatusStream(qint64,qint64)),
            m_tweetListModel, SLOT(onDeleteStatusStream(qint64,qint64)));
    // ### TODO: Test for syncing problems
    connect(m_userStream, SIGNAL(reconnected()),
            m_tweetListModel, SLOT(fetchLastTweets()));
    //try using REST API when streams fails
    connect(m_userStream, SIGNAL(failureConnect()),
            m_tweetListModel, SLOT(fetchLastTweets()));

    m_mentionsListModel = new MentionsQmlListModel(m_oauthTwitter);
    connect(m_userStream, SIGNAL(statusesStream(QTweetStatus)),
            m_mentionsListModel, SLOT(onStatusesStream(QTweetStatus)));
    connect(m_userStream, SIGNAL(deleteStatusStream(qint64,qint64)),
            m_mentionsListModel, SLOT(onDeleteStatusStream(qint64,qint64)));
    connect(m_userStream, SIGNAL(reconnected()),
            m_mentionsListModel, SLOT(fetchLastTweets()));

    m_directMessagesListModel = new DirectMessagesQmlListModel(m_oauthTwitter);
    connect(m_userStream, SIGNAL(directMessageStream(QTweetDMStatus)),
            m_directMessagesListModel, SLOT(onDirectMessageStream(QTweetDMStatus)));

    m_searchListModel = new SearchQmlListModel(m_oauthTwitter);

    m_userInfo = new UserInfo;
    m_userInfo->setOAuthTwitter(m_oauthTwitter);
    connect(m_userStream, SIGNAL(friendsList(QList<qint64>)),
            m_userInfo, SLOT(onUserStreamFriendsList(QList<qint64>)));

    m_userTimelineListModel = new UserTimelineListModel(m_oauthTwitter);

    m_conversationListModel = new ConversationListModel(m_oauthTwitter);

    rootContext()->setContextProperty("hometimelineListModel", m_tweetListModel);
    rootContext()->setContextProperty("mentionsListModel", m_mentionsListModel);
    rootContext()->setContextProperty("directMessagesListModel", m_directMessagesListModel);
    rootContext()->setContextProperty("searchListModel", m_searchListModel);
    rootContext()->setContextProperty("userInfo", m_userInfo);
    rootContext()->setContextProperty("userTimelineListModel", m_userTimelineListModel);
    rootContext()->setContextProperty("conversationListModel", m_conversationListModel);
    rootContext()->setContextProperty("rootWindow", this);
	
#ifdef Q_OS_MAC
	rootContext()->setContextProperty("osMac", true);
#else
	rootContext()->setContextProperty("osMac", false);
#endif

    //setSource(QUrl::fromLocalFile("qml/QTwitdget/Main.qml"));
    setMainQmlFile(QLatin1String("qml/QTwitdget/Main.qml"));
}

QString MainWindow::userScreenName() const
{
    return m_userScreenName;
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
