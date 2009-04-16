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

#include <QtDebug>
#include <QDesktopServices>
#include <QCoreApplication>
#include "imagedownloader.h"

ImageDownloader::ImageDownloader(QObject *parent)
	:	QObject(parent), m_manager(0)
{
	resolveHomeAppPath();
}

void ImageDownloader::setNetworkAccessManager(QNetworkAccessManager* manager)
{
	m_manager = manager;
}

QNetworkAccessManager* ImageDownloader::networkAccessManager() const
{
	return m_manager;
}

void ImageDownloader::downloadImages(const QList<QUrl> &urls)
{
	downloadCount = 0;

	foreach(const QUrl& url, urls){

		if(!m_images.contains(url.toString())){ //avoid download/load duplicate urls

			m_images.insert(url.toString(), QImage());	//insert empty image for avoiding duplicates

			QString imageFileName = homeAppImagesPath + decodeUrlToFilename(url);

			if(!QFile::exists(imageFileName)){	//download
				Q_ASSERT(m_manager != 0);
				downloadCount++;
				QNetworkRequest request(url);
				QNetworkReply *reply = m_manager->get(request);
				connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
				connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
			} else {							//load from disk
				QImage img;

				if(!img.load(imageFileName))
					qDebug() << "nema slika";

				m_images.insert(url.toString(), img);
			}
		}
	}

	if(downloadCount == 0)
		emit finished();
}

void ImageDownloader::downloadFinished()
{
	downloadCount--;
	
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
	
	if(reply){
		if(reply->error()){	//?????
			qDebug() << "error downloading";
		}
			
		QImage img;

		QByteArray imgArray = reply->readAll();
		if(!img.loadFromData(imgArray))
			qDebug() << "ups";

		reply->deleteLater();

		m_images.insert(reply->url().toString(), img);

		//example
		//http://s3.amazonaws.com/twitter_production/profile_images/60168448/whats-buzz-head_bigger_normal.png
		//to 60168448_whats-buzz-head_bigger_normal.png
		//I hope in the future twitter will not change urls paths
		
		QString filename = decodeUrlToFilename(reply->url());
		if(filename.isEmpty())
			return;				

		QString fullPathFilename = homeAppImagesPath + filename;

		QFile file(fullPathFilename);
		if(!file.open(QIODevice::WriteOnly)) {
			qDebug() << "Could not write: ";
		}

		file.write(imgArray);
		file.close();
	}

	if(downloadCount == 0)
		emit finished();
}

void ImageDownloader::error()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
	if(reply){
		QUrl url = reply->url();
		qDebug() << "error downloading";
		qDebug() << url;
		qDebug() << reply->errorString();
	}

	downloadCount--;
}

QString ImageDownloader::decodeUrlToFilename(const QUrl &url)
{
	QString wholePath = url.path();
	QFileInfo fileInfo(wholePath);
	QString basename = fileInfo.fileName();

	if(basename.isEmpty())
		return QString();

	QString path = fileInfo.path();

	if(path.isEmpty())
		return QString();

	//extract number from path
	int pos = path.lastIndexOf('/');
	path.remove(0, pos + 1);

	return path + '_' + basename;
}

void ImageDownloader::resolveHomeAppPath()
{
	QCoreApplication::setOrganizationName("QTwitdget");
	homeAppImagesPath = QDesktopServices::storageLocation(QDesktopServices::DataLocation);

	//make directory if doesn't exist
	QDir dir;
	dir.setPath(homeAppImagesPath);
	if(!dir.exists())
		dir.mkpath(".");

	homeAppImagesPath += '/';
}
	
QHash<QString, QImage> ImageDownloader::getImages() const
{
	return m_images;
}