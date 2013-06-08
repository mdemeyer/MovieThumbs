
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

#include "moviethumbs.h"
#include "fileparser.h"

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
    m_movie = new MovieService(m_networkManager);
#ifdef HAVE_TVDB
    m_series = new TvService(m_networkManager);
#endif
}

MovieThumbs::~MovieThumbs()
{
    delete m_networkManager;
    delete m_movie;
#ifdef HAVE_TVDB
    delete m_series;
#endif
}

bool MovieThumbs::create(const QString &path, int /*w*/, int /*h*/, QImage &img)
{
    if(Solid::Networking::status() == Solid::Networking::Unconnected)
    {
        qFatal("No network connection available");
        return false;
    }

    QString year = FileParser::year(path);
    QString name = FileParser::cleanName(path);

    QEventLoop loop;
    connect(m_movie, SIGNAL(posterDownloaded()), &loop, SLOT(quit()));
    connect(m_movie, SIGNAL(posterFound()), &loop, SLOT(quit()));
    connect(m_movie, SIGNAL(downloadError()), &loop, SLOT(quit()));

    if(FileParser::isSeries(path)){
#ifdef HAVE_TVDB
        m_series->startSearch(name, year);

        connect(m_series, SIGNAL(posterFound()), &loop, SLOT(quit()));
        connect(m_series, SIGNAL(downloadError()), &loop, SLOT(quit()));
        connect(m_series, SIGNAL(posterDownloaded()), &loop, SLOT(quit()));

        loop.exec();

        if(!m_series->hasPoster()) {
            //Try again with a cleaner filename
            //TODO remove duplicate code
            QString clean = FileParser::filterBlacklist(name);
            m_series->startSearch(clean, year);
            loop.exec();
        }
        if(m_series->hasPoster()) {
            m_series->startDownload();
            loop.exec();
            img = m_series->Poster();
        }
#endif
    }
    if(img.isNull()) {
        m_movie->startSearch(name, year);
        loop.exec();

        if(!m_movie->hasPoster()) {
            //Try again with a cleaner filename
            QString clean = FileParser::filterBlacklist(name);
            m_movie->startSearch(clean, year);
            loop.exec();
        }
        if(m_movie->hasPoster()) {
            m_movie->startDownload();
            loop.exec();
            img = m_movie->Poster();
        }
    }

    return !img.isNull();
}

ThumbCreator::Flags MovieThumbs::flags() const
{
    return (Flags)(None);
}

#include "moviethumbs.moc"
