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

//TODO Turn browser into a QListView (that's how they do it in KDevelop)

MessageBox::MessageBox(QWidget *parent) : QWidget(parent) {
    msgM = new QStringListModel(this);

    QVBoxLayout *lay = new QVBoxLayout();
    lay->setMargin(0);

    browser = new QTextBrowser(this);
//     browser->setModel(msgM);
    lay->addWidget(browser);

    setLayout(lay);

    browser->setOpenLinks(false);

    connect(browser, SIGNAL(anchorClicked(const QUrl&)), this, SLOT(moveTo(const QUrl&)));
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
    msgs.clear();

    updateModel();
}

void MessageBox::message(const QString &text) {
    msgs << QString("<div>%1</div>").arg(text);

    updateModel();
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
        msgs << QString("<div><a href=\"%1#%2\" style=\"color: #F479F4; text-decoration: none;\">%3</a></div>").arg(fileName).arg(lineNoStr).arg(text);
    } else {
        msgs << QString("<div><a href=\"%1\" style=\"color: #F479F4; text-decoration: none;\">%2</a></div>").arg(fileName).arg(text);
    }

    updateModel();
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
        msgs << QString("<div><a href=\"%1#%2\" style=\"color: #FF0000; text-decoration: none;\">%3</a></div>").arg(fileName).arg(lineNoStr).arg(text);
    } else {
        msgs << QString("<div><a href=\"%1\" style=\"color: #FF0000; text-decoration: none;\">%2</a></div>").arg(fileName).arg(text);
    }

    updateModel();
}

void MessageBox::good(const QString &text) {
    msgs << QString("<div><span style=\"color: #008000;\">%1</span></div>").arg(text);

    updateModel();
}

void MessageBox::updateModel() {
    msgM->setStringList(msgs);

    browser->setHtml(msgM->stringList().join(""));
}
