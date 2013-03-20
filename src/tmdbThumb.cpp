
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

#include <QtCore/QByteArray>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

#include <QtCore/QVariant>
#include <QtCore/QVariantList>
#include <QtCore/QVariantMap>

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QSslError>
#include <qjson/parser.h>
#include <stdio.h>

#include "tmdbThumb.h"

const QString TmdbThumb::KEY = "5c8533aacb1fa275a5113d0728268d5a";
QImage moviePoster;

TmdbThumb::TmdbThumb(QString &movieName)
{
    QUrl urlQuery("https://api.themoviedb.org/3/search/movie");
    urlQuery.addQueryItem("api_key",KEY);

    /*If there is a year included in the title use it to refine the search
     * \(19|20) number starting with 19 OR 20
     * \d{2} followed by 2 numbers [0-9]
     * This works for all movies released from 1900 to 2099.
     */
    QRegExp regex("(19|20)\\d{2}");
    if(regex.lastIndexIn(movieName) != -1){
        if(!regex.isEmpty()){
            urlQuery.addQueryItem("year",regex.cap(0));
        }
    }

    /*Ignore all information between brackets.
     * Works with both () and []
     * TODO Clean up and improve the regular expression. This makes my head hurt.
     */
    regex.setPattern("\\([^\\(]*\\)|\\[([^]]+)\\]");
    movieName.remove(regex);
    urlQuery.addQueryItem("query",movieName);

    m_networkManager = new QNetworkAccessManager(this);

    QNetworkRequest request;
    request.setUrl(urlQuery);
    request.setRawHeader("Accept", "application/json");

    QNetworkReply *reply = m_networkManager->get(request);    
    connect(reply, SIGNAL(finished()), this, SLOT(queryFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetworkError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));
}

TmdbThumb::~TmdbThumb()
{
    delete m_networkManager;
}

QImage TmdbThumb::getPoster()
{
    return moviePoster;
}

void TmdbThumb::queryFinished()
{
    QNetworkReply *queryReply = qobject_cast<QNetworkReply *>(sender());
    QByteArray data = queryReply->readAll();
    queryReply->deleteLater();
    
    QJson::Parser parser;
    bool ok;

    QVariantMap result = parser.parse (data, &ok).toMap();
    if (!ok) {
        qFatal("An error occurred during parsing");
        emit downloadError();
        exit (1);
    }

    QVariantList movies = result["results"].toList();
    QStringList m_posterPath;

    foreach (const QVariant &variant, movies) {
        QVariantMap poster = variant.toMap();
        m_posterPath << (poster["poster_path"]).toString();
    }

    QUrl downloadUrl("https://cf2.imgobject.com/t/p/w185/" + m_posterPath.at(0));

    QNetworkRequest request;
    request.setUrl(downloadUrl);
    request.setRawHeader("Accept", "application/json");

    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
}

bool TmdbThumb::downloadFinished()
{
    QNetworkReply *downloadReply = qobject_cast<QNetworkReply *>(sender());
    QByteArray data = downloadReply->readAll();
    downloadReply->deleteLater();
    
    moviePoster.loadFromData(data);
    
    emit posterDownloaded();
    return true;
}

void TmdbThumb::onNetworkError(QNetworkReply::NetworkError)
{
    qFatal("error");
    emit downloadError();
}

void TmdbThumb::slotSslErrors(const QList<QSslError> &sslErrors)
{
    foreach (const QSslError &error, sslErrors)
        fprintf(stderr, "SSL error: %s\n", qPrintable(error.errorString()));

    emit downloadError();
}

#include "tmdbThumb.moc"