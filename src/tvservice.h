
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

#ifndef TVSERVICE_H
#define TVSERVICE_H

#include <posterservice.h>

#include <tvdb/client.h>
#include <tvdb/series.h>

class QNetworkAccessManager;

class TvService : public PosterService
{
    Q_OBJECT

public:
    TvService(QNetworkAccessManager *qnam) : PosterService(qnam) {}
    ~TvService();
    void startSearch(const QString& name, const QString& year);

private:
    static const QString KEY;
    Tvdb::Client* m_client;

private slots:
    void foundSeries(const Tvdb::Series& series);
    void foundMultipleSeries(const QList<Tvdb::Series>& series);
};

#endif // TVSERVICE_H
