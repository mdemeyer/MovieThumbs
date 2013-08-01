
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

#include <iostream>
#include <unistd.h>    /*getopt*/

#include "tools.h"

using namespace std;

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    int argument;
    int size = 128;
    QString input;
    QString output;

    Tools tool;

    while ((argument = getopt (argc, argv, "i:o:s:h")) != -1) {
        switch (argument) {
            case 'i':
                cout << "Input file: " << optarg << endl;
                input = optarg;
                break;
            case 'o':
                cout << "Output file: " << optarg << endl;
                output = optarg;
                break;
            case 's':
                cout << "size" << endl;
                size = atoi(optarg);
                break;
            case 'h':
                tool.printHelp();
                return 0;
            case '?':
            default:
                tool.printHelp();
                return -1;
        }
    }

//     cout << "Starting download" << endl;
    tool.createThumbnail(input, output, size);

return 0;
}
