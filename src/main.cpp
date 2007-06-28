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


#include <QApplication>
#include <QSplashScreen>
#include "peditor.h"

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(application);
    QApplication app(argc, argv);

    QPixmap pixmap(":/peditor.png");
    QSplashScreen splash(pixmap);
    splash.showMessage(QObject::tr("Loading..."), Qt::AlignLeft | Qt::AlignBottom);
    splash.show();
    app.processEvents();

    PEditor mw;
    splash.finish(&mw);

//     if (argc == 1)
//         mw = new PEditor();
//     else
//         mw = new PEditor(argv[1]);

    mw.show();

    return app.exec();
}
