
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
#include <libmoviethumbs/movieclient.h>

#include <QtCore/QEventLoop>
#include <QtCore/QString>
#include <QtGui/QImage>
#include <QtNetwork/QNetworkAccessManager>

#include <KDebug>
#include <solid/networking.h>

QImage thumbnail;

extern "C"
{
    KDE_EXPORT ThumbCreator *new_creator()
    {
        return new MovieThumbs;
    }
}

MovieThumbs::MovieThumbs()
{
    size = 0;
    m_thumbDownloader = new MovieClient();
    connect(m_thumbDownloader, SIGNAL(slotPosterFinished(const QImage&)), this, SLOT(setImage(const QImage&)));
}

MovieThumbs::~MovieThumbs()
{
    delete m_thumbDownloader;
}

bool MovieThumbs::create(const QString &path, int width, int /*h*/, QImage &img)
{
    if (Solid::Networking::status() == Solid::Networking::Unconnected) {
        kDebug() << "No network connection available";
        return false;
    }

    //Large or normal thumbnail
    if (size != width) {
        m_thumbDownloader->setSize(width);
        size = width;
    }

    //Reset image
    thumbnail = QImage();

    //Search and download thumbnail
    m_thumbDownloader->addSearch(path);
    img = thumbnail;

    return !img.isNull();
}

void MovieThumbs::setImage(const QImage &thumb)
{
    thumbnail = thumb;
}

ThumbCreator::Flags MovieThumbs::flags() const
{
    return (Flags)(None);
}

#include "moviethumbs.moc"
