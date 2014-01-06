
/***************************************************************************
 *   This file is part of MovieThumbs.                                     *
 *   Copyright (C) 2013 De Meyer Maarten <de.meyer.maarten@gmail.com>      *
 *                                                                         *
 *   MovieThumbs is free software; you can redistribute it and/or modify   *
 *   it  under the terms of the GNU Lesser General Public License version  *
 *   2.1 as published by the Free Software Foundation.                     *
 *                                                                         *
 *   MovieThumbs is distributed in the hope that it will be useful, but    *
 *   WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,            *
 *   MA  02110-1301  USA                                                   *
 ***************************************************************************/

#include "downloadmanager.h"

#include <QtCore/QByteArray>
#include <QtCore/QUrl>

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QSslError>

#include <QDebug>

DownloadManager::DownloadManager(QNetworkAccessManager *qnam)
{
    networkManager = qnam;
    hasPoster = false;
}

DownloadManager::~DownloadManager()
{
}

bool DownloadManager::duplicate(const QString &name, const QString & /*year*/)
{
    if(cache.contains(name)) {
        copyImage(cache.object(name));
        return true;
    }
    return false;
}

QImage DownloadManager::Poster()
{
    return poster;
}

void DownloadManager::setUrl(QUrl url)
{
    posterLink = url;
    hasPoster = true;
}

void DownloadManager::startDownload()
{
    QNetworkRequest request;
    request.setUrl(posterLink);

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetworkError(QNetworkReply::NetworkError)));

    hasPoster = false;
}

void DownloadManager::downloadFinished()
{
    QNetworkReply *downloadReply = qobject_cast<QNetworkReply *>(sender());
    QByteArray data = downloadReply->readAll();
    downloadReply->deleteLater();

    poster.loadFromData(data);

    emit posterDownloaded();
}

void DownloadManager::copyImage(QImage *image)
{
    poster = *image;
}

QString DownloadManager::language()
{
    // QLocale::name returns the locale in lang_COUNTRY format
    // we only need the 2 letter lang code
    QString lang = QLocale::system().name();
    return lang.left(2);
}

void DownloadManager::storeImage()
{
    cache.insert(nameKey, new QImage(Poster()));

    // We only want to cache series.
    QObject::disconnect(this, SIGNAL(posterDownloaded()), this, SLOT(storeImage()));
}

void DownloadManager::onNetworkError(QNetworkReply::NetworkError)
{
    qDebug() << "Download error";
    emit downloadError();
}

void DownloadManager::slotSslErrors(const QList<QSslError> &sslErrors)
{
    foreach(const QSslError & error, sslErrors) {
        qDebug() << "SSL error: " << qPrintable(error.errorString());
    }

    emit downloadError();
}

#include "downloadmanager.moc"
