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
#include "peditor.h"
#include "texteditwidget.h"
#include "environment.h"
#include "messagebox.h"
#include "newprojectwidget.h"

#include <QTextStream>
#include <QCloseEvent>
#include <QFileDialog>

#include <assert.h>

void PEditor::init() {
    setWindowIcon(QIcon(":/pi_icon.png"));

    env = new Environment(this);
    pi = new ProgInfo(".");

    lineNumbering = true;

    tabWidget = new QTabWidget(this);

    setCentralWidget(tabWidget);
    setFocusProxy(tabWidget);

//     textEdit = new TextEditWidget(tabWidget);
    textEdit = 0;

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    createDockWindows();

    connect(env, SIGNAL(canCompileChanged(bool)), this, SLOT(canCompileChanged(bool))); // This should be placed after createActions();
    if (env->getCanCompile())
        enableCLActs();
    else
        disableCLActs();

    tabCloseButton = new QToolButton(this);
    tabCloseButton->setDefaultAction(tabCloseAction);
    tabWidget->setCornerWidget(tabCloseButton);
    connect(tabCloseButton, SIGNAL(clicked()), this, SLOT(removeTab()));

    readSettings();

//     connect(textEdit->getDocument(), SIGNAL(modificationChanged(bool)), this, SLOT(documentWasModified(bool)));
//     connect(textEdit->getDocument(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));

//     setCurrentFile("");
//
//     tabWidget->addTab(textEdit, shownName);
//     textEdit->getFont()->setPointSize(textSize);
//     textEdit->getFont()->setFamily(textFont);
//     textEdit->updateFont();
//     textEdit->setLineNumbering(lineNumbering);
//     connect(tabWidget->currentWidget(), SIGNAL(highlighting(bool)), this, SLOT(setSyntaxHighlightingMenuItem(bool)));
//     connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(switchToTab(int)));
//     connect(textEdit, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updatePos(int, int)));

    openProgFunc(progName);
}

PEditor::PEditor() {
    init();
}

PEditor::~PEditor() {
    delete pi;
}

bool PEditor::switchToFile(const QString &fileName) {
//   qWarning("Switching to %s", fileName.toStdString().c_str());

    int i(0);
    while ((i < tabWidget->count()) && (((TextEditWidget*)tabWidget->widget(i))->getShownName() != fileName))
        ++i;

    if (i == tabWidget->count())
        return false;

    switchToTab(i);

    return true;
}

void PEditor::closeTab(int index, bool force) {
    if ((index < 0) || (index >= tabWidget->count())) {
        qWarning("Warning: Tried to close tab %d, but there are only %d tabs.\n", index, tabWidget->count());
        return;
    }

    bool ret = true;

//     QWidget *aux = tabWidget->widget(index);

//     if (((TextEditWidget *)tabWidget->widget(index))->getDocument()->isModified())
//         ret = maybeSave(!force); // If force then can't cancel. If not force, then can cancel.

    if (!force) {
        if (!ret)
            return;
    }

    ((TextEditWidget*)tabWidget->widget(index))->getDocument()->clear();

    tabWidget->removeTab(index);

    if (tabWidget->count() == 0) {
        textEdit = 0;

        disableFSOActs();
    }

//     if (!force && (tabWidget->count() == 0)) {
//         newFile();
//     }

//     delete aux;
}

void PEditor::closeEvent(QCloseEvent *event) {
    int ret = QMessageBox::warning(this, tr("PEditor"),
                                   tr("Are you sure you want to quit?"),
                                   QMessageBox::Yes | QMessageBox::Default,
                                   QMessageBox::No | QMessageBox::Escape);

    if (ret == QMessageBox::No) {
        event->ignore();
        return;
    }

    saveAll();

    while (tabWidget->count())
        closeTab(0, true);

    writeSettings();
    event->accept();
}

void PEditor::newFile() {
    tabWidget->addTab(new TextEditWidget(tabWidget), "");
    switchToTab(tabWidget->count() - 1);

    textEdit->getFont()->setPointSize(textSize);
    textEdit->getFont()->setFamily(textFont);
    textEdit->updateFont();
    textEdit->setLineNumbering(lineNumbering);

    connect(tabWidget->currentWidget(), SIGNAL(highlighting(bool)), this, SLOT(setSyntaxHighlightingMenuItem(bool)));
    setCurrentFile("");
}

void PEditor::open(QString fileName) {
    if (fileName.isEmpty())
        fileName = QFileDialog::getOpenFileName(this, "PEditor", progName);

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

void PEditor::saveAll() {
    for (int i(0); i < tabWidget->count(); ++i)
        if (((TextEditWidget*)tabWidget->widget(i))->getDocument()->isModified()) {
            save(i);
            tabWidget->setTabText(i, tr("%1").arg(((TextEditWidget*)tabWidget->widget(i))->getShownName()));
        }
}

bool PEditor::save(int tab) {
    if (tab == -1)
        tab = tabWidget->currentIndex();

    if ((tab < 0) || (tab >= tabWidget->count()))
        return false;

    if (((TextEditWidget*)tabWidget->widget(tab))->getCurFile().isEmpty())
        return saveAs(tab);
    else
        return saveFile(curFile, tab);
}

bool PEditor::saveAs(int tab) {
    if (tab == -1)
        tab = tabWidget->currentIndex();

    if ((tab < 0) || (tab >= tabWidget->count()))
        return false;

    QString fileName = QFileDialog::getSaveFileName(this, "PEditor", progName);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName, tab);
}

void PEditor::about() {
    QMessageBox::about(this, tr("About PEditor"),
                       tr("<b>PEditor</b> is an experimental programmer's editor.<br />"
                          "Copyright 2007 Alexandru Scvortov <scvalex@gmail.com>"));
}

void PEditor::documentWasModified(bool changed) {
    setWindowModified(changed);

    textEdit->getDocument()->setModified(changed); //NOTE This shouldn't do anything, but it does.

    if (changed)
        tabWidget->setTabText(tabWidget->currentIndex(), tr("%1*").arg(shownName));
    else
        tabWidget->setTabText(tabWidget->currentIndex(), tr("%1").arg(shownName));
}

void PEditor::documentWasModified() {
    documentWasModified(true);
}

void PEditor::toggleSyntaxHighlighting() {
    if (textEdit)
        textEdit->toggleHighlighting();
}

void PEditor::toggleLineNumbering() {
    lineNumbering = !lineNumbering;

    for (int i(0); i < tabWidget->count(); ++i)
        ((TextEditWidget*)tabWidget->widget(i))->setLineNumbering(lineNumbering);

    lineNumbersAct->setChecked(lineNumbering);
}

void PEditor::switchToTab(int _tab) {
    if (_tab >= tabWidget->count()) {
        qWarning("Warning: Tried to switch to tab %d, but there are only %d tabs.\n", _tab, tabWidget->count());
        return;
    }

    tabWidget->setCurrentIndex(_tab);

    if (textEdit) {
        disconnect(cutAct, SIGNAL(triggered()), textEdit->getTextEdit(), SLOT(cut()));
        disconnect(copyAct, SIGNAL(triggered()), textEdit->getTextEdit(), SLOT(copy()));
        disconnect(pasteAct, SIGNAL(triggered()), textEdit->getTextEdit(), SLOT(paste()));
        disconnect(textEdit->getDocument(), SIGNAL(modificationChanged(bool)), this, SLOT(documentWasModified(bool)));
        disconnect(textEdit, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updatePos(int, int)));
        disconnect(mb, SIGNAL(moveToLine(int)), textEdit, SLOT(moveToLine(int)));
    }

    textEdit = (TextEditWidget*)tabWidget->currentWidget();
    assert(textEdit != 0);
    enableFSOActs();

    tabWidget->setFocusProxy(textEdit);
    textEdit->getFont()->setPointSize(textSize);
    textEdit->getFont()->setFamily(textFont);
    textEdit->updateFont();

    setSyntaxHighlightingMenuItem(textEdit->getHighlighting());
    connect(mb, SIGNAL(moveToLine(int)), textEdit, SLOT(moveToLine(int)));
    connect(cutAct, SIGNAL(triggered()), textEdit->getTextEdit(), SLOT(cut()));
    connect(copyAct, SIGNAL(triggered()), textEdit->getTextEdit(), SLOT(copy()));
    connect(pasteAct, SIGNAL(triggered()), textEdit->getTextEdit(), SLOT(paste()));
    connect(textEdit->getDocument(), SIGNAL(modificationChanged(bool)), this, SLOT(documentWasModified(bool)));
    connect(textEdit, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updatePos(int, int)));

    curFile = textEdit->getCurFile();
    shownName = textEdit->getShownName();

    setWindowModified(textEdit->getDocument()->isModified() || (curFile == ""));
    setWindowTitle(tr("%1 - PEditor").arg(env->lastDir(progName)));
}

void PEditor::removeTab() {
    closeTab(tabWidget->currentIndex());
}

void PEditor::setSyntaxHighlightingMenuItem(bool state) {
    highlightAct->setChecked(state);
}

void PEditor::textBigger() {
    if (!textEdit)
        return;

    ++textSize;

    textEdit->getFont()->setPointSize(textSize);
    textEdit->updateFont();
}

void PEditor::textSmaller() {
    if (!textEdit)
        return;

    if (textSize == 1)
        return;

    --textSize;
    textEdit->getFont()->setPointSize(textSize);
    textEdit->updateFont();
}

void PEditor::setFontFamily() {
    QAction *action = qobject_cast<QAction *>(sender());
    if (!action)
        return;

    if (!textEdit)
        return;

    textFont = action->data().toString();
    textEdit->getFont()->setFamily(textFont);
    textEdit->updateFont();

    for (int i(0); i < 3; ++i)
        fontFamilyAct[i]->setChecked(false);

    action->setChecked(true);
}

void PEditor::updatePos(int line, int col) {
    lineLabel->setText(tr("Line: %1").arg(line));
    columnLabel->setText(tr("Col: %1").arg(col));
}

void PEditor::createDockWindows() {
    mb = new MessageBox(this);
    mb->setFocusProxy(tabWidget);

    env->setMessageBox(mb);

    mb->setMaximumHeight(fontMetrics().height() * 6);

    messageDock = new QDockWidget(tr("Messages"), this);
    messageDock->setAllowedAreas(Qt::BottomDockWidgetArea);
    messageDock->setTitleBarWidget(0);
    messageDock->setWidget(mb);
    messageDock->hide();

    addDockWidget(Qt::BottomDockWidgetArea, messageDock);

    viewMenu->addSeparator();
    viewMenu->addAction(messageDock->toggleViewAction());

    otherToolBar->addAction(messageDock->toggleViewAction());

    connect(mb, SIGNAL(switchToFile(const QString&)), this, SLOT(switchToFile(const QString&)));
    if (textEdit)
        connect(mb, SIGNAL(moveToLine(int)), textEdit, SLOT(moveToLine(int)));
}

void PEditor::newProg() {
    static NewProjectWidget *npw = new NewProjectWidget(this);
    static bool justCreated = true;

    if (justCreated) {
        connect(npw, SIGNAL(createProgramme(const QString &, const QString &)), this, SLOT(createProgramme(const QString &, const QString &)));
        justCreated = false;
    }
    npw->show();
}

void PEditor::createProgramme(const QString &dir, const QString &templ) {
    env->mkdir(dir);

    env->dupDir(templ, dir);

    openProgFunc(dir);
}

void PEditor::openProg() {
    QSettings settings("ScvTech", "PEditor");
    QString progDir = settings.value("progDir", ".").toString();
    progDir = QDir(progDir).absolutePath() + "/";

    QString fileName = QFileDialog::getExistingDirectory(this, tr("Open programme"), progDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (fileName.isEmpty())
        return;

    openProgFunc(fileName);
}

void PEditor::openProgFunc(QString fileName) {
    static bool firstGo = true;

    if (!firstGo && (fileName == progName))
        return;

    firstGo = false;

    if (fileName[fileName.length() - 1] != '/')
        fileName += '/';

    progName = fileName;
    setWindowTitle(tr("%1 - PEditor").arg(env->lastDir(progName)));

    enableCLActs();

    pi->setProg(progName);

    mb->reset();

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
        mb->message("Opening " + list[i]);
        open(list[i]);
    }
    mb->message("-----------------------");
    mb->message(QString("Total: %1 files loaded").arg(list.count()));
}

void PEditor::openRecentProg() {
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        openProgFunc(action->data().toString());
}

void PEditor::compileCurrentFile() {
    save();

    disableCLActs();

    messageDock->show();

    connect(env, SIGNAL(compileSuccesful()), this, SLOT(compileSuccesful()));
    env->compileFile(curFile);
}

void PEditor::compileNext() {
    if (compileQueue.isEmpty())
        return;

    if (compileQueue.count() == 1) {
        disconnect(env, SIGNAL(compileSuccesful()), this, SLOT(compileNext()));
        connect(env, SIGNAL(compileSuccesful()), this, SLOT(compileSuccesful()));
    }

    QString src = compileQueue[0];
    compileQueue.pop_front();
    env->compileFile(src, true);
}

void PEditor::compileFailed() {
    compileQueue.clear();

    mb->error(tr("*** Failed ***"));
    disconnect(env, SIGNAL(compileSuccesful()), this, SLOT(compileSuccesful()));
    disconnect(env, SIGNAL(compileFailed()), this, SLOT(compileFailed()));
    disconnect(env, SIGNAL(compileSuccesful()), this, SLOT(compileNext()));

    enableCLActs();
}

void PEditor::compileSuccesful() {
    compileQueue.clear();

    mb->good(tr("*** Succes ***"));
    disconnect(env, SIGNAL(compileFailed()), this, SLOT(compileFailed()));
    disconnect(env, SIGNAL(compileSuccesful()), this, SLOT(compileSuccesful()));

    emit compilationSuccesful();

    enableCLActs();
}

void PEditor::compileAll() {
    saveAll();

    disableCLActs();

    mb->reset();
    messageDock->show();

    QFileInfo info = QFileInfo(progName);
    QDir::setCurrent(info.absolutePath());

    mb->message("------ Compiling ------");

    compileQueue = pi->sourceFiles();

    connect(env, SIGNAL(compileSuccesful()), this, SLOT(compileNext()));
    connect(env, SIGNAL(compileFailed()), this, SLOT(compileFailed()));
    compileNext();
}

void PEditor::linkSuccesful() {
    disconnect(env, SIGNAL(linkSuccesful()), this, SLOT(linkSuccesful()));
    disconnect(env, SIGNAL(linkFailed()), this, SLOT(linkFailed()));

    mb->good(tr("*** Success ***"));

    enableCLActs();
}

void PEditor::linkFailed() {
    disconnect(env, SIGNAL(linkSuccesful()), this, SLOT(linkSuccesful()));
    disconnect(env, SIGNAL(linkFailed()), this, SLOT(linkFailed()));

    mb->error(tr("*** Problems ***"));

    enableCLActs();
}

void PEditor::linkObjects(bool dontClear) {
    disableCLActs();

    messageDock->show();

    connect(env, SIGNAL(linkSuccesful()), this, SLOT(linkSuccesful()));
    connect(env, SIGNAL(linkFailed()), this, SLOT(linkFailed()));
    env->linkObjects(progName, dontClear);
}

void PEditor::runProg() {
    messageDock->show();
    mb->reset();

    mb->message(tr("Attempting to run %1").arg(pi->exe()));

    connect(env, SIGNAL(runDone(int, QProcess::ExitStatus)), this, SLOT(runDone(int, QProcess::ExitStatus)));
    env->run(pi->exe());
}

void PEditor::runDone(int exitCode, QProcess::ExitStatus exitStatus) {
    Q_UNUSED(exitCode);

    disconnect(env, SIGNAL(runDone(int, QProcess::ExitStatus)), this, SLOT(runDone(int, QProcess::ExitStatus)));

    if (exitStatus != QProcess::NormalExit) {
        mb->error(tr("*** Failed ***"));

        return;
    }

    mb->good(tr("*** Done ***"));
}

void PEditor::buildProg() {
    compileAll();

    connect(this, SIGNAL(compilationSuccesful()), this, SLOT(continueBuild()));
}

void PEditor::continueBuild() {
    disconnect(this, SIGNAL(compilationSuccesful()), this, SLOT(continueBuild()));

    linkObjects(true);
}

void PEditor::disableCLActs() {
    compileFileAct->setDisabled(true);
    buildAct->setDisabled(true);
    compileAct->setDisabled(true);
    linkAct->setDisabled(true);
}

void PEditor::enableCLActs() {
    compileFileAct->setEnabled(true);
    buildAct->setEnabled(true);
    compileAct->setEnabled(true);
    linkAct->setEnabled(true);
}

void PEditor::canCompileChanged(bool newState) {
    if (newState)
        enableCLActs();
    else
        disableCLActs();
}

void PEditor::closeProg() {
    mb->message("Closing programme");

    QStringList list = env->listViewableFiles(progName);
    int i(0);
    while (i < tabWidget->count()) {
        if (list.contains(((TextEditWidget*)tabWidget->widget(i))->getCurFile()))
            closeTab(i--);
        ++i;
    }

    progName = "NoProject";
    setWindowTitle(tr("%1 - PEditor").arg(env->lastDir(progName)));
}

void PEditor::createActions() {
    compileFileAct = new QAction(tr("Compile"), this);
    compileFileAct->setStatusTip(tr("Compiles the current file"));
    connect(compileFileAct, SIGNAL(triggered()), this, SLOT(compileCurrentFile()));

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

    saveAllAct = new QAction(tr("Save Al&l"), this);
    saveAllAct->setStatusTip(tr("Save all open documents"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAll()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    cutAct = new QAction(QIcon(":/editcut.xpm"), tr("Cu&t"), this);
    cutAct->setShortcut(tr("Ctrl+X"));
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    if (textEdit)
        connect(cutAct, SIGNAL(triggered()), textEdit->getTextEdit(), SLOT(cut()));

    copyAct = new QAction(QIcon(":/editcopy.xpm"), tr("&Copy"), this);
    copyAct->setShortcut(tr("Ctrl+C"));
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    if (textEdit)
        connect(copyAct, SIGNAL(triggered()), textEdit->getTextEdit(), SLOT(copy()));

    pasteAct = new QAction(QIcon(":/editpaste.xpm"), tr("&Paste"), this);
    pasteAct->setShortcut(tr("Ctrl+V"));
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    if (textEdit)
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

    runAct = new QAction(QIcon(":/run.xpm"), tr("&Run"), this);
    runAct->setShortcut(tr("F9"));
    runAct->setStatusTip(tr("Runs the programme"));
    connect(runAct, SIGNAL(triggered()), this, SLOT(runProg()));

    buildAct = new QAction(QIcon(":/configure.xpm"), tr("&Build"), this);
    buildAct->setShortcut(tr("F8"));
    buildAct->setStatusTip(tr("Builds the programme"));
    connect(buildAct, SIGNAL(triggered()), this, SLOT(buildProg()));

    compileAct = new QAction(tr("&Compile"), this);
    compileAct->setStatusTip(tr("Compiles all source files"));
    connect(compileAct, SIGNAL(triggered()), this, SLOT(compileAll()));

    linkAct = new QAction(tr("&Link"), this);
    linkAct->setStatusTip(tr("Links the objects into an executable"));
    connect(linkAct, SIGNAL(triggered()), this, SLOT(linkObjects()));

    newProgAct = new QAction(QIcon(":window_new.xpm"), tr("&New"), this);
    newProgAct->setStatusTip(tr("Creates a new programme"));
    connect(newProgAct, SIGNAL(triggered()), this, SLOT(newProg()));

    openProgAct = new QAction(QIcon(":fileopen.xpm"), tr("&Open"), this);
    openProgAct->setStatusTip(tr("Opens an existing programme"));
    connect(openProgAct, SIGNAL(triggered()), this, SLOT(openProg()));

    closeProgAct = new QAction(tr("&Close"), this);
    closeProgAct->setStatusTip(tr("Close the current programme"));
    connect(closeProgAct, SIGNAL(triggered()), this, SLOT(closeProg()));

    aboutAct = new QAction(QIcon(":/pi_icon.png"), tr("&About"), this);
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
    if (textEdit) {
        connect(textEdit->getTextEdit(), SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
        connect(textEdit->getTextEdit(), SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));
    }

    if (!textEdit)
        disableFSOActs();
}

void PEditor::createMenus() {
    deMenu = menuBar()->addMenu(tr("&PEditor"));
    deMenu->addAction(env->getConfigureAct());
    deMenu->addSeparator();
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
    progMenu->addAction(runAct);
    progMenu->addAction(buildAct);
    progMenu->addSeparator();
    progMenu->addAction(compileAct);
    progMenu->addAction(linkAct);
    progMenu->addSeparator();
    progMenu->addAction(newProgAct);
    progMenu->addAction(openProgAct);
    openRecentProgMenu = progMenu->addMenu(tr("Open recent"));
    progMenu->addAction(closeProgAct);

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(saveAllAct);
    fileMenu->addSeparator();
    fileMenu->addAction(compileFileAct);
}

void PEditor::createToolBars() {
    QToolBar *toolBar;

    toolBar = addToolBar(tr("File"));
    toolBar->addAction(newAct);
    toolBar->addAction(openAct);
    toolBar->addAction(saveAct);

    toolBar = addToolBar(tr("Edit"));
    toolBar->addAction(cutAct);
    toolBar->addAction(copyAct);
    toolBar->addAction(pasteAct);

    toolBar = addToolBar(tr("View"));
    toolBar->addAction(textSmallerAct);
    toolBar->addAction(textBiggerAct);

    toolBar = addToolBar(tr("Programme"));
    toolBar->addAction(runAct);
    toolBar->addAction(buildAct);

    otherToolBar = addToolBar(tr("Other"));
}

void PEditor::createStatusBar() {
    QFrame *aux = new QFrame;
    aux->setFrameStyle(QFrame::StyledPanel);
    aux->setLineWidth(2);

    lineLabel = new QLabel("Line: 0", aux);
    columnLabel = new QLabel("Col: 0", aux);

    QHBoxLayout *clbox = new QHBoxLayout;
    clbox->setSpacing(3);
    clbox->setMargin(0);
    clbox->setContentsMargins(2, 0, 2, 0);
    clbox->addWidget(lineLabel);
    clbox->addWidget(columnLabel);

    aux->setLayout(clbox);

//   QHBoxLayout *box = new QHBoxLayout;
//   box->setSpacing(3);
//   box->setMargin(0);
//   clbox->setContentsMargins(2, 0, 2, 0);
//   box->addSpacing(2000);
//   box->addWidget(aux);
//
//   aux = new QFrame;
//   aux->setLayout(box);

//   esb = new ExtendedStatusBar(this);
//   setStatusBar(0);
//   setStatusBar(esb);

    statusBar()->addPermanentWidget(aux);

    statusBar()->showMessage(tr("Ready"));
}

void PEditor::readSettings() {
    QSettings settings("ScvTech", "PEditor");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    textSize = settings.value("pointSize", 12).toInt();
    textFont = settings.value("fontFamily", "Monospace").toString();
    progName = settings.value("progName", "NoProject").toString();
    recentProgs = settings.value("recentProgs").toStringList();

    if ((progName != "NoProject") && (!env->isDir(progName))) {
        progName = "NoProject";
        disableCLActs();
    }

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

void PEditor::writeSettings() {
    QSettings settings("ScvTech", "PEditor");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("pointSize", textSize);
    settings.setValue("fontFamily", textFont);
    settings.setValue("progName", progName);
    settings.setValue("recentProgs", recentProgs);
}

bool PEditor::maybeSave(bool canCancel) {
    if (!textEdit)
        return false;

    if (textEdit->getDocument()->isModified()) {
        int ret;
        if (canCancel)
            ret = QMessageBox::warning(this, tr("PEditor"),
                                       tr("The document %1 has been modified.\n"
                                          "Do you want to save your changes?").arg(shownName),
                                       QMessageBox::Yes | QMessageBox::Default,
                                       QMessageBox::No,
                                       QMessageBox::Cancel | QMessageBox::Escape);
        else
            ret = QMessageBox::warning(this, tr("PEditor"),
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

void PEditor::loadFile(const QString &fileName) {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (!textEdit) {
        tabWidget->addTab(new TextEditWidget(tabWidget), "");
        switchToTab(tabWidget->count() - 1);
    }

    textEdit->setCurFile(fileName);
    textEdit->load();

    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    tabWidget->setTabText(tabWidget->currentIndex(), shownName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool PEditor::saveFile(const QString &fileName, int tab) {
    if (tab == -1)
        tab = tabWidget->currentIndex();

    if ((tab < 0) || (tab >= tabWidget->count()))
        return false;

    QApplication::setOverrideCursor(Qt::WaitCursor);

    ((TextEditWidget*)tabWidget->widget(tab))->setCurFile(fileName);
    bool ret = ((TextEditWidget*)tabWidget->widget(tab))->save();

    QApplication::restoreOverrideCursor();

    if (!ret)
        return false;

    if (tab == tabWidget->currentIndex())
        setCurrentFile(fileName);

    statusBar()->showMessage(tr("File saved"), 2000);

    return true;
}

void PEditor::setCurrentFile(const QString &fileName) {
    curFile = fileName;

    if (textEdit)
        textEdit->getDocument()->setModified(false);
    setWindowModified(false);

    if (curFile.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = env->strippedName(curFile);

    if (!textEdit) {
        textEdit = new TextEditWidget(tabWidget);

        tabWidget->addTab(textEdit, shownName);
    }

    setWindowTitle(tr("%1 - PEditor").arg(env->lastDir(progName)));
    if (isWindowModified())
        tabWidget->setTabText(tabWidget->currentIndex(), tr("%1*").arg(shownName));
    else
        tabWidget->setTabText(tabWidget->currentIndex(), shownName);

    textEdit->setCurFile(curFile);
    textEdit->setShownName(shownName);
}

void PEditor::disableFSOActs() {
    openAct->setDisabled(true);
    saveAct->setDisabled(true);
    saveAsAct->setDisabled(true);
    saveAllAct->setDisabled(true);
}

void PEditor::enableFSOActs() {
    openAct->setEnabled(true);
    saveAct->setEnabled(true);
    saveAsAct->setEnabled(true);
    saveAllAct->setEnabled(true);
}
