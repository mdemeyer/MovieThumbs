
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

/*
 * [0-9]+                 Can be one or two numbers.
 * (19|20)\\d{2}          year
 * 0[1-9] | 1[012]        Number from 01 to 09 OR 10 to 12 (month)
 * (0[1-9]|[12]\\d)|3[01] Number 01 - 31 (day)
 * [_- .]                 Date seperator
 */
const QStringList FileParser::REGEXSERIES = QStringList()
                    << "[sS]([0-9]+)[eE]([0-9]+)" // S00E00
                    << "(19|20)\\d{2}[_- .]((0[1-9])|(1[012]))[_- .]((0[1-9]|[12]\\d)|3[01])";  // yyyy-mm-dd

const QString FileParser::REGEXALPHANUMERIC = "[^a-zA-Z0-9\\s]";
const QString FileParser::REGEXBRACKETS = "\\([^\\(]*\\)|\\[([^]]+)\\]";
const QString FileParser::REGEXYEAR = "(19|20)\\d{2}";

FileParser::FileParser()
{
}

FileParser::~FileParser()
{
}

QString FileParser::cleanName(const QString &path)
{
    QFileInfo file(path);
    QString clean = file.completeBaseName(); //remove file extension

    /*Ignore all information between brackets.
     * Works with both () and []
     * TODO Clean up and improve the regular expression. This makes my head hurt.
     */
    QRegExp regex(REGEXBRACKETS);
    clean.remove(regex);

    /* Remove all non alphanumerical characters from the name and replace them with a space.
     * This way you can use dots and underscores in filenames.
     */
    regex.setPattern(REGEXALPHANUMERIC);
    clean.replace(regex, " ");

    /* Remove the series detection part from the name. TheTvdb does not recognise it.
     */
    QStringList::const_iterator constIterator;
    for (constIterator = REGEXSERIES.constBegin(); constIterator != REGEXSERIES.constEnd(); ++constIterator) {
        regex.setPattern(*constIterator);
        clean.remove(regex);
    }

    /* Remove the year from the name.
     */
    if(clean.length() > 4) //movie 2012
        regex.setPattern(REGEXYEAR);
        clean.remove(regex);

    return clean;
}

QString FileParser::year(const QString &name)
{
    /*If there is a year included in the title use it to refine the search
     * \(19|20) number starting with 19 OR 20
     * \d{2} followed by 2 numbers [0-9]
     * This works for all movies released from 1900 to 2099.
     */
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
    /* Check if the file is a series. We look for these strings:
     * S00E00: S and E can be lower or uppercase. 00 can be any number
     * yyyy-mm-dd: Full date. This is also a common naming scheme.
     */
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
