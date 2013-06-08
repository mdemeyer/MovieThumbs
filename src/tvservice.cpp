
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

#include <QtNetwork/QNetworkRequest>

#include <KDebug>

#include <tvdb/client.h>
#include <tvdb/series.h>

const QString TvService::KEY = "DA777D9ACDBB771E";

TvService::~TvService()
{
    delete m_client;
}

void TvService::startSearch(const QString &name, const QString & /*year*/)
{
    m_client = new Tvdb::Client(this);
    m_client->setApiKey(KEY);

    connect(m_client, SIGNAL(finished(Tvdb::Series)),SLOT(foundSeries(Tvdb::Series)));
    connect(m_client, SIGNAL(multipleResultsFound(QList<Tvdb::Series>)),SLOT(foundMultipleSeries(QList<Tvdb::Series>)));
    m_client->getSeriesByName(name);
}

void TvService::foundSeries(const Tvdb::Series &series)
{
    if(!series.isValid()){
        kDebug() << "No valid series found";
        emit downloadError();
        return;
    }

    QList<QUrl> posterList = series.posterUrls();

    if(posterList.isEmpty()){
        //No posters to download
        emit downloadError();
        return;
    }

    setUrl(posterList[0]);
    emit posterFound();
}

void TvService::foundMultipleSeries(const QList<Tvdb::Series> &series)
{
    m_client->getSeriesById(series[0].id());
}

#include "tvservice.moc"
