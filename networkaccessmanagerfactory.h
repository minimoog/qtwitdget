#ifndef NETWORKACCESSMANAGERFACTORY_H
#define NETWORKACCESSMANAGERFACTORY_H

#include <QDeclarativeNetworkAccessManagerFactory>
#include <QNetworkAccessManager>

class NetworkAccessManagerFactory : public QDeclarativeNetworkAccessManagerFactory
{
public:
    explicit NetworkAccessManagerFactory();
    virtual QNetworkAccessManager* create(QObject *parent);
};

#endif // NETWORKACCESSMANAGERFACTORY_H
