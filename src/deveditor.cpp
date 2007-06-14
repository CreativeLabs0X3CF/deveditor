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


#include <QtGui>
#include "deveditor.h"
#include "texteditwidget.h"
#include "environment.h"

#include <QTextStream>
#include <QCloseEvent>
#include <QFileDialog>

#include <assert.h>

void DevEditor::init() {
  env = new Environment;

  lineNumbering = true;

  tabWidget = new QTabWidget(this);

  setCentralWidget(tabWidget);

  textEdit = new TextEditWidget(tabWidget);

  createActions();
  createMenus();
  createToolBars();
  createStatusBar();

  msgs = "";
  createDockWindows();

  tabCloseButton = new QToolButton(this);
  tabCloseButton->setDefaultAction(tabCloseAction);
  tabWidget->setCornerWidget(tabCloseButton);
  connect(tabCloseButton, SIGNAL(clicked()), this, SLOT(removeTab()));

  readSettings();

  connect(textEdit->getDocument(), SIGNAL(modificationChanged(bool)), this, SLOT(documentWasModified(bool)));
  connect(textEdit->getDocument(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));

  setCurrentFile("");

  tabWidget->addTab(textEdit, shownName);
  textEdit->getFont()->setPointSize(textSize);
  textEdit->getFont()->setFamily(textFont);
  textEdit->updateFont();
  textEdit->setLineNumbering(lineNumbering);
  connect(tabWidget->currentWidget(), SIGNAL(highlighting(bool)), this, SLOT(setSyntaxHighlightingMenuItem(bool)));
  connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(switchToTab(int)));
  connect(textEdit, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updatePos(int, int)));

  openProgFunc(progName);
}

DevEditor::DevEditor() {
  init();
}

DevEditor::DevEditor(char *_filename) {
  init();

  QFile file(_filename);
  if (file.exists())
    loadFile(QString(_filename));
  else
    qWarning("Unable to open %s", _filename);
}

void DevEditor::closeTab(int index, bool force) {
  if ((index < 0) || (index >= tabWidget->count())) {
    qWarning("Warning: Tried to close tab %d, but there are only %d tabs.\n", index, tabWidget->count());
    return;
  }

  bool ret = true;

  QWidget *aux = tabWidget->widget(index);

  if (((TextEditWidget *)tabWidget->widget(index))->getDocument()->isModified())
    ret = maybeSave(!force); // If force then can't cancel. If not force, then can cancel.

  if (!force) {
    if (!ret)
      return;
  }

  ((TextEditWidget*)tabWidget->widget(index))->getDocument()->clear();

  tabWidget->removeTab(index);

  if (!force && (tabWidget->count() == 0)) {
    newFile();
  }

  delete aux;
}

void DevEditor::closeEvent(QCloseEvent *event) {
  bool modified = false;
  for (int i = 0; i < tabWidget->count(); ++i)
    if (((TextEditWidget *)tabWidget->widget(i))->getDocument()->isModified()) {
      modified = true;
      break;
    }

  if (modified) {
    int ret = QMessageBox::warning(this, tr("DevEditor"),
      tr("There are multiple tabs open\n"
      "and some documents have been modified since last save.\n"
      "Are you sure you want to exit?"),
      QMessageBox::Yes | QMessageBox::Default,
      QMessageBox::Cancel | QMessageBox::Escape);
    if (ret == QMessageBox::Cancel) {
      event->ignore();
      return;
    }
  }

  while (tabWidget->count())
    closeTab(0, true);

  writeSettings();
  event->accept();
}

void DevEditor::newFile() {
  tabWidget->addTab(new TextEditWidget(tabWidget), "");
  switchToTab(tabWidget->count() - 1);

  textEdit->getFont()->setPointSize(textSize);
  textEdit->getFont()->setFamily(textFont);
  textEdit->updateFont();
  textEdit->setLineNumbering(lineNumbering);

  connect(tabWidget->currentWidget(), SIGNAL(highlighting(bool)), this, SLOT(setSyntaxHighlightingMenuItem(bool)));
  setCurrentFile("");
}

void DevEditor::open(QString fileName) {
  if (fileName.isEmpty())
    fileName = QFileDialog::getOpenFileName(this, "DevEditor", progName);
  if (!fileName.isEmpty() && (env->isReadableFile(fileName))) {
    if (curFile != "") {
      tabWidget->addTab(new TextEditWidget(tabWidget), "");
      switchToTab(tabWidget->count() - 1);

      textEdit->getFont()->setPointSize(textSize);
      textEdit->getFont()->setFamily(textFont);
      textEdit->updateFont();
      textEdit->setLineNumbering(lineNumbering);

      connect(tabWidget->currentWidget(), SIGNAL(highlighting(bool)), this, SLOT(setSyntaxHighlightingMenuItem(bool)));
    }
    loadFile(fileName);
  }
}

bool DevEditor::save() {
  if (curFile.isEmpty())
    return saveAs();
  else
    return saveFile(curFile);
}

bool DevEditor::saveAs() {
  QString fileName = QFileDialog::getSaveFileName(this, "DevEditor", progName);
  if (fileName.isEmpty())
    return false;

  return saveFile(fileName);
}

void DevEditor::about() {
  QMessageBox::about(this, tr("About DevEditor"),
    tr("<b>DevEditor</b> is an experimental programmer's editor.\n"
    "Copyright 2007 Alexandru Scvortov <scvalex@gmail.com>"));
}

void DevEditor::documentWasModified(bool changed) {
  setWindowModified(changed);

  textEdit->getDocument()->setModified(changed); //NOTE This shouldn't do anything, but it does.

  if (changed)
    tabWidget->setTabText(tabWidget->currentIndex(), tr("%1*").arg(shownName));
  else
    tabWidget->setTabText(tabWidget->currentIndex(), tr("%1").arg(shownName));
}

void DevEditor::documentWasModified() {
  documentWasModified(true);
}

void DevEditor::toggleSyntaxHighlighting() {
  textEdit->toggleHighlighting();
}

void DevEditor::toggleLineNumbering() {
  lineNumbering = !lineNumbering;

  for (int i(0); i < tabWidget->count(); ++i)
    ((TextEditWidget*)tabWidget->widget(i))->setLineNumbering(lineNumbering);

  lineNumbersAct->setChecked(lineNumbering);
}

void DevEditor::switchToTab(int _tab) {
  if (_tab >= tabWidget->count()) {
    qWarning("Warning: Tried to switch to tab %d, but there are only %d tabs.\n", _tab, tabWidget->count());
    return;
  }

  tabWidget->setCurrentIndex(_tab);

  disconnect(cutAct, SIGNAL(triggered()), textEdit->getTextEdit(), SLOT(cut()));
  disconnect(copyAct, SIGNAL(triggered()), textEdit->getTextEdit(), SLOT(copy()));
  disconnect(pasteAct, SIGNAL(triggered()), textEdit->getTextEdit(), SLOT(paste()));
  disconnect(textEdit->getDocument(), SIGNAL(modificationChanged(bool)), this, SLOT(documentWasModified(bool)));

  textEdit = (TextEditWidget*)tabWidget->currentWidget();
  assert(textEdit != 0);

  textEdit->getFont()->setPointSize(textSize);
  textEdit->getFont()->setFamily(textFont);
  textEdit->updateFont();

  setSyntaxHighlightingMenuItem(textEdit->getHighlighting());
  connect(cutAct, SIGNAL(triggered()), textEdit->getTextEdit(), SLOT(cut()));
  connect(copyAct, SIGNAL(triggered()), textEdit->getTextEdit(), SLOT(copy()));
  connect(pasteAct, SIGNAL(triggered()), textEdit->getTextEdit(), SLOT(paste()));
  connect(textEdit->getDocument(), SIGNAL(modificationChanged(bool)), this, SLOT(documentWasModified(bool)));

  curFile = textEdit->getCurFile();
  shownName = textEdit->getShownName();

  setWindowModified(textEdit->getDocument()->isModified() || (curFile == ""));
  setWindowTitle(tr("%1 - DevEditor").arg(env->lastDir(progName)));
}

void DevEditor::removeTab() {
  closeTab(tabWidget->currentIndex());
}

void DevEditor::setSyntaxHighlightingMenuItem(bool state) {
  highlightAct->setChecked(state);
}

void DevEditor::textBigger() {
  ++textSize;
  textEdit->getFont()->setPointSize(textSize);
  textEdit->updateFont();
}

void DevEditor::textSmaller() {
  if (textSize == 1)
    return;

  --textSize;
  textEdit->getFont()->setPointSize(textSize);
  textEdit->updateFont();
}

void DevEditor::setFontFamily() {
  QAction *action = qobject_cast<QAction *>(sender());
  if (!action)
    return;

  textFont = action->data().toString();
  textEdit->getFont()->setFamily(textFont);
  textEdit->updateFont();

  for (int i(0); i < 3; ++i)
    fontFamilyAct[i]->setChecked(false);

  action->setChecked(true);
}

void DevEditor::updatePos(int line, int col) {
  lineLabel->setText(tr("Line: %1").arg(line));
  columnLabel->setText(tr("Col: %1").arg(col));
}

void DevEditor::showMsg(QString text) {
  msgs += text + "<br />";
  msgBox->setHtml(msgs);
}

void DevEditor::clearMsgs() {
  msgs = "";
}

void DevEditor::createDockWindows() {
  msgBox = new QTextBrowser(this);
  msgBox->setReadOnly(true);
  msgBox->setMaximumHeight(fontMetrics().height() * 4);

  QDockWidget *dock = new QDockWidget(tr("Messages"), this);
  dock->setAllowedAreas(Qt::BottomDockWidgetArea);
  dock->setTitleBarWidget(0);
  dock->setWidget(msgBox);

  addDockWidget(Qt::BottomDockWidgetArea, dock);

  viewMenu->addSeparator();
  viewMenu->addAction(dock->toggleViewAction());

  otherToolBar->addAction(dock->toggleViewAction());
}

void DevEditor::newProg() {
  QString fileName = QFileDialog::getSaveFileName(this, tr("New programme"), progPath, "");
  if (fileName.isEmpty())
    return;

  if (env->exists(fileName)) {
    QMessageBox::warning(this, tr("DevEditor"),
        tr("The programme %1 already exists.\n"
        "This is a problem. Solve it.").arg(fileName),
        QMessageBox::Ok | QMessageBox::Default);
    return;
  }

  env->mkdir(fileName);

  env->mkfile(fileName, "main.cpp", true);

  openProgFunc(fileName);
}

void DevEditor::openProg() {
  QString fileName = QFileDialog::getExistingDirectory(this, tr("Open programme"), progPath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (fileName.isEmpty())
    return;

  openProgFunc(fileName);
}

void DevEditor::openProgFunc(QString fileName) {
  static bool firstGo = true;

  if (!firstGo && (fileName == progName))
    return;

  firstGo = false;

  if (fileName[fileName.length() - 1] != env->separator())
    fileName += env->separator();

  progName = fileName;
  setWindowTitle(tr("%1 - DevEditor").arg(env->lastDir(progName)));

  clearMsgs();

  for (int i(0); i < recentProgs.size(); ++i)
    if ((i >= 5) || (recentProgs[i] == progName)) {
      recentProgs.removeAt(i);
    }
  recentProgs.push_front(progName);

  writeSettings();
  readSettings();

  // Now, open all readable files in the current programme.
  QStringList list = env->listViewableFiles(progName);
  for (int i(0); i < list.count(); ++i) {
    showMsg("Opening " + list[i]);
    open(list[i]);
  }
  showMsg("-----------------------");
  showMsg(QString("Total: %1 files loaded").arg(list.count()));
}

void DevEditor::openRecentProg() {
  QAction *action = qobject_cast<QAction *>(sender());
  if (action)
    openProgFunc(action->data().toString());
}

void DevEditor::createActions() {
  newAct = new QAction(QIcon(":/filenew.xpm"), tr("&New"), this);
  newAct->setShortcut(tr("Ctrl+N"));
  newAct->setStatusTip(tr("Create a new file"));
  connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

  openAct = new QAction(QIcon(":/fileopen.xpm"), tr("&Open..."), this);
  openAct->setShortcut(tr("Ctrl+O"));
  openAct->setStatusTip(tr("Open an existing file"));
  connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

  saveAct = new QAction(QIcon(":/filesave.xpm"), tr("&Save"), this);
  saveAct->setShortcut(tr("Ctrl+S"));
  saveAct->setStatusTip(tr("Save the document to disk"));
  connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

  saveAsAct = new QAction(tr("Save &As..."), this);
  saveAsAct->setStatusTip(tr("Save the document under a new name"));
  connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

  exitAct = new QAction(tr("E&xit"), this);
  exitAct->setShortcut(tr("Ctrl+Q"));
  exitAct->setStatusTip(tr("Exit the application"));
  connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

  cutAct = new QAction(QIcon(":/editcut.xpm"), tr("Cu&t"), this);
  cutAct->setShortcut(tr("Ctrl+X"));
  cutAct->setStatusTip(tr("Cut the current selection's contents to the "
    "clipboard"));
  connect(cutAct, SIGNAL(triggered()), textEdit->getTextEdit(), SLOT(cut()));

  copyAct = new QAction(QIcon(":/editcopy.xpm"), tr("&Copy"), this);
  copyAct->setShortcut(tr("Ctrl+C"));
  copyAct->setStatusTip(tr("Copy the current selection's contents to the "
    "clipboard"));
  connect(copyAct, SIGNAL(triggered()), textEdit->getTextEdit(), SLOT(copy()));

  pasteAct = new QAction(QIcon(":/editpaste.xpm"), tr("&Paste"), this);
  pasteAct->setShortcut(tr("Ctrl+V"));
  pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
    "selection"));
  connect(pasteAct, SIGNAL(triggered()), textEdit->getTextEdit(), SLOT(paste()));

  highlightAct = new QAction(tr("Syntax &Highlighting"), this);
  highlightAct->setStatusTip(tr("Toggle syntax highlighting"));
  highlightAct->setCheckable(true);
  highlightAct->setChecked(true);
  connect(highlightAct, SIGNAL(triggered()), this, SLOT(toggleSyntaxHighlighting()));

  textBiggerAct = new QAction(QIcon(":viewmag+.xpm"), tr("Text bigger"), this);
  textBiggerAct->setStatusTip(tr("Makes the text in the current tab bigger"));
  connect(textBiggerAct, SIGNAL(triggered()), this, SLOT(textBigger()));

  textSmallerAct = new QAction(QIcon(":viewmag-.xpm"), tr("Text smaller"), this);
  textSmallerAct->setStatusTip(tr("Makes the text in the current tab smaller"));
  connect(textSmallerAct, SIGNAL(triggered()), this, SLOT(textSmaller()));

  fontFamilyAct[0] = new QAction(tr("Monospace"), this);
  fontFamilyAct[0]->setStatusTip(tr("Sets the font in the current tab to Monospace"));
  fontFamilyAct[0]->setCheckable(true);
  fontFamilyAct[0]->setChecked(true);
  fontFamilyAct[0]->setData("Monospace");
  connect(fontFamilyAct[0], SIGNAL(triggered()), this, SLOT(setFontFamily()));

  fontFamilyAct[1] = new QAction(tr("Courier New"), this);
  fontFamilyAct[1]->setStatusTip(tr("Sets the font in the current tab to Courier New"));
  fontFamilyAct[1]->setCheckable(true);
  fontFamilyAct[1]->setData("Courier New");
  connect(fontFamilyAct[1], SIGNAL(triggered()), this, SLOT(setFontFamily()));

  fontFamilyAct[2] = new QAction(tr("Andale Mono"), this);
  fontFamilyAct[2]->setStatusTip(tr("Sets the font in the current tab to Andale Mono"));
  fontFamilyAct[2]->setCheckable(true);
  fontFamilyAct[2]->setData("Andale Mono");
  connect(fontFamilyAct[2], SIGNAL(triggered()), this, SLOT(setFontFamily()));

  lineNumbersAct = new QAction(tr("Line numbers"), this);
  lineNumbersAct->setStatusTip(tr("Shows/Hides line numbers."));
  lineNumbersAct->setCheckable(true);
  lineNumbersAct->setChecked(true);
  connect(lineNumbersAct, SIGNAL(triggered()), this, SLOT(toggleLineNumbering()));

  newProgAct = new QAction(QIcon(":window_new.xpm"), tr("New"), this);
  newProgAct->setStatusTip(tr("Creates a new programme"));
  connect(newProgAct, SIGNAL(triggered()), this, SLOT(newProg()));

  openProgAct = new QAction(QIcon(":fileopen.xpm"), tr("Open"), this);
  openProgAct->setStatusTip(tr("Opens an existing programme"));
  connect(openProgAct, SIGNAL(triggered()), this, SLOT(openProg()));

  aboutAct = new QAction(tr("&About"), this);
  aboutAct->setStatusTip(tr("Show the application's About box"));
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

  aboutQtAct = new QAction(tr("About &Qt"), this);
  aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
  connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  tabCloseAction = new QAction(QIcon(":tab_remove.xpm"), tr("Close tab"), this);
  tabCloseAction->setStatusTip(tr("Close the current tab"));
  // This is added to a toolbutton in init().

  cutAct->setEnabled(false);
  copyAct->setEnabled(false);
  connect(textEdit->getTextEdit(), SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
  connect(textEdit->getTextEdit(), SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));
}

void DevEditor::createMenus() {
  deMenu = menuBar()->addMenu(tr("&DevEditor"));
  deMenu->addAction(aboutAct);
  deMenu->addAction(aboutQtAct);
  deMenu->addSeparator();
  deMenu->addAction(exitAct);

  editMenu = menuBar()->addMenu(tr("&Edit"));
  editMenu->addAction(cutAct);
  editMenu->addAction(copyAct);
  editMenu->addAction(pasteAct);

  viewMenu = menuBar()->addMenu(tr("&View"));
  viewMenu->addAction(highlightAct);
  QMenu *textMenu = viewMenu->addMenu(tr("&Text"));
  textMenu->addAction(textBiggerAct);
  textMenu->addAction(textSmallerAct);
  textMenu->addSeparator();
  for (int i(0); i < 3; ++i)
    textMenu->addAction(fontFamilyAct[i]);
  viewMenu->addSeparator();
  viewMenu->addAction(lineNumbersAct);

  progMenu = menuBar()->addMenu(tr("&Programme"));
  progMenu->addAction(newProgAct);
  progMenu->addAction(openProgAct);
  openRecentProgMenu = progMenu->addMenu(tr("Open recent"));

  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newAct);
  fileMenu->addAction(openAct);
  fileMenu->addAction(saveAct);
  fileMenu->addAction(saveAsAct);
}

void DevEditor::createToolBars() {
  fileToolBar = addToolBar(tr("File"));
  fileToolBar->addAction(newAct);
  fileToolBar->addAction(openAct);
  fileToolBar->addAction(saveAct);

  editToolBar = addToolBar(tr("Edit"));
  editToolBar->addAction(cutAct);
  editToolBar->addAction(copyAct);
  editToolBar->addAction(pasteAct);

  viewToolBar = addToolBar(tr("View"));
  viewToolBar->addAction(textSmallerAct);
  viewToolBar->addAction(textBiggerAct);

  otherToolBar = addToolBar(tr("Other"));
}

void DevEditor::createStatusBar() {
  lineLabel = new QLabel("Line: 0", this);
  columnLabel = new QLabel("Col: 0", this);

  QHBoxLayout *clbox = new QHBoxLayout;
  clbox->setSpacing(3);
  clbox->setMargin(0);
  clbox->setContentsMargins(2, 0, 2, 0);
  clbox->addWidget(lineLabel);
  clbox->addWidget(columnLabel);

  QFrame *aux = new QFrame;
  aux->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
  aux->setLineWidth(2);
  aux->setLayout(clbox);

  QHBoxLayout *box = new QHBoxLayout;
  box->setSpacing(3);
  box->setMargin(0);
  clbox->setContentsMargins(2, 0, 2, 0);
  box->addSpacing(2000);
  box->addWidget(aux);

  aux = new QFrame;
  aux->setLayout(box);

  statusBar()->addWidget(aux);

  statusBar()->showMessage(tr("Ready"));
}

void DevEditor::readSettings() {
  QSettings settings("ScvTech", "DevEditor");
  QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
  QSize size = settings.value("size", QSize(400, 400)).toSize();
  textSize = settings.value("pointSize", 12).toInt();
  textFont = settings.value("fontFamily", "Monospace").toString();
  progName = settings.value("progName", "NoProject").toString();
  progPath = settings.value("progPath", ".").toString();
  recentProgs = settings.value("recentProgs").toStringList();

  if (!env->isDir(progPath))
    progPath = env->home();

  if ((progName != "NoProject") && (!env->isDir(progName)))
    progName = "NoProject";

  openRecentProgMenu->clear();
  for (int i(0); i < recentProgs.count(); ++i)
    if (env->isDir(recentProgs[i])) {
      QAction *act = new QAction(env->lastDir(recentProgs[i]), this);
      act->setData(recentProgs[i]);
      openRecentProgMenu->addAction(act);
      connect(act, SIGNAL(triggered()), this, SLOT(openRecentProg()));
    }

  resize(size);
  move(pos);
}

void DevEditor::writeSettings() {
  QSettings settings("ScvTech", "DevEditor");
  settings.setValue("pos", pos());
  settings.setValue("size", size());
  settings.setValue("pointSize", textSize);
  settings.setValue("fontFamily", textFont);
  settings.setValue("progName", progName);
  settings.setValue("progPath", progPath);
  settings.setValue("recentProgs", recentProgs);
}

bool DevEditor::maybeSave(bool canCancel) {
  if (textEdit->getDocument()->isModified()) {
    int ret;
    if (canCancel)
      ret = QMessageBox::warning(this, tr("DevEditor"),
        tr("The document %1 has been modified.\n"
        "Do you want to save your changes?").arg(shownName),
        QMessageBox::Yes | QMessageBox::Default,
        QMessageBox::No,
        QMessageBox::Cancel | QMessageBox::Escape);
    else
      ret = QMessageBox::warning(this, tr("DevEditor"),
        tr("The document %1 has been modified.\n"
        "Do you want to save your changes?").arg(shownName),
        QMessageBox::Yes | QMessageBox::Default,
        QMessageBox::No);
    if (ret == QMessageBox::Yes)
      return save();
    else if (ret == QMessageBox::Cancel)
      return false;
  }
  return true;
}

void DevEditor::loadFile(const QString &fileName) {
  QFile file(fileName);
  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("DevEditor"),
      tr("Cannot read file %1:\n%2.")
      .arg(fileName)
      .arg(file.errorString()));
    return;
  }

  QTextStream in(&file);
  QApplication::setOverrideCursor(Qt::WaitCursor);
  textEdit->getTextEdit()->setPlainText(in.readAll());
  QApplication::restoreOverrideCursor();

  setCurrentFile(fileName);
  tabWidget->setTabText(tabWidget->currentIndex(), shownName);
  statusBar()->showMessage(tr("File loaded"), 2000);
}

bool DevEditor::saveFile(const QString &fileName) {
  QFile file(fileName);
  if (!file.open(QFile::WriteOnly | QFile::Text)) {
    QMessageBox::warning(this, tr("DevEditor"),
      tr("Cannot write file %1:\n%2.")
      .arg(fileName)
      .arg(file.errorString()));
    return false;
  }

  QTextStream out(&file);
  QApplication::setOverrideCursor(Qt::WaitCursor);
  out << textEdit->getTextEdit()->toPlainText();
  QApplication::restoreOverrideCursor();

  setCurrentFile(fileName);
  statusBar()->showMessage(tr("File saved"), 2000);
  return true;
}

void DevEditor::setCurrentFile(const QString &fileName) {
  curFile = fileName;
  textEdit->getDocument()->setModified(false);
  setWindowModified(false);

  if (curFile.isEmpty())
    shownName = "untitled.txt";
  else
    shownName = env->strippedName(curFile);

  setWindowTitle(tr("%1 - DevEditor").arg(env->lastDir(progName)));
  if (isWindowModified())
    tabWidget->setTabText(tabWidget->currentIndex(), tr("%1*").arg(shownName));
  else
    tabWidget->setTabText(tabWidget->currentIndex(), shownName);

  textEdit->setCurFile(curFile);
  textEdit->setShownName(shownName);
}

DevEditor::~DevEditor() {
  delete env;
}
