
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

#ifndef MOVIETHUMBS_H
#define MOVIETHUMBS_H

#include "MovieThumbsConfig.h"

#include "movieservice.h"
#ifdef HAVE_TVDB
#include "tvservice.h"
#endif

#include <QtCore/QObject>
#include <kio/thumbcreator.h>

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
QT_END_NAMESPACE

class MovieThumbs : public QObject, public ThumbCreator
{
    Q_OBJECT

public:
    MovieThumbs();
    virtual ~MovieThumbs();
    virtual bool create(const QString& path, int width, int height, QImage& img);
    virtual Flags flags() const;

private:
    QNetworkAccessManager *m_networkManager;
    MovieService *m_movie;
#ifdef HAVE_TVDB
    TvService *m_series;
#endif
};

#endif // MOVIETHUMBS_H
