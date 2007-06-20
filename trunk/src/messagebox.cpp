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
}


MessageBox::~MessageBox() {
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
  msgs += "<div style=\"color: #F479F4;\">" + text + "</div>";
  setHtml(msgs);
}

void MessageBox::error(const QString &text) {
  msgs += "<div style=\"color: #FF0000;\">" + text + "</div>";
  setHtml(msgs);
}

void MessageBox::good(const QString &text) {
  msgs += "<div style=\"color: #008000;\">" + text + "</div>";
  setHtml(msgs);
}
