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


#ifndef PEDITOR_H
#define PEDITOR_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QProcess>

class QAction;
class QFont;
class QMenu;
class QTabWidget;
class QToolButton;
class QLabel;
class TextEditWidget;
class QTextBrowser;
class QStringList;
class Environment;
class MessageBox;
class ProgInfo;

/*!
  Main window
  @author Alexandru Scvortov <scvalex@gmail.com>
*/
class PEditor : public QMainWindow {
    Q_OBJECT

public:
    PEditor();
    ~PEditor();

public slots:
    /*!
      Attempts to switch to tab, containing the file fileName.
      @return True, if switch was succesful.
      @return False, if the tab was not found, or something else prevented the switch from taking place.
    */
    bool switchToFile(const QString &fileName);

signals:
    //! Emitted when a bulk compile job has succeded.
    void compilationSuccesful();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    //! Opens an empty file.
    void newFile();

    /*!
      If fileName is empty, pops up an open file dialog and opens the selected file, otherwise, just opens the specified file.
      @note If the file in the current tab is empty, the new file is opened into the same tab.
    */
    void open(QString fileName = "");

    void saveAll();
    //! Saves the current file. If no name is specified, pops a save file dialog. If tab != -1, saves the file in tab tab.
    bool save(int tab = -1);
    //! @sa save()
    bool saveAs(int tab = -1);
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
    void setFontFamily();

    //! Updates the line and column labels in th status bar.
    void updatePos(int line, int col);

    //! Called when Programme|New is pressed.
    void newProg();

    //! Creates a new programme.
    void createProgramme(const QString &dir, const QString &templ);

    //! Opens a programme.
    void openProg();

    //! Opens a recent programme.
    void openRecentProg();

    //! Closes the current programme's tabs and changes the title bar to NoProject.
    void closeProg();

    //! Compiles the current file.
    void compileCurrentFile();

    //! Compiles the next file in the compile queue.
    void compileNext();

    //! Compiles all source files.
    void compileAll();

    //! Is called when compile has failed.
    void compileFailed();

    //! Is called when compile has succeded.
    void compileSuccesful();

    //! Links the objects into an executable.
    void linkObjects(bool dontClear = false);

    //! Is called when link has failed.
    void linkFailed();

    //! Is called when link has succeded.
    void linkSuccesful();

    //! If compile was succesful, continues the build process.
    void continueBuild();

    //! Attempts to run the current programme.
    void runProg();

    //! Is called when the programme has finished running.
    void runDone(int, QProcess::ExitStatus);

    //! Builds --- compiles and links --- the programme.
    void buildProg();

    //! Disables the compile/link acts.
    void disableCLActs();

    //! Enables the compile/link acts.
    void enableCLActs();

    //! Triggered when the status (functional/non-functional) of the compiler changes.
    void canCompileChanged(bool);

private:
    //! Code sequences common to all constructors.
    void init();

    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();

    /*!
      Saves/loads the global settings:
       * "pos": the position on the screen.
       * "size": the size of the window.
       * "pointSize": the size of the text.
       * "fontFamily": the font family of the text.
       * and a few more
    */
    void readSettings();
    void writeSettings();

    bool maybeSave(bool canCancel = true);
    void loadFile(const QString &fileName);

    //! Saves the current file. If tab != -1, saves the file in tab tab.
    bool saveFile(const QString &fileName, int tab = -1);
    void setCurrentFile(const QString &fileName);

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

    //! The path of the current programme.
    QString progName;

    //! The list of recent programmes.
    QStringList recentProgs;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *progMenu;
    QMenu *deMenu;
    QToolBar *otherToolBar;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *highlightAct;
    QAction *textBiggerAct;
    QAction *textSmallerAct;
    QAction *lineNumbersAct;
    QAction *fontFamilyAct[3];
    QAction *newProgAct;
    QAction *openProgAct;
    QMenu *openRecentProgMenu;
    QAction *closeProgAct;
    QAction *tabCloseAction;
    QAction *runAct;
    QAction *buildAct;
    QAction *compileFileAct;
    QAction *compileAct;
    QAction *linkAct;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *saveAllAct;

    QToolButton *tabCloseButton;

    //! Placed in the status bar, show the number of the line and column.
    QLabel *lineLabel, *columnLabel;

    //! The environment.
    Environment *env;

    //! The lower section message box.
    MessageBox *mb;

    //! The dock that contains the MessageBox.
    QDockWidget *messageDock;

    //! @internal This is the function that actually opens the programme.
    void openProgFunc(QString fileName);

    //! @internal Used to store info about the programme.
    ProgInfo *pi;

    //! @internal The files that will be compiled.
    QStringList compileQueue;

    //! @internal Disables the File|{Open, Save, Save As, Save All} actions.
    void disableFSOActs();

    //! @internal Enables the File|{Open, Save, Save As, Save All} actions.
    void enableFSOActs();
};

#endif
