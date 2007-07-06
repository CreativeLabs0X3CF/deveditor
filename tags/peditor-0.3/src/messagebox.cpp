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


#include <QtCore>
#include <QtGui>

#include "messagebox.h"

MessageBox::MessageBox(QWidget *parent) : QTextBrowser(parent), msgs("") {
    setOpenLinks(false);

    connect(this, SIGNAL(anchorClicked(const QUrl&)), this, SLOT(moveTo(const QUrl&)));
}

MessageBox::~MessageBox() {}

void MessageBox::moveTo(const QUrl &url) {
//   qWarning("URL is %s", url.toString().toStdString().c_str());
    if (url.toString().indexOf('#') != -1) {
        QString aux = url.toString();
        aux.chop(aux.count() - aux.indexOf('#'));
        emit switchToFile(aux);
//     qWarning("Switching to file %s", aux.toStdString().c_str());
        emit moveToLine(url.toString().right(url.toString().count() - url.toString().indexOf('#') - 1).toInt());
        qWarning("Moving to line %d", url.toString().right(url.toString().count() - url.toString().indexOf('#') - 1).toInt());
    } else {
        emit switchToFile(url.toString());
//     qWarning("Switching to file %s", url.toString().toStdString().c_str());
    }
}

void MessageBox::reset() {
    msgs = "";
    setHtml(msgs);
}

void MessageBox::message(const QString &text) {
    msgs += "<div>" + text + "</div>";
    setHtml(msgs);
}

void MessageBox::warn(const QString &text) {
    QString fileName = "";
    QString lineNoStr = "";
    int c = 0;
    while ((c < text.count()) && (text[c] != ':'))
        fileName += text[c++];

    ++c;

    while ((c < text.count()) && (text[c] != ':'))
        lineNoStr += text[c++];

    if (lineNoStr.toInt()) {
        msgs += QString("<a href=\"%1#%2\"><div style=\"color: #F479F4; text-decoration: none;\">").arg(fileName).arg(lineNoStr) + text + "</div></a>";
    } else {
        msgs += QString("<a href=\"%1\"><div style=\"color: #F479F4; text-decoration: none;\">").arg(fileName) + text + "</div></a>";
    }
    setHtml(msgs);
}

void MessageBox::error(const QString &text) {
    QString fileName = "";
    QString lineNoStr = "";
    int c = 0;
    while ((c < text.count()) && (text[c] != ':'))
        fileName += text[c++];

    ++c;

    while ((c < text.count()) && (text[c] != ':'))
        lineNoStr += text[c++];

    if (lineNoStr.toInt()) {
        msgs += QString("<a href=\"%1#%2\"><div style=\"color: #FF0000; text-decoration: none;\">").arg(fileName).arg(lineNoStr) + text + "</div></a>";
    } else {
        msgs += QString("<a href=\"%1\"><div style=\"color: #FF0000; text-decoration: none;\">").arg(fileName) + text + "</div></a>";
    }
    setHtml(msgs);
}

void MessageBox::good(const QString &text) {
    msgs += "<div style=\"color: #008000;\">" + text + "</div>";
    setHtml(msgs);
}
