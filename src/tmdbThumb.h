
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

#ifndef _TMDBTHUMB_H_
#define _TMDBTHUMB_H_

#include <QtGui/QImage>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class TmdbThumb : public QObject
{
Q_OBJECT
public:
    TmdbThumb(QString&);
    ~TmdbThumb();
    QImage getPoster();

private:
    static const QString KEY;
    QNetworkAccessManager* m_networkManager;
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

#endif // _TMDBTHUMB_H_