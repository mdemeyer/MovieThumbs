
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

#include <QCoreApplication>
#include <QObject>
#include <QString>

#include <unistd.h>    /*getopt*/

#include "tools.h"

using namespace std;

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    int argument;
    int size = 0;
    QString input;
    QString output;

    Tools tool;

    while ((argument = getopt (argc, argv, "i:o:s::hv")) != -1) {
        switch (argument) {
            case 'i':
                // Input file
                input = optarg;
                break;
            case 'o':
                // Output file
                output = optarg;
                break;
            case 's':
                // Size
                size = optarg ? atoi(optarg) : 128;
                break;
            case 'h':
                tool.printHelp();
                return 0;
            case 'v':
                tool.printVersion();
                return 0;
            case '?':
            default:
                tool.printHelp();
                return -1;
        }
    }

    if(!input.isNull() && !output.isNull()) {
        tool.createThumbnail(input, output, size);
    } else {
        tool.printHelp();
    }

    return 0;
}
