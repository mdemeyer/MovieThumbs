
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

#include <QtCore/QObject>
#include <QImage>

#include <kio/thumbcreator.h>
#include <libmoviethumbs/movieclient.h>

class QNetworkConfigurationManager;

class MovieThumbs : public QObject, public ThumbCreator
{
    Q_OBJECT

public:
    MovieThumbs();
    virtual ~MovieThumbs();
    virtual bool create(const QString& path, int width, int height, QImage& img);
    virtual Flags flags() const;

    MovieClient *m_thumbDownloader;

public slots:
    void setImage(const QImage &);

private:
    int size;
    QNetworkConfigurationManager *m_qncm; // used to monitor internet connection status
};

#endif // MOVIETHUMBS_H
