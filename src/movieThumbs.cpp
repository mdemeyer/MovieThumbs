
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

#include <QtCore/QEventLoop>
#include <QtCore/QFileInfo>
#include <QtCore/QString>
#include <QtGui/QImage>

#include "movieThumbs.h"

extern "C"
{
    KDE_EXPORT ThumbCreator *new_creator()
    {
        return new MovieThumbs;
    }
}

MovieThumbs::MovieThumbs()
{
}

MovieThumbs::~MovieThumbs()
{
}

bool MovieThumbs::create(const QString &path, int /*w*/, int /*h*/, QImage &img)
{
    QFileInfo file(path);
    QString movieName = file.baseName();

    tmdbThumb movie(movieName);

    QEventLoop loop;
    QObject::connect(&movie, SIGNAL(posterDownloaded()), &loop, SLOT(quit()));
    QObject::connect(&movie, SIGNAL(downloadError), &loop, SLOT(quit()));
    loop.exec();

    img = movie.getPoster();

    if(!img.isNull()){
        return true;
    } else
        return false;
}

ThumbCreator::Flags MovieThumbs::flags() const
{
    QImage test;
    return (Flags)(None);
}

#include "movieThumbs.moc"