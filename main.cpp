#include <QtGui/QApplication>
#include <QtDeclarative>
#include "mainwindow.h"
#include "shorturlsitem.h"
#include "tweetentitylistmodel.h"

int main(int argc, char *argv[])
{
    //QApplication::setGraphicsSystem("opengl");
    QApplication app(argc, argv);
    QCoreApplication::setAttribute(Qt::AA_S60DisablePartialScreenInputMode, false);

    qmlRegisterType<ShortUrlsItem>("qtwitdget.components", 1, 0, "ShortUrlsComponent");
    qmlRegisterType<TweetEntityListModel>("qtwitdget.components", 1, 0, "TweetEntityListModel");

    MainWindow mainWindow;
    mainWindow.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    mainWindow.setAttribute(Qt::WA_NoSystemBackground); //Symbian^3
    //mainWindow.setMainQmlFile(QLatin1String("qml/QTwitdget/main.qml"));
    mainWindow.show();
    mainWindow.startUp();
    mainWindow.showExpanded();

    return app.exec();
}
