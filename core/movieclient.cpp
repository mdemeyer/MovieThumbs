
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

#include "movieclient.h"
#include "fileparser.h"

#include <QtCore/QEventLoop>
#include <QtCore/QString>
#include <QtGui/QImage>
#include <QtNetwork/QNetworkAccessManager>

MovieClient::MovieClient()
{
    m_tmdb = new TmdbManager();
}

MovieClient::~MovieClient()
{
    delete m_tmdb;
}

void MovieClient::addSearch(const QString &path)
{
    QString baseName = FileParser::baseName(path);
    QString year = FileParser::year(baseName);
    QString name = FileParser::cleanName(baseName);

    QString filteredName;

    //Look for a local poster or cover file
    QString localFile = FileParser::findLocalFile(path);
    if (!localFile.isNull()) {
        QImage localImage = QImage(localFile);
        if (!localImage.isNull()) {
            emit slotPosterFinished(localImage);
            return;
        }
    }

    if (FileParser::isSeries(baseName)) {
        //Is the poster already in cache?
        if (m_tmdb->duplicate(name, year)) {
            emit slotPosterFinished(m_tmdb->Poster());
            return;
        }
        //Retry cache with cleaner filename
        if (filteredName.isEmpty()) {
            filteredName = FileParser::filterBlacklist(name);
        }
        if (m_tmdb->duplicate(filteredName, year)) {
            emit slotPosterFinished(m_tmdb->Poster());
            return;
        }

        if (seriesDownload(name, year)) {
            emit slotPosterFinished(m_tmdb->Poster());
            return;
        } else if ((filteredName != name) && (seriesDownload(filteredName, year))) {
            emit slotPosterFinished(m_tmdb->Poster());
            return;
        }
    }

    if (movieDownload(name, year)) {
        emit slotPosterFinished(m_tmdb->Poster());
        return;
    } else {
        //Retry search with cleaner filename
        if (filteredName.isEmpty()) {
            filteredName = FileParser::filterBlacklist(name);
        }
        if ((filteredName != name) && (movieDownload(filteredName, year))) {
            emit slotPosterFinished(m_tmdb->Poster());
            return;
        }
    }
}

void MovieClient::setSize(int size)
{
    m_tmdb->setSize(size);
}

bool MovieClient::seriesDownload(const QString &seriesName, const QString &year)
{
    QEventLoop loop;
    connect(m_tmdb, SIGNAL(posterFound()), &loop, SLOT(quit()));
    connect(m_tmdb, SIGNAL(downloadError()), &loop, SLOT(quit()));
    connect(m_tmdb, SIGNAL(posterDownloaded()), &loop, SLOT(quit()));

    m_tmdb->findTv(seriesName, year);
    loop.exec();

    if (m_tmdb->hasPoster) {
        m_tmdb->startDownload();
        loop.exec();
        return true;
    }
    return false;
}

bool MovieClient::movieDownload(const QString &movieName, const QString &movieYear)
{
    QEventLoop loop;
    connect(m_tmdb, SIGNAL(posterDownloaded()), &loop, SLOT(quit()));
    connect(m_tmdb, SIGNAL(posterFound()), &loop, SLOT(quit()));
    connect(m_tmdb, SIGNAL(downloadError()), &loop, SLOT(quit()));

    m_tmdb->findMovie(movieName, movieYear);
    loop.exec();

    if (m_tmdb->hasPoster) {
        m_tmdb->startDownload();
        loop.exec();
        return true;
    }
    return false;
}

#include "movieclient.moc"
