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

#include <QTextStream>
#include <QCloseEvent>
#include <QFileDialog>

#include <assert.h>

void DevEditor::init() {
  lineNumbering = true;

  tabWidget = new QTabWidget(this);
  setCentralWidget(tabWidget);

  textEdit = new TextEditWidget(tabWidget);

  createActions();
  createMenus();
  createToolBars();
  createStatusBar();

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

void DevEditor::open() {
  QString fileName = QFileDialog::getOpenFileName(this);
  if (!fileName.isEmpty()) {
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
  QString fileName = QFileDialog::getSaveFileName(this);
  if (fileName.isEmpty())
    return false;

  return saveFile(fileName);
}

void DevEditor::about() {
  QMessageBox::about(this, tr("About DevEditor"),
    tr("<b>DevEditor</b> is an experimental programmer's editor."));
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
  setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("DevEditor")));
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

void DevEditor::textMonospace() {
  textFont = "Monospace";
  textEdit->getFont()->setFamily("Monospace");
  textEdit->updateFont();
  monospaceAct->setChecked(true);
  courierAct->setChecked(false);
  andaleAct->setChecked(false);
}

void DevEditor::textCourier() {
  textFont = "Courier New";
  textEdit->getFont()->setFamily("Courier New");
  textEdit->updateFont();
  monospaceAct->setChecked(false);
  courierAct->setChecked(true);
  andaleAct->setChecked(false);
}

void DevEditor::textAndale() {
  textFont = "Andale Mono";
  textEdit->getFont()->setFamily("Andale Mono");
  textEdit->updateFont();
  monospaceAct->setChecked(false);
  courierAct->setChecked(false);
  andaleAct->setChecked(true);
  //TODO Find out if there isn't a better way to do this.
}

void DevEditor::updatePos(int line, int col) {
  lineLabel->setText(tr("Line: %1").arg(line));
  columnLabel->setText(tr("Col: %1").arg(col));
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

  monospaceAct = new QAction(tr("Monospace"), this);
  monospaceAct->setStatusTip(tr("Sets the font in the current tab to Monospace"));
  monospaceAct->setCheckable(true);
  monospaceAct->setChecked(true);
  connect(monospaceAct, SIGNAL(triggered()), this, SLOT(textMonospace()));

  courierAct = new QAction(tr("Courier New"), this);
  courierAct->setStatusTip(tr("Sets the font in the current tab to Courier New"));
  courierAct->setCheckable(true);
  connect(courierAct, SIGNAL(triggered()), this, SLOT(textCourier()));

  andaleAct = new QAction(tr("Andale Mono"), this);
  andaleAct->setStatusTip(tr("Sets the font in the current tab to Andale Mono"));
  andaleAct->setCheckable(true);
  connect(andaleAct, SIGNAL(triggered()), this, SLOT(textAndale()));

  lineNumbersAct = new QAction(tr("Line numbers"), this);
  lineNumbersAct->setStatusTip(tr("Shows/Hides line numbers."));
  lineNumbersAct->setCheckable(true);
  lineNumbersAct->setChecked(true);
  connect(lineNumbersAct, SIGNAL(triggered()), this, SLOT(toggleLineNumbering()));

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
  QMenu *textMenu = viewMenu->addMenu(tr("&Text"));
  textMenu->addAction(textBiggerAct);
  textMenu->addAction(textSmallerAct);
  textMenu->addSeparator();
  textMenu->addAction(monospaceAct);
  textMenu->addAction(courierAct);
  textMenu->addAction(andaleAct);
  viewMenu->addSeparator();
  viewMenu->addAction(lineNumbersAct);

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

  editToolBar = addToolBar(tr("View"));
  editToolBar->addAction(textSmallerAct);
  editToolBar->addAction(textBiggerAct);
}

void DevEditor::createStatusBar() {
  lineLabel = new QLabel("Line: 0", this);
  columnLabel = new QLabel("Col: 0", this);

  QHBoxLayout *box = new QHBoxLayout;
  box->setSpacing(3);
  box->setMargin(1);
  box->addWidget(lineLabel);
  box->addWidget(columnLabel);

  QFrame *aux = new QFrame;
  aux->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
  aux->setLineWidth(2);
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
  resize(size);
  move(pos);
}

void DevEditor::writeSettings() {
  QSettings settings("ScvTech", "DevEditor");
  settings.setValue("pos", pos());
  settings.setValue("size", size());
  settings.setValue("pointSize", textSize);
  settings.setValue("fontFamily", textFont);
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
    shownName = strippedName(curFile);

  setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("DevEditor")));
  if (isWindowModified())
    tabWidget->setTabText(tabWidget->currentIndex(), tr("%1*").arg(shownName));
  else
    tabWidget->setTabText(tabWidget->currentIndex(), shownName);

  textEdit->setCurFile(curFile);
  textEdit->setShownName(shownName);
}

QString DevEditor::strippedName(const QString &fullFileName) {
  return QFileInfo(fullFileName).fileName();
}

DevEditor::~DevEditor() {
}
