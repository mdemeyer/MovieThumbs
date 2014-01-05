
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

#ifndef MOVIESERVICE_H
#define MOVIESERVICE_H

#include "posterservice.h"

class QNetworkAccessManager;
class QSslError;

class MovieService : public PosterService
{
    Q_OBJECT

public:
    MovieService(QNetworkAccessManager *qnam) : PosterService(qnam) {}
    void startSearch(const QString& name, const QString& year);

private:
    static const QString KEY;

private slots:
    void searchFinished();
    void slotSslErrors(const QList<QSslError>& sslErrors);
};

#endif // MOVIESERVICE_H
