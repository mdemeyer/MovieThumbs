
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

#include "movieThumbs.h"
#include "fileparser.h"
#include "MovieThumbsConfig.h"

#ifdef HAVE_TVDB
#include "tvdbFetcher.h"
#endif

#include <QtCore/QEventLoop>
#include <QtCore/QString>
#include <QtGui/QImage>
#include <QtNetwork/QNetworkAccessManager>

#include <solid/networking.h>

extern "C"
{
    KDE_EXPORT ThumbCreator *new_creator()
    {
        return new MovieThumbs;
    }
}

MovieThumbs::MovieThumbs()
{
    m_networkManager = new QNetworkAccessManager(this);
}

MovieThumbs::~MovieThumbs()
{
    delete m_networkManager;
}

bool MovieThumbs::create(const QString &path, int /*w*/, int /*h*/, QImage &img)
{
    if(Solid::Networking::status() == Solid::Networking::Unconnected)
    {
        qFatal("No network connection available");
        return false;
    }

    QString year = FileParser::year(path);
    QString movieName = FileParser::cleanName(path);

    if(FileParser::isSeries(path)){
#ifdef HAVE_TVDB
        TvdbFetcher series(movieName, m_networkManager);

        QEventLoop loop;
        QObject::connect(&series, SIGNAL(posterDownloaded()), &loop, SLOT(quit()));
        QObject::connect(&series, SIGNAL(downloadError()), &loop, SLOT(quit()));
        loop.exec();

        img = series.getPoster();
#endif
    } else {
        TmdbThumb movie(movieName, year, m_networkManager);

        QEventLoop loop;
        QObject::connect(&movie, SIGNAL(posterDownloaded()), &loop, SLOT(quit()));
        QObject::connect(&movie, SIGNAL(downloadError()), &loop, SLOT(quit()));
        loop.exec();

        img = movie.getPoster();
    }

    return !img.isNull();
}

ThumbCreator::Flags MovieThumbs::flags() const
{
    return (Flags)(None);
}

#include "movieThumbs.moc"
