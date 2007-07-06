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


#include "aboutwidget.h"

#include <QtCore>
#include <QtGui>

AboutWidget::AboutWidget(QWidget *parent) : QDialog(parent) {
    setupUi();
}

AboutWidget::~AboutWidget() {
}

void AboutWidget::setupUi() {
    QVBoxLayout *ml = new QVBoxLayout;

        QHBoxLayout *cl = new QHBoxLayout;

            QLabel *label = new QLabel(this);
            label->setPixmap(QPixmap(":/pi_icon.png").scaledToHeight(16));
        cl->addWidget(label);

            label = new QLabel(tr("PEditor 0.2"), this);
        cl->addWidget(label);
        cl->setAlignment(label, Qt::AlignLeft);
    ml->addLayout(cl);

        QTabWidget *tw = new QTabWidget(this);

            QWidget *w = new QWidget(this);
            QVBoxLayout *tl = new QVBoxLayout;

                label = new QLabel(tr("PEditor - an experimental programmer's editor"), this);
            tl->addWidget(label);
            tl->setAlignment(label, Qt::AlignCenter);

                label = new QLabel(tr("(c) 2007, Alexandru Scvortov"), this);
            tl->addWidget(label);
            tl->setAlignment(label, Qt::AlignCenter);

                label = new QLabel(tr("<a href=\"http://code.google.com/p/deveditor/\">http://code.google.com/p/deveditor/</a>"), this);
            tl->addWidget(label);
            tl->setAlignment(label, Qt::AlignCenter);

            w->setLayout(tl);
        tw->addTab(w, tr("&About"));

            QTextBrowser *tb = new QTextBrowser(this);
            QFile file(":/AUTHORS");
            file.open(QFile::ReadOnly | QFile::Text);
            tb->setHtml(file.readAll());
        tw->addTab(tb, tr("A&uthors"));

            tb = new QTextBrowser(this);
            QFile file2(":/COPYING");
            file2.open(QFile::ReadOnly | QFile::Text);
            tb->setPlainText(file2.readAll());
        tw->addTab(tb, tr("&License Agreement"));

    ml->addWidget(tw);

    setLayout(ml);

    setFixedWidth(450);
}
