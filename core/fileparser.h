
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

#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <QtCore/QObject>

class QString;
class QStringList;

class FileParser : public QObject
{
    Q_OBJECT
public:
    FileParser();
    ~FileParser();

    static QString baseName(const QString& path);
    static QString cleanName(const QString& name);
    static QString year(const QString& name);
    static QString filterBlacklist(const QString& name);

    static bool isSeries(const QString& name);

private:
    /* Regex to find series. We look for these strings:
     * S00E00: S and E can be lower or uppercase. 00 can be any number
     * Ep.00 Episode number
     * 1x00
     * Part or Pt.
     * yyyy-mm-dd: Full date. This is also a common naming scheme.
     * dd-dd-yyyy: Full date reversed.
     * [xxxxxxxx]: 8 character long checksum. Commonly used with anime.
     */
    static const QStringList REGEXSERIES;

    /* Regex to detect separators in filenames.
     */
    static const QString REGEXSEPARATORS;

    /* Regex to detect special characters.
     */
    static const QString REGEXSPECIAL;

    /* Regex to select everything between brackets. Both () and []
     */
    static const QString REGEXBRACKETS;

    /* Regex to detect a year.
     * This works for all years between 1900 and 2099.
     */
    static const QString REGEXYEAR;

    /* Regex to filter cd from name.
     */
    static const QString REGEXCD;

    /* List with words to filter out.
     * Read from $XDG_DATA_DIRS/MovieThumbs/blacklist
     */
    static const QStringList BLACKLIST;
    static QStringList readBlacklist();
};

#endif // FILEPARSER_H
