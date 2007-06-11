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

class QAction;
class QFont;
class QMenu;
class QTabWidget;
class QToolButton;
class QLabel;
class TextEditWidget;

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
  //! Opens an empty file.
  void newFile();

  /*!
    Pops up an open file dialog and opens the selected file.
    @note If the file in the current tab is empty, the new file is opened into the same tab.
  */
  void open();

  bool save();
  bool saveAs();
  void about();
  //!@param changed The new modification state.
  void documentWasModified(bool changed);
  void documentWasModified();

  //! Toggles syntax highlighting on/off for the current tab.
  void toggleSyntaxHighlighting();

  //! Toggles line numbering for all tabs.
  void toggleLineNumbering();

  //! Switches to the _tab-th tab. If it does not exist, returns.
  void switchToTab(int _tab);

  /*!
    Removes the current tab from view. Provided for convenience.
    @sa closeTab()
  */
  void removeTab();

  //! Sets the syntax highlighting menu item checkbox to checked or not checked.
  void setSyntaxHighlightingMenuItem(bool state);

  //! Makes the text in all tabs bigger.
  void textBigger();

  //! Makes the text in all tabs smaller.
  void textSmaller();

  //! Sets the font to the specifed family.
  void textMonospace();
  void textCourier();
  void textAndale();

  //! Updates the line and column labels in th status bar.
  void updatePos(int line, int col);

private:
  //! Code sequences common to all constructors.
  void init();

  void createActions();
  void createMenus();
  void createToolBars();
  void createStatusBar();

  /*!
    Saves/loads the global settings:
     * "pos": the position on the screen.
     * "size": the size of the window.
     * "pointSize": the size of the text.
     * "fontFamily": the font family of the text.
  */
  void readSettings();
  void writeSettings();

  bool maybeSave(bool canCancel = true);
  void loadFile(const QString &fileName);
  bool saveFile(const QString &fileName);
  void setCurrentFile(const QString &fileName);

  //! @return The base --- last part of the filename.
  QString strippedName(const QString &fullFileName);

  /*!
    Closes the index-th tab.
    @param index The number of the tab to closed. If smaller than 0 or greater or equal to the number of tabs, returns.
    @param force If false, then the tab may not be closed (if the user desires it). If true, by the time the function returns, the tab will be hidden and deleted.
  */
  void closeTab(int index, bool force = false);

  QTabWidget *tabWidget;

  //! The current TextEditWidget, i.e. the one in the current tab.
  TextEditWidget *textEdit;

  //! The current file.
  QString curFile;

  //! The file name that's actually shown to the user.
  QString shownName;

  /*!
    The current text size in points.
    @sa textBigger(), textSmaller()
  */
  int textSize;

  /*!
    The current text font family.
    @sa textMonospace(), textCourier(), textAndale()
  */
  QString textFont;

  //! True if the line numbers column is visible.
  bool lineNumbering;

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
  QAction *textBiggerAct;
  QAction *textSmallerAct;
  QAction *monospaceAct;
  QAction *courierAct;
  QAction *andaleAct;
  QAction *lineNumbersAct;
  QAction *aboutAct;
  QAction *aboutQtAct;
  QAction *tabCloseAction;

  QToolButton *tabCloseButton;

  //! Placed in the status bar, show the number of the line and column.
  QLabel *lineLabel, *columnLabel;
};

#endif
