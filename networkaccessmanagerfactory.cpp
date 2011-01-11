#include "networkaccessmanagerfactory.h"
#include <QNetworkDiskCache>
#include <QDesktopServices>
#include <QDir>

NetworkAccessManagerFactory::NetworkAccessManagerFactory() :
    QDeclarativeNetworkAccessManagerFactory()
{
}

QNetworkAccessManager* NetworkAccessManagerFactory::create(QObject *parent)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(parent);

    QNetworkDiskCache* diskCache = new QNetworkDiskCache(parent);

    QString dataPath = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);

    QDir().mkpath(dataPath);

    diskCache->setCacheDirectory(dataPath);
    diskCache->setMaximumCacheSize(2 * 1024 * 1024);
    manager->setCache(diskCache);

    return manager;
}
