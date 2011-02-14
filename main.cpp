#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    //mainWindow.setMainQmlFile(QLatin1String("qml/QTwitdget/main.qml"));
    mainWindow.show();
    mainWindow.startUp();
    //mainWindow.showExpanded();

    return app.exec();
}
