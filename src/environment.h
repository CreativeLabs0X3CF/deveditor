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


#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

class QString;

/*!
  Class that provieds data about the environment --- platform, paths, etc.
	@author Alexandru Scvortov <scvalex@gmail.com>
*/
class Environment{
public:
  Environment();

  ~Environment();

  //! @return The base --- last part of the filename.
  QString strippedName(const QString &fullFileName);

  //! @return The last directory in the path, i.e. the programme name.
  QString lastDir(const QString &fullFileName);

  //! Creates a the specified directory if it does no exist.
  void mkdir(const QString &fullFileName);

  //! @return True if the file or directory exists. False otherwise.
  bool exists(const QString &fullFileName);

  //! @return True if the path exists and is a directory. False otherwise.
  bool isDir(const QString &path);

protected:
  //! True if platform is Windows. False otherwise.
  bool isWindows;

  //! True if platform is Unix. False otherwise.
  bool isUnix;
};

#endif
