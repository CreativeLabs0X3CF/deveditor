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

#include <QDialog>

class QString;
class MessageBox;
class QAction;
class QLineEdit;
class QLabel;
class Environment;

/*!
  Class that determines information about a programme.
  @author Alexandru Scvortov <scvalex@gmail.com>
*/
class ProgInfo {
public:
  //! @param _progName The path to the programme.
  ProgInfo(const QString &_progName);
  ~ProgInfo();

  //! @return List all source files in the current programme.
  QStringList sourceFiles();

  //! @return List all object files in the current programme.
  QStringList objectFiles();

  //! Sets the progamme.
  void setProg(const QString &_progName);

  //! Returns the path to the executable.
  QString exe();

protected:
  QString progName;
};

/*!
  Class that allows users to change many of Environment's variables.
  @author Alexandru Scvortov <scvalex@gmail.com>
*/
class EnvironmentConfigurationWidget : public QDialog {
  Q_OBJECT

public:
  //! @param _env The Environment the widget will be changing.
  EnvironmentConfigurationWidget(Environment *_env, QWidget *parent = 0);
  ~EnvironmentConfigurationWidget();

protected slots:
  void getCcPath();
  void getCppPath();
  void test();

protected:
  Environment *env;

  //! Sets up all visible widgets, and the connections to the underlaying functions.
  void setupUI();

  QPushButton *testButton;
  QPushButton *doneButton;

  QLineEdit *ccEdit;
  QLabel *ccStatusLabel;

  QLineEdit *cppEdit;
  QLabel *cppStatusLabel;
};

/*!
  Class that provides data about the environment --- platform, paths, etc.
	@author Alexandru Scvortov <scvalex@gmail.com>
*/
class Environment : public QObject {
  Q_OBJECT

public:
  Environment(QWidget *parent = 0);

  ~Environment();

  //! @return The base --- last part of the filename.
  QString strippedName(const QString &fullFileName);

  //! @return The last directory in the path, i.e. the programme name.
  QString lastDir(const QString &fullFileName);

  //! Creates a the specified directory if it does no exist.
  bool mkdir(const QString &fullFileName);

  //! @return True if the file or directory exists. False otherwise.
  bool exists(const QString &fullFileName);

  //! @return True if the path exists and is a directory. False otherwise.
  bool isDir(const QString &path);

  //! @return True if file is executable, false otherwise.
  bool isExe(const QString &path);

  /*!
    Creates the specified file in the specified directory. If the directory does not exist, aborts and returs false. On succesful exit, returns true.
    @param useTemplate If set to true, mkfile will search for a template and attempt to use it.
  */
  bool mkfile(const QString &dir, const QString &name, bool useTemplate = false);

  //! @return The path to the home directory.
  QString home() const;

  //! @return A list of all openable/viewable (non-binary) files in the specified directory.
  QStringList listViewableFiles(const QString &dir);

  //! @return The path resulting from combining the specified paths.
  QString unitePath(const QStringList &list) const;

  //! @return True if the path is to a file and the file is readable.
  bool isReadableFile(const QString &path);

  /*!
    Attempts to compile path.
    @return Returns true if compile was succesful, false otherwise.
  */
  bool compileFile(const QString &path, bool partOfBulkJob = false);

  //! Tells the Environment which, if any, MessageBox it should use.
  void setMessageBox(MessageBox *_mb);

  //! Links the objects in the specified directory.
  bool linkObjects(const QString &dir);

  //! Runs the specified executable.
  bool run(const QString &path);

  //! @return A pointer to the Environment's configure action.
  QAction* getConfigureAct();

  //! @return The path to the C compiler.
  QString getCc() const;

  //! @return The path to the C++ compiler.
  QString getCpp() const;

  //! Sets the path to the C compiler.
  void setCc(const QString &path);

  //! Sets the path to the C++ compiler.
  void setCpp(const QString &path);

protected:
  //! True if platform is Windows. False otherwise.
  bool isWindows;

  //! True if platform is Unix. False otherwise.
  bool isUnix;

  //! List of bad extensions.
  QStringList badExts;

  //! Path to compiler executables.
  QString cc;
  QString cpp;

  //! Default path to compiler executables/
  QString defaultCc;
  QString defaultCpp;

  //! The MessageBox used to display messages.
  MessageBox *mb;

  //! When triggered, shows a configuration dialog.
  QAction *configureAct;

  //! The environment's configuration dialog.
  EnvironmentConfigurationWidget *ecw;

  //! Saves the environment's settings.
  void writeSettings();

  //! Loads the environment's settings.
  void readSettings();
};

#endif
