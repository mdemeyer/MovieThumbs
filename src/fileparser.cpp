
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
    QRegExp regex("\\([^\\(]*\\)|\\[([^]]+)\\]");
    clean.remove(regex);

    /* Remove all non alphanumerical characters from the name and replace them with a space.
     * This way you can use dots and underscores in filenames.
     */
    regex.setPattern("[^a-zA-Z0-9\\s]");
    clean.replace(regex, " ");

    /* Remove the S00E00 part from series. TheTvdb does not recognise it.
     */
    regex.setPattern("[sS]([0-9]+)[eE]([0-9]+)");
    clean.remove(regex);

    /* Remove the year from the name.
     */
    if(clean.length() > 4) //movie 2012
        regex.setPattern("(19|20)\\d{2}");
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
    QRegExp regex("(19|20)\\d{2}");
    if (regex.lastIndexIn(name) != -1) {
        if (!regex.isEmpty()) {
            return regex.cap(0);
        }
    }
    return QString();
}

bool FileParser::isSeries(const QString &name)
{
    QRegExp regex("[sS]([0-9]+)[eE]([0-9]+)");
    if (regex.lastIndexIn(name) != -1) {
        if (!regex.isEmpty()) {
            return true;
        }
    }
    return false;
}

#include "fileparser.moc"
