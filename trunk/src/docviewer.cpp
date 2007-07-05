/***************************************************************************
 *   Copyright (C) 2007 by Alexandru Scvortov   *
 *   scvalex@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QtAssistant>

#include "docviewer.h"

#ifdef Q_OS_WIN32
#include "mainwindow.h"
#include "config.h"
#endif

#include "environment.h"

DocViewer::DocViewer(QWidget *_parent) : parent(_parent) {
    env = new Environment();

    docDir = QString("%1/%2").arg(QDir::homePath()).arg(".cppreference");
}

DocViewer::~DocViewer() {
    delete env;
}

void DocViewer::show() {
//     qWarning("%s", QDir(":/reference").entryList().join("\n").toStdString().c_str());

    if (!env->areSimilar(":/reference", docDir)) {
        if (!env->exists(docDir))
            env->mkdir(docDir);

        if (!env->dupDir(":/reference", docDir))
            qWarning("Unable to build documentation");
    }

#ifdef Q_OS_UNIX
    static QAssistantClient *ass = new QAssistantClient("", parent);
    static bool initRun = true;

    if (initRun) {
        QStringList arguments;
        arguments << "-profile" << QString("%1/creference.adp").arg(docDir);
        ass->setArguments(arguments);

        initRun = false;
    }

    ass->showPage("");
#endif

#ifdef Q_OS_WIN32
    static bool initRun = true;

    static Config *conf = Config::loadConfig(QString("%1/creference.adp").arg(docDir));
    static MainWindow *mw = new MainWindow();
    static QStringList links;

    if (initRun) {
//       QStringList links = conf->source();

        mw->setObjectName("Assistant");

        links << QString("%1/index.html").arg(docDir);

        initRun = false;
    }

    mw->showLinks(links);
    mw->show();
#endif
}
