
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
#include <QtCore/QRegExp>
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
    QString movieName = file.completeBaseName(); //remove file extension
    QString year;

    /*If there is a year included in the title use it to refine the search
     * \(19|20) number starting with 19 OR 20
     * \d{2} followed by 2 numbers [0-9]
     * This works for all movies released from 1900 to 2099.
     */
    QRegExp regex("(19|20)\\d{2}");
    if (regex.lastIndexIn(movieName) != -1) {
        if (!regex.isEmpty()) {
            year = regex.cap(0);
        }
    }
    
    /*Ignore all information between brackets.
     * Works with both () and []
     * TODO Clean up and improve the regular expression. This makes my head hurt.
     */
    regex.setPattern("\\([^\\(]*\\)|\\[([^]]+)\\]");
    movieName.remove(regex);

    /* Remove all non alphanumerical characters from the name and replace them with a space.
     * This way you can use dots and underscores in filenames.
     */
    regex.setPattern("[^a-zA-Z0-9\\s]");
    movieName.replace(regex, " ");

    TmdbThumb movie(movieName, year);

    QEventLoop loop;
    QObject::connect(&movie, SIGNAL(posterDownloaded()), &loop, SLOT(quit()));
    QObject::connect(&movie, SIGNAL(downloadError()), &loop, SLOT(quit()));
    loop.exec();

    img = movie.getPoster();

    if (!img.isNull()) {
        return true;
    } else
        return false;
}

ThumbCreator::Flags MovieThumbs::flags() const
{
    return (Flags)(None);
}

#include "movieThumbs.moc"