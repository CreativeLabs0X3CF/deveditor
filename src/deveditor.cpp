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
#include "highlighter.h"

#include <QTextStream>
#include <QCloseEvent>
#include <QFileDialog>

#include <assert.h>

TextEditWidget::TextEditWidget(QWidget *parent) : QTextEdit(parent), curFile(""), shownName("") {
  QFont defaultFont;
  defaultFont.setFamily("Monospace");
  defaultFont.setFixedPitch(true);
  defaultFont.setPointSize(12);

  setFont(defaultFont);
}

TextEditWidget::~TextEditWidget() {
}

QString TextEditWidget::getCurFile() const {
  return curFile;
}

QString TextEditWidget::getShownName() const {
  return shownName;
}

void TextEditWidget::setCurFile(QString _curFile) {
  curFile = _curFile;
}

void TextEditWidget::setShownName(QString _shownName) {
  shownName = _shownName;
}

void DevEditor::init() {
  tabWidget = new QTabWidget();
  setCentralWidget(tabWidget);

  textEdit = new TextEditWidget;

  createActions();
  createMenus();
  createToolBars();
  createStatusBar();

  tabCloseButton = new QToolButton();
  tabCloseButton->setDefaultAction(tabCloseAction);
//   tabCloseButton->setPixmap(QPixmap(":tab_remove.xpm"));
  tabWidget->setCornerWidget(tabCloseButton);
  connect(tabCloseButton, SIGNAL(clicked()), this, SLOT(removeTab()));

  readSettings();

  connect(textEdit->document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));

  highlighter = new Highlighter(textEdit->document());

  setCurrentFile("");

  tabWidget->addTab(textEdit, shownName);
  connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(switchToTab(int)));
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

void DevEditor::closeEvent(QCloseEvent *event) {
  if (maybeSave()) {
    writeSettings();
    event->accept();
  } else {
    event->ignore();
  }
}

void DevEditor::newFile() {
  tabWidget->addTab(new TextEditWidget, "");
  switchToTab(tabWidget->count() - 1);
  setCurrentFile("");
}

void DevEditor::open() {
  QString fileName = QFileDialog::getOpenFileName(this);
  if (!fileName.isEmpty()) {
    if (curFile != "") {
      tabWidget->addTab(new TextEditWidget, "");
      switchToTab(tabWidget->count() - 1);
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
  QString fileName = QFileDialog::getSaveFileName(this);
  if (fileName.isEmpty())
    return false;

  return saveFile(fileName);
}

void DevEditor::about() {
  QMessageBox::about(this, tr("About DevEditor"),
    tr("<b>DevEditor</b> is an experimental programmer's editor."));
}

void DevEditor::documentWasModified() {
  setWindowModified(true);
}

void DevEditor::toggleSyntaxHighlighting() {
  if (highlighter->document())
    highlighter->setDocument(0);
  else
    highlighter->setDocument(textEdit->document());
}

void DevEditor::switchToTab(int _tab) {
  if (_tab >= tabWidget->count()) {
    qWarning("Warning: Tried to switch to tab %d, but there are only %d tabs.\n", _tab, tabWidget->count());
    return;
  }

  tabWidget->setCurrentIndex(_tab);

  //TODO Memorize and set curFile when tab changes.

  disconnect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));
  disconnect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));
  disconnect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));
  disconnect(textEdit->document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));

  textEdit = (TextEditWidget*)tabWidget->currentWidget();
  assert(textEdit != 0);

  highlighter->setDocument(textEdit->document());
  connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));
  connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));
  connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));
  connect(textEdit->document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));

  curFile = textEdit->getCurFile();
  shownName = textEdit->getShownName();

  setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("DevEditor")));
}

void DevEditor::removeTab() {
  closeTab(tabWidget->currentIndex());
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
  connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

  copyAct = new QAction(QIcon(":/editcopy.xpm"), tr("&Copy"), this);
  copyAct->setShortcut(tr("Ctrl+C"));
  copyAct->setStatusTip(tr("Copy the current selection's contents to the "
    "clipboard"));
  connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

  pasteAct = new QAction(QIcon(":/editpaste.xpm"), tr("&Paste"), this);
  pasteAct->setShortcut(tr("Ctrl+V"));
  pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
    "selection"));
  connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

  highlightAct = new QAction(tr("Syntax &Highlighting"), this);
  highlightAct->setStatusTip(tr("Toggle syntax highlighting"));
  highlightAct->setCheckable(true);
  highlightAct->setChecked(true);
  connect(highlightAct, SIGNAL(triggered()), this, SLOT(toggleSyntaxHighlighting()));

  aboutAct = new QAction(tr("&About"), this);
  aboutAct->setStatusTip(tr("Show the application's About box"));
  connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

  aboutQtAct = new QAction(tr("About &Qt"), this);
  aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
  connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  tabCloseAction = new QAction(QIcon(":tab_remove.xpm"), tr("Close tab"), this);
  tabCloseAction->setStatusTip(tr("Close the current tab"));
  //TODO Connect to something.

  cutAct->setEnabled(false);
  copyAct->setEnabled(false);
  connect(textEdit, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
  connect(textEdit, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));
}

void DevEditor::createMenus() {
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newAct);
  fileMenu->addAction(openAct);
  fileMenu->addAction(saveAct);
  fileMenu->addAction(saveAsAct);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAct);

  editMenu = menuBar()->addMenu(tr("&Edit"));
  editMenu->addAction(cutAct);
  editMenu->addAction(copyAct);
  editMenu->addAction(pasteAct);

  viewMenu = menuBar()->addMenu(tr("&View"));
  viewMenu->addAction(highlightAct);

  menuBar()->addSeparator();

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAct);
  helpMenu->addAction(aboutQtAct);
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
}

void DevEditor::createStatusBar() {
  statusBar()->showMessage(tr("Ready"));
}

void DevEditor::readSettings() {
  QSettings settings("ScvTech", "DevEditor");
  QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
  QSize size = settings.value("size", QSize(400, 400)).toSize();
  resize(size);
  move(pos);
}

void DevEditor::writeSettings() {
  QSettings settings("ScvTech", "DevEditor");
  settings.setValue("pos", pos());
  settings.setValue("size", size());
}

bool DevEditor::maybeSave() {
  if (textEdit->document()->isModified()) {
    int ret = QMessageBox::warning(this, tr("DevEditor"),
      tr("The document %1 has been modified.\n"
      "Do you want to save your changes?").arg(shownName),
      QMessageBox::Yes | QMessageBox::Default,
      QMessageBox::No,
      QMessageBox::Cancel | QMessageBox::Escape);
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
  textEdit->setPlainText(in.readAll());
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
  out << textEdit->toPlainText();
  QApplication::restoreOverrideCursor();

  setCurrentFile(fileName);
  statusBar()->showMessage(tr("File saved"), 2000);
  return true;
}

void DevEditor::setCurrentFile(const QString &fileName) {
  curFile = fileName;
  textEdit->document()->setModified(false);
  setWindowModified(false);

  if (curFile.isEmpty())
    shownName = "untitled.txt";
  else
    shownName = strippedName(curFile);

  setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("DevEditor")));
  tabWidget->setTabText(tabWidget->currentIndex(), shownName);

  textEdit->setCurFile(curFile);
  textEdit->setShownName(shownName);
}

QString DevEditor::strippedName(const QString &fullFileName) {
  return QFileInfo(fullFileName).fileName();
}

void DevEditor::closeTab(int index, bool force) {
  if (force || maybeSave()) {
    if (index == tabWidget->currentIndex()) {
      //TODO Fix closeTab code.
    }

//     TextEditWidget *aux = (TextEditWidget*)tabWidget->widget(index);
//     disconnect(cutAct, SIGNAL(triggered()), aux, SLOT(cut()));
//     disconnect(copyAct, SIGNAL(triggered()), aux, SLOT(copy()));
//     disconnect(pasteAct, SIGNAL(triggered()), aux, SLOT(paste()));
//     disconnect(aux->document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
    tabWidget->removeTab(index);
//     delete aux;
  }

  if (!force && (tabWidget->count() == 0)) {
    newFile();
  }
}

DevEditor::~DevEditor() {
  while (tabWidget->count()) {
//     qWarning("%d\n", tabWidget->count());
    closeTab(0, true);
  }

  delete tabWidget;
}
