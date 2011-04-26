#include <QtGui/QApplication>
#include <QtDeclarative>
#include "mainwindow.h"
#include "shorturlsitem.h"

int main(int argc, char *argv[])
{
    //QApplication::setGraphicsSystem("opengl");

    QApplication app(argc, argv);

    qmlRegisterType<ShortUrlsItem>("qtwitdget.components", 1, 0, "ShortUrlsComponent");

    MainWindow mainWindow;
    mainWindow.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    //mainWindow.setMainQmlFile(QLatin1String("qml/QTwitdget/main.qml"));
    mainWindow.show();
    mainWindow.startUp();
    mainWindow.showExpanded();

    return app.exec();
}
