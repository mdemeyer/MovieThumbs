
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
#include <QImage>
#include <QFile>

#include <iostream>
#include <movieclient.h>

#include "tools.h"

using namespace std;

QString outputFile;
int fileSize;

Tools::Tools()
{
}

Tools::~Tools()
{
}

void Tools::printVersion()
{
    cout << "VERSION: 0.3.1" << endl;
}

void Tools::printHelp()
{
    cout << endl
         << "Usage: moviethumbs [options]" << endl << endl
         << "Options:" << endl
         << "  -i<s>   : input file" << endl
         << "  -o<s>   : output file" << endl
         << "  -s<n>   : thumbnail size (default: 128)" << endl
         << "  -v      : print version number" << endl
         << "  -h      : show this help" << endl;
}

void Tools::savePoster(const QImage poster)
{
    QImage saveFile = poster;

    //Resize the image
    if(fileSize != 0 && saveFile.height() != fileSize) {
         saveFile = saveFile.scaledToHeight(fileSize);
    }

    //Save the file on disk
    if(!saveFile.save(outputFile, "PNG", 0)) {
        cerr << "Cannot save file!" << endl;
    }
}

void Tools::createThumbnail(const QString &input, const QString &output, int size)
{
    outputFile = output;
    fileSize = size;

    MovieClient *thumb = new MovieClient();
    connect(thumb, SIGNAL(slotPosterFinished(const QImage&)), this , SLOT(savePoster(const QImage&)));

    thumb->addSearch(input);
}
