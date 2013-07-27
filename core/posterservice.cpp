
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

#include "posterservice.h"

#include <QtCore/QByteArray>
#include <QtCore/QUrl>

#include <QtNetwork/QNetworkRequest>

#include <QDebug>

PosterService::PosterService(QNetworkAccessManager *qnam)
{
    networkManager = qnam;
    hasPoster = false;
}

PosterService::~PosterService()
{
}

QImage PosterService::Poster()
{
    return poster;
}

void PosterService::setUrl(QUrl url)
{
    posterLink = url;
    hasPoster = true;
}

void PosterService::startDownload()
{
    QNetworkRequest request;
    request.setUrl(posterLink);

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetworkError(QNetworkReply::NetworkError)));

    hasPoster = false;
}

void PosterService::downloadFinished()
{
    QNetworkReply *downloadReply = qobject_cast<QNetworkReply *>(sender());
    QByteArray data = downloadReply->readAll();
    downloadReply->deleteLater();

    poster.loadFromData(data);

    emit posterDownloaded();
}

void PosterService::copyImage(QImage *image)
{
    poster = *image;
}

void PosterService::onNetworkError(QNetworkReply::NetworkError)
{
    qDebug() << "Download error";
    emit downloadError();
}

#include "posterservice.moc"
