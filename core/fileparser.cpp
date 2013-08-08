
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

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>

#include <QDebug>

/* REGEXSERIES
 * [0-9]+                 Can be one or two numbers.
 * (19|20)\\d{2}          year
 * 0[1-9] | 1[012]        Number from 01 to 09 OR 10 to 12 (month)
 * (0[1-9]|[12]\\d)|3[01] Number 01 - 31 (day)
 * (?i)                   Ignore case
 * [\\[|\\(]              '[' or '['
 * [a-zA-Z0-9\\s]{8}      8 alphanumeric chars
 * \s Matches a whitespace character (QChar::isSpace()).
 */
const QStringList FileParser::REGEXSERIES = QStringList()
                    << "[sS]([0-9]+)\\s*[eE]([0-9]+)(.*)" // S00E00
                    << "\\s[Ee][Pp]\\s?([0-9]+)(.*)" // Ep.00
                    << "\\s([0-9]+)x[0-9]{2}\\s" // blah.100
                    << "\\s(?i)p(?:ar)?t\\s" //Part Pt.
                    << "(19|20)\\d{2}\\s((0[1-9])|(1[012]))\\s((0[1-9]|[12]\\d)|3[01])"  // yyyy-mm-dd
                    << "((0[1-9]|[12]\\d)|3[01])\\s((0[1-9])|(1[012]))\\s(19|20)\\d{2}"  // dd-mm-yyyy
                    << ("[\\[|\\(][a-zA-Z0-9]{8}[\\]|\\)]"); //[abCD5678] or (abCD5678)

/* REGEXSEPARATORS
 * Remove dots, underscores, etc. from filenames
 */
const QString FileParser::REGEXSEPARATORS = "[_\\-.]";

/* REGEXSPECIAL
 * Used to remove special characters
 * \W Matches a non-word character.
 * Thank you Giuseppe Calà
 */
const QString FileParser::REGEXSPECIAL = "[^\\w\\s]";

/* REGEXBRACKETS
 * \\(               Include opening bracket
 * [^\\(]            Start match
 * *\\)              Match everyting until closing bracket
 */
// const QString FileParser::REGEXBRACKETS = "\\([^\\(]*\\)|\\[([^]]+)\\]";
const QString FileParser::REGEXBRACKETS = "\\([^\\(]*\\)|\\[([^]]+)\\]|\\{([^}]+)\\}";

/* REGEXYEAR
 * \(19|20) number starting with 19 OR 20
 * \d{2} 2 numbers [0-9]
 */
const QString FileParser::REGEXYEAR = "(19|20)\\d{2}";

/* REGEXCD.
 * \\s* zero or more whitespaces
 * \\d+ 1 or more numbers
 */
const QString FileParser::REGEXCD = "[C|c][D|d]\\s*\\d+";

const QStringList FileParser::BLACKLIST = readBlacklist();

FileParser::FileParser()
{
}

FileParser::~FileParser()
{
}

QString FileParser::baseName(const QString &path)
{
    //remove file extension
    QFileInfo file(path);
    QString clean = file.completeBaseName();

    //remove separator characters
    QRegExp regex(REGEXSEPARATORS);
    clean.replace(regex, " ");

    return clean.simplified();
}

QString FileParser::cleanName(const QString &name)
{
    QString clean = name;

    //Ignore all information between brackets.
    QRegExp regex(REGEXBRACKETS);
    clean.remove(regex);

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

    return clean.simplified();
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

QString FileParser::filterBlacklist(const QString &name)
{
    QString clean = name;

    //Remove special characters.
    QRegExp regex(REGEXSPECIAL);
    clean.remove(regex);

    //Remove CD 1
    regex.setPattern(REGEXCD);
    clean.remove(regex);

    //Remove blacklisted words
    foreach(const QString& word, BLACKLIST) {
        if(clean.contains(word, Qt::CaseInsensitive)) {
            clean.truncate(clean.indexOf(word, 0, Qt::CaseInsensitive));
        }
    }
    return clean.simplified();
}

QStringList FileParser::readBlacklist()
{
    QStringList list;
    QString tempWord;

    //Open the file
    QFile file("/usr/share/MovieThumbs/blacklist");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //Something went wrong: return empty list
        qDebug() << "blacklist file not found!";
        return list;
    }

    //Read every line and store it in the list
    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        tempWord = stream.readLine();
        if(!tempWord.startsWith('#') && !tempWord.isEmpty()) {
            list << tempWord.trimmed();
        }
    }
    return list;
}

#include "fileparser.moc"
