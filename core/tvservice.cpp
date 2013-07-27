
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

#include "tvservice.h"

#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QXmlStreamReader>

#include <QtNetwork/QNetworkRequest>

#include <QDebug>

const QString TvService::KEY = "DA777D9ACDBB771E";

TvService::TvService(QNetworkAccessManager *qnam) : PosterService(qnam) {
    connect(this,SIGNAL(posterDownloaded()),this,SLOT(storeImage()));
}

TvService::~TvService()
{

}

bool TvService::duplicate(const QString &name, const QString & /*year*/)
{
    if(cache.contains(name)) {
        copyImage(cache.object(name));
        return true;
    }
    return false;
}

void TvService::startSearch(const QString &name, const QString & /*year*/)
{
    nameKey = name;

    QUrl urlQuery("http://thetvdb.com/api/GetSeries.php");
    urlQuery.addQueryItem("seriesname", name);

    QNetworkRequest request;
    request.setUrl(urlQuery);

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(foundSeries()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetworkError(QNetworkReply::NetworkError)));
}

void TvService::foundSeries()
{
    QNetworkReply *queryReply = qobject_cast<QNetworkReply *>(sender());
    QByteArray data = queryReply->readAll();
    queryReply->deleteLater();

    QXmlStreamReader xmlStream(data);

    //Read the seriesid
    while(!xmlStream.atEnd()) {
        xmlStream.readNext();

        if(xmlStream.name().toString() == "seriesid") {
            // http://thetvdb.com/api/<apikey>/series/<seriesid>/banners.xml
            QUrl bannerQuery("http://thetvdb.com/api/" + KEY + "/series/" + xmlStream.readElementText() + "/banners.xml");

            QNetworkRequest request;
            request.setUrl(bannerQuery);

            QNetworkReply *reply = networkManager->get(request);
            connect(reply, SIGNAL(finished()), this, SLOT(foundBanners()));
            connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetworkError(QNetworkReply::NetworkError)));
            return;
        }
    }

    //The cake is a lie!
    qDebug() <<"No valid series found";
    emit downloadError();
    return;
}

void TvService::foundBanners()
{
    QNetworkReply *queryReply = qobject_cast<QNetworkReply *>(sender());
    QByteArray data = queryReply->readAll();
    queryReply->deleteLater();

    QXmlStreamReader xmlStream(data);

    QString url = QString();

    //Read the bannerLink
    while(!xmlStream.atEnd()) {
        xmlStream.readNext();

        if(xmlStream.name().toString() == "BannerPath") {
            url = xmlStream.readElementText();
        }
        if(xmlStream.name().toString() == "BannerType") {
            //If the banner type is a poster, break
            if(xmlStream.readElementText() == "poster") {
                setUrl("http://thetvdb.com/banners/" + url);
                emit posterFound();
                return;
            }
        }
    }
    //If we are here all hope is lost.
    emit downloadError();
}

void TvService::storeImage()
{
    cache.insert(nameKey, new QImage(Poster()));
}

#include "tvservice.moc"
