#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    mainWindow.show();
    mainWindow.startUp();
    //mainWindow.showExpanded();

    return app.exec();
}
