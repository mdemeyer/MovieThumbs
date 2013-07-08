
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

#include <QtCore/QCache>

class QNetworkAccessManager;

class TvService : public PosterService
{
    Q_OBJECT

public:
    explicit TvService(QNetworkAccessManager *qnam);
    ~TvService();
    void startSearch(const QString& name, const QString& year);
    bool duplicate(const QString& name, const QString& year);

private:
    static const QString KEY;

    QString nameKey;
    QCache<QString, QImage> cache;

private slots:
    void foundSeries();
    void foundBanners();
    void storeImage();
};

#endif // TVSERVICE_H
