
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

#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QtCore/QCache>
#include <QtGui/QImage>
#include <QtNetwork/QNetworkReply>

class QNetworkAccessManager;
class QNetworkReply;
class QSslError;
class QString;

class DownloadManager : public QObject
{
    Q_OBJECT

public:
    DownloadManager(QNetworkAccessManager *qnam);
    virtual ~DownloadManager();

    bool duplicate(const QString& name, const QString& year);

    void startDownload();
    void copyImage(QImage* image);

    QImage Poster();
    bool hasPoster;

protected:
    void setUrl(QUrl url);
    QNetworkAccessManager *networkManager;
    QString language();

    //TODO Remove global and use storeImage(QString name)
    QString nameKey;

private:
    virtual void startSearch(const QUrl& query) = 0;

    QImage poster;
    QUrl posterLink;
    QCache<QString, QImage> cache;

public slots:
    void storeImage();

private slots:
    void downloadFinished();
    void onNetworkError(QNetworkReply::NetworkError);
    void slotSslErrors(const QList<QSslError>& sslErrors);

signals:
    void posterDownloaded();
    void downloadError();
    void posterFound();
};

#endif // DOWNLOADMANAGER_H
