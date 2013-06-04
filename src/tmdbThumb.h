
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

#ifndef TMDBTHUMB_H
#define TMDBTHUMB_H

#include <QtGui/QImage>
#include <QtNetwork/QNetworkReply>

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
class QNetworkReply;
QT_END_NAMESPACE

class TmdbThumb : public QObject
{
    Q_OBJECT

public:
    TmdbThumb(const QString& name, const QString& year, QNetworkAccessManager *qnam);
    QImage getPoster();

private:
    static const QString KEY;
    QNetworkAccessManager *networkManager;
    QImage moviePoster;

private slots:
    void queryFinished();
    bool downloadFinished();
    void onNetworkError(QNetworkReply::NetworkError);
    void slotSslErrors(const QList<QSslError>& sslErrors);

signals:
    void posterDownloaded();
    void downloadError();
};

#endif // TMDBTHUMB_H
