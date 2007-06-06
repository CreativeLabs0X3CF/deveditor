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


#ifndef DEVEDITOR_H
#define DEVEDITOR_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QTextEdit>

class QAction;
class QMenu;
class QToolButton;
class QTabWidget;
class Highlighter;

class TextEditWidget : public QTextEdit {
  Q_OBJECT

public:
  TextEditWidget(QWidget *parent = 0);
  ~TextEditWidget();

  QString getCurFile() const;
  QString getShownName() const;

  void setCurFile(QString _curFile);
  void setShownName(QString _shownName);

protected:
  QString curFile;
  QString shownName;
};

//! Main window
class DevEditor:public QMainWindow {
  Q_OBJECT

public:
  DevEditor();
  //!@param _filename Path to the file to be open.
  DevEditor(char *_filename);
  ~DevEditor();

protected:
  void closeEvent(QCloseEvent *event);

private slots:
  void newFile();
  void open();
  bool save();
  bool saveAs();
  void about();
  void documentWasModified();
  void toggleSyntaxHighlighting();
  void switchToTab(int);
  void removeTab();

private:
  void init();

  void createActions();
  void createMenus();
  void createToolBars();
  void createStatusBar();
  void readSettings();
  void writeSettings();
  bool maybeSave();
  void loadFile(const QString &fileName);
  bool saveFile(const QString &fileName);
  void setCurrentFile(const QString &fileName);
  QString strippedName(const QString &fullFileName);
  void closeTab(int index, bool force = false);

  QTabWidget *tabWidget;
  TextEditWidget *textEdit;
  QString curFile;
  QString shownName;

  QMenu *fileMenu;
  QMenu *editMenu;
  QMenu *viewMenu;
  QMenu *helpMenu;
  QToolBar *fileToolBar;
  QToolBar *editToolBar;
  QAction *newAct;
  QAction *openAct;
  QAction *saveAct;
  QAction *saveAsAct;
  QAction *exitAct;
  QAction *cutAct;
  QAction *copyAct;
  QAction *pasteAct;
  QAction *highlightAct;
  QAction *aboutAct;
  QAction *aboutQtAct;
  QAction *tabCloseAction;

  QToolButton *tabCloseButton;

  Highlighter *highlighter;
};

#endif
