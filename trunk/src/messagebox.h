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


#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QWidget>

class QStringListModel;
class QListView;
class QTextBrowser;
class QUrl;

/*!
  @author Alexandru Scvortov <scvalex@gmail.com>
*/
class MessageBox : public QWidget {
    Q_OBJECT

public:
    MessageBox(QWidget *parent = 0);

    ~MessageBox();

    //! Resets the MessageBox.
    void reset();

    //! Shows a normal message.
    void message(const QString &text);

    //! Shows a warning.
    void warn(const QString &text);

    //! Shows an error.
    void error(const QString &text);

    //! Shows something good.
    void good(const QString &text);

signals:
    void switchToFile(const QString &fileName);
    void moveToLine(int);

private slots:
    //! Recives open link requests.
    void moveTo(const QUrl &url);

private:
    QStringList msgs;
    QStringListModel *msgM;

    QTextBrowser *browser;

    void updateModel();
};

#endif
