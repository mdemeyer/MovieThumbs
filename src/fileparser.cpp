
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

#include "fileparser.h"

#include <QtCore/QFileInfo>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtCore/QStringList>

/* REGEXSERIES
 * [0-9]+                 Can be one or two numbers.
 * (19|20)\\d{2}          year
 * 0[1-9] | 1[012]        Number from 01 to 09 OR 10 to 12 (month)
 * (0[1-9]|[12]\\d)|3[01] Number 01 - 31 (day)
 * [_- .]                 Date separator
 */
const QStringList FileParser::REGEXSERIES = QStringList()
                    << "[sS]([0-9]+)[eE]([0-9]+)" // S00E00
                    << "(19|20)\\d{2}[_- .]((0[1-9])|(1[012]))[_- .]((0[1-9]|[12]\\d)|3[01])";  // yyyy-mm-dd

/* REGEXALPHANUMERIC
 *TODO Allow special characters from foreign languages
 */
const QString FileParser::REGEXALPHANUMERIC = "[^a-zA-Z0-9\\s]";

/* REGEXBRACKETS
 * \\(               Include opening bracket
 * [^\\(]            Start match
 * *\\)              Match everyting until closing bracket
 */
const QString FileParser::REGEXBRACKETS = "\\([^\\(]*\\)|\\[([^]]+)\\]";

/* REGEXYEAR
 * \(19|20) number starting with 19 OR 20
 * \d{2} 2 numbers [0-9]
 */
const QString FileParser::REGEXYEAR = "(19|20)\\d{2}";

FileParser::FileParser()
{
}

FileParser::~FileParser()
{
}

QString FileParser::cleanName(const QString &path)
{
    //remove file extension
    QFileInfo file(path);
    QString clean = file.completeBaseName();

    //Ignore all information between brackets.
    QRegExp regex(REGEXBRACKETS);
    clean.remove(regex);

    // Remove all non alphanumerical characters from the name.
    regex.setPattern(REGEXALPHANUMERIC);
    clean.replace(regex, " ");

    // Remove the series detection part from the name. TheTvdb does not recognise it.
    QStringList::const_iterator constIterator;
    for (constIterator = REGEXSERIES.constBegin(); constIterator != REGEXSERIES.constEnd(); ++constIterator) {
        regex.setPattern(*constIterator);
        clean.remove(regex);
    }

    // Remove the year from the name.
    if(clean.length() > 4) //movie 2012
        regex.setPattern(REGEXYEAR);
        clean.remove(regex);

    return clean.trimmed();
}

QString FileParser::year(const QString &name)
{
    //If there is a year included in the title use it to refine the search
    QRegExp regex(REGEXYEAR);
    if (regex.lastIndexIn(name) != -1) {
        if (!regex.isEmpty()) {
            return regex.cap(0);
        }
    }
    return QString();
}

bool FileParser::isSeries(const QString &name)
{
    // Check if the file is a series.
    QRegExp regex;

    QStringList::const_iterator constIterator;
    for (constIterator = REGEXSERIES.constBegin(); constIterator != REGEXSERIES.constEnd(); ++constIterator) {
        regex.setPattern(*constIterator);
        if (regex.lastIndexIn(name) != -1) {
            if (!regex.isEmpty()) {
                return true;
            }
        }
    }
    return false;
}

#include "fileparser.moc"
