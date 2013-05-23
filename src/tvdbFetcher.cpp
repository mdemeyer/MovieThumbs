
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

#include "tvdbFetcher.h"

#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

#include <QtNetwork/QNetworkRequest>

#include <tvdb/client.h>
#include <tvdb/series.h>

QImage poster;

TvdbFetcher::TvdbFetcher(const QString &name)
{
    m_client = new Tvdb::Client(this);
    m_client->setApiKey("DA777D9ACDBB771E");

    connect(m_client, SIGNAL(finished(Tvdb::Series)),SLOT(foundSeries(Tvdb::Series)));
    connect(m_client, SIGNAL(multipleResultsFound(QList<Tvdb::Series>)),SLOT(foundMultipleSeries(QList<Tvdb::Series>)));
    m_client->getSeriesByName(name);
}

TvdbFetcher::~TvdbFetcher()
{
    delete m_client;
    delete m_networkManager;
}

QImage TvdbFetcher::getPoster()
{
    return poster;
}

void TvdbFetcher::foundSeries(const Tvdb::Series &series)
{
    QList<QUrl> posterList = series.posterUrls();

    m_networkManager = new QNetworkAccessManager(this);

    QNetworkRequest request;
    request.setUrl(posterList[0]);

    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
}

void TvdbFetcher::foundMultipleSeries(const QList<Tvdb::Series> &series)
{
    m_client->getSeriesById(series[0].id());
}

bool TvdbFetcher::downloadFinished()
{
    QNetworkReply *downloadReply = qobject_cast<QNetworkReply *>(sender());
    QByteArray data = downloadReply->readAll();
    downloadReply->deleteLater();

    poster.loadFromData(data);

    emit posterDownloaded();
    return true;
}

#include "tvdbFetcher.moc"
