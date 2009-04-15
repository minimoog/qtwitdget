/* Copyright (c) 2008, Antonie Jovanoski
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
 * Contact e-mail: Antonie Jovanoski <minimoog77@gmail.com>
 */

#ifndef IMAGEDOWNLOADER_H
#define IMAGEDOWNLOADER_H

#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QImage>
#include <QByteArray>
#include <QFileInfo>
#include <QDir>
#include <QHash>

/*! Class ImageDownloader */
class ImageDownloader : public QObject
{
	Q_OBJECT

public:
	/*! Constructor */
	ImageDownloader(QObject *parent = 0);
	/*! Download images for given urls */
	void downloadImages(const QList<QUrl> &urls);
	/*! Gets downloaded images in hash table */
	QHash<QString, QImage> getImages() const;
	/*! Sets Network Access Manager */
	void setNetworkAccessManager(QNetworkAccessManager* manager);
	/*! Gets Network Access Manager */
	QNetworkAccessManager* networkAccessManager() const;

signals:
	/*! Emitted when download/load of all images is finished */
	void finished();

	//TODO: Nice to have separate signal for every image

private slots:
	void downloadFinished();
	void error();

private:
	QNetworkAccessManager *m_manager;
	QString homeAppImagesPath;
	QHash<QString, QImage> m_images;
	int downloadCount;

	QString decodeUrlToFilename(const QUrl &url);
	void resolveHomeAppPath();
};

#endif // IMAGEDOWNLOADER_H