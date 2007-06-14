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

#include "environment.h"

Environment::Environment() {
}

Environment::~Environment() {
}

QString Environment::strippedName(const QString &fullFileName) {
  return QFileInfo(fullFileName).fileName();
}

QString Environment::lastDir(const QString &fullFileName) {
  if (!exists(fullFileName))
    return fullFileName;

  QStringList dirs = QFileInfo(fullFileName).absolutePath().split("/");
  return dirs[dirs.count() - 1];
}

void Environment::mkdir(const QString &fullFileName) {
  QDir().mkdir(fullFileName);
}

bool Environment::exists(const QString &fullFileName) {
  if (QDir(fullFileName).exists() || QFile(fullFileName).exists())
    return true;
  return false;
}

bool Environment::isDir(const QString &path) {
  if (QDir(path).exists())
    return true;
  return false;
}
