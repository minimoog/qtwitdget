/* Copyright (c) 2011, Antonie Jovanoski
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

#ifndef NAMFACTORY_H
#define NAMFACTORY_H

#include <QDeclarativeNetworkAccessManagerFactory>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QDesktopServices>

/**
 *  Implements qnam factory where qnam uses a disk cache
 */
class NAMFactory : public QDeclarativeNetworkAccessManagerFactory
{
public:
    QNetworkAccessManager* create(QObject *parent) {
        QNetworkAccessManager *nam = new QNetworkAccessManager(parent);

        QNetworkDiskCache *diskCache = new QNetworkDiskCache(nam);
        diskCache->setCacheDirectory(QDesktopServices::storageLocation(QDesktopServices::CacheLocation));
        diskCache->setMaximumCacheSize(1024 * 1024);

        nam->setCache(diskCache);

        return nam;
    }
};

#endif // NAMFACTORY_H
