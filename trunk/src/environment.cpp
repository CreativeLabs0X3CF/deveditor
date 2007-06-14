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
  badExts << "o" << "exe" << "xpm"; //TODO Add all extensions of images, etc.
}

Environment::~Environment() {
}

QString Environment::strippedName(const QString &fullFileName) {
  return QFileInfo(fullFileName).fileName();
}

QString Environment::lastDir(const QString &fullFileName) {
  if (!exists(fullFileName)) {
    QString path = fullFileName;
    if (path.data()[path.length() - 1] == separator())
      path.chop(1);
    return path;
  }

  QStringList dirs = QFileInfo(fullFileName).absolutePath().split(separator());
  return dirs[dirs.count() - 1];
}

bool Environment::mkdir(const QString &fullFileName) {
  return QDir().mkdir(fullFileName);
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

bool Environment::mkfile(const QString &dir, const QString &name, bool useTemplate) {
  if (!isDir(dir))
    return false;

  QString path = dir + separator() + name;
  if (exists(path))
    return false;

  QFileInfo info(name);

  QString templateName = ":/" + info.completeSuffix() + ".template";

  if (useTemplate && exists(templateName)) {
    QFile file(templateName);
    bool ret = file.copy(path);
    if (ret)
      QFile(path).setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ReadUser | QFile::WriteUser | QFile::ReadGroup | QFile::WriteGroup);
    return ret;
  } else {
    QFile file(path);
    if (!file.open(QFile::WriteOnly | QFile::Text))
      return false;
  }

  return true;
}

QString Environment::home() const {
  return QDir::toNativeSeparators(QDir::homePath());
}

QChar Environment::separator() const {
  return QDir::separator();
}

QStringList Environment::listViewableFiles(const QString &dir) {
  QFileInfoList aux = QDir(dir).entryInfoList();

  QStringList list;
  for (int i(0); i < aux.count(); ++i)
    if (aux[i].isReadable() && aux[i].isFile() && !aux[i].isHidden() && !aux[i].isExecutable()) {
      // Good, I can read it, it's visible and it's not a directory. But does it have an unacceptable extension?
      bool bad = false;
      QString suf = aux[i].suffix();
      for (int j(0); j < badExts.count(); ++j)
        if (badExts[j] == suf) {
          bad = true;
          break;
        }
      if (suf.data()[suf.length() - 1] == '~')
        bad = true;

      if (!bad)
        list.push_back(unitePath(QStringList(dir) << aux[i].fileName()));
    }

  return list;
}

QString Environment::unitePath(const QStringList &list) const {
  QString result;
  for (int i(0); i < list.count(); ++i) {
    result += list[i];
    if ((i != list.count() - 1) && (result.data()[result.length() - 1] != separator()))
      result += separator();
  }
  return result;
}

bool Environment::isReadableFile(const QString &path) {
  QFileInfo info(path);

  if (info.exists() && info.isFile() && info.isReadable())
    return true;

  return false;
}
