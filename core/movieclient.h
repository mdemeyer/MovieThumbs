
/***************************************************************************
 *   This file is part of MovieThumbs.                                     *
 *   Copyright (C) 2013 De Meyer Maarten <de.meyer.maarten@gmail.com>      *
 *                                                                         *
 *   MovieThumbs is free software; you can redistribute it and/or modify  *
 *   it  under the terms of the GNU Lesser General Public License version  *
 *   2.1 as published by the Free Software Foundation.                     *
 *                                                                         *
 *   MovieThumbs is distributed in the hope that it will be useful, but   *
 *   WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,            *
 *   MA  02110-1301  USA                                                   *
 ***************************************************************************/

#ifndef MOVIECLIENT_H
#define MOVIECLIENT_H

#include "movieservice.h"
#include "tvservice.h"

#include <QtCore/QObject>

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
QT_END_NAMESPACE

class MovieClient : public QObject
{
    Q_OBJECT

public:
    explicit MovieClient();
    virtual ~MovieClient();

    void addSearch(const QString& path);

private:
    QNetworkAccessManager *m_networkManager;
    MovieService *m_movie;
    TvService *m_series;

    bool seriesDownload(const QString& name, const QString& year);
    bool movieDownload(const QString& name, const QString& year);

signals:
    void slotPosterFinished(const QImage& poster);
};

#endif // MOVIECLIENT_H
