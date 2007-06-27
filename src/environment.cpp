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
#include "messagebox.h"

ProgInfo::ProgInfo(const QString &_progName) : progName(_progName) {
    if (!QDir(progName).exists())
        qWarning("No such programme: %s", progName.toStdString().c_str());
}

ProgInfo::~ProgInfo() {}

QStringList ProgInfo::sourceFiles() {
    QFileInfoList aux = QDir(progName).entryInfoList();

    QStringList list;
    for (int i(0); i < aux.count(); ++i)
        if ((aux[i].suffix() == "cpp") || (aux[i].suffix() == "c"))
            list.push_back(progName + '/' + aux[i].fileName());

    return list;
}

QStringList ProgInfo::objectFiles() {
    QFileInfoList aux = QDir(progName).entryInfoList();

    QStringList list;
    for (int i(0); i < aux.count(); ++i)
        if (aux[i].suffix() == "o")
            list.push_back(progName + '/' + aux[i].fileName());

    return list;
}

void ProgInfo::setProg(const QString &_progName) {
    progName = _progName;
}

EnvironmentConfigurationWidget::EnvironmentConfigurationWidget(Environment *_env, QWidget *parent) : QDialog(parent), env(_env) {
    setModal(true);

    setupUI();

    setFixedSize(sizeHint());
}

EnvironmentConfigurationWidget::~EnvironmentConfigurationWidget() {}

void EnvironmentConfigurationWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QAction *action;
    QHBoxLayout *curLayout;
    QLabel *nameLabel;
    QToolButton *browseButton;

    QGroupBox *box = new QGroupBox(tr("Paths"), this);
    QVBoxLayout *boxLayout = new QVBoxLayout;

    // Initial values.
    cc = env->getCc();
    cpp = env->getCpp();

    // The C compiler line.
    curLayout = new QHBoxLayout;

    nameLabel = new QLabel(tr("C Compiler"), this);
    nameLabel->setFixedWidth(fontMetrics().width(QString("C++ Compiler")));
    curLayout->addWidget(nameLabel);

    ccEdit = new QLineEdit(cc, this);
    ccEdit->setFixedWidth(fontMetrics().width(QString("0")) * 32);
    connect(ccEdit, SIGNAL(textChanged(const QString &)), this, SIGNAL(ccChanged(const QString &)));
    connect(ccEdit, SIGNAL(textChanged(const QString &)), this, SLOT(test()));
    //TODO See if a QCompleter is a good idea.
    curLayout->addWidget(ccEdit);

    browseButton = new QToolButton(this);
    action = new QAction(QIcon(":/package_system.xpm"), "", this);
    browseButton->setDefaultAction(action);
    connect(browseButton, SIGNAL(clicked()), this, SLOT(getCcPath()));
    curLayout->addWidget(browseButton);

    ccStatusLabel = new QLabel(this);
    curLayout->addWidget(ccStatusLabel);

    boxLayout->addLayout(curLayout);

    // The CPP compiler line.
    curLayout = new QHBoxLayout;

    nameLabel = new QLabel(tr("C++ Compiler"), this);
    nameLabel->setFixedWidth(fontMetrics().width(QString("C++ Compiler")));
    curLayout->addWidget(nameLabel);

    cppEdit = new QLineEdit(cpp, this);
    cppEdit->setFixedWidth(fontMetrics().width(QString("0")) * 32);
    connect(cppEdit, SIGNAL(textChanged(const QString &)), this, SLOT(test()));
    connect(cppEdit, SIGNAL(textChanged(const QString &)), this, SIGNAL(cppChanged(const QString &)));
    curLayout->addWidget(cppEdit);

    browseButton = new QToolButton(this);
    action = new QAction(QIcon(":/package_system.xpm"), "", this);
    browseButton->setDefaultAction(action);
    connect(browseButton, SIGNAL(clicked()), this, SLOT(getCppPath()));
    curLayout->addWidget(browseButton);

    cppStatusLabel = new QLabel(this);
    curLayout->addWidget(cppStatusLabel);

    boxLayout->addLayout(curLayout);

    box->setLayout(boxLayout);

    mainLayout->addWidget(box);

    // The options line.
    box = new QGroupBox(tr("Options"), this);

    curLayout = new QHBoxLayout;

    nameLabel = new QLabel(tr("Compile"), this);
    curLayout->addWidget(nameLabel);

    compileOptEdit = new QLineEdit(env->getCompileOpt(), this);
    compileOptEdit->setFixedWidth(fontMetrics().width(QString("0")) * 4);
    connect(compileOptEdit, SIGNAL(textChanged(const QString &)), this, SLOT(updateOpts()));
    curLayout->addWidget(compileOptEdit);

    nameLabel = new QLabel(tr("Link"), this);
    curLayout->addWidget(nameLabel);

    linkOptEdit = new QLineEdit(env->getLinkOpt(), this);
    linkOptEdit->setFixedWidth(fontMetrics().width(QString("0")) * 4);
    connect(linkOptEdit, SIGNAL(textChanged(const QString &)), this, SLOT(updateOpts()));
    curLayout->addWidget(linkOptEdit);

    nameLabel = new QLabel(tr("Other"), this);
    curLayout->addWidget(nameLabel);

    otherOptEdit = new QLineEdit(env->getOtherOpt().join(" "), this);
    otherOptEdit->setFixedWidth(fontMetrics().width(QString("0")) * 23);
    connect(otherOptEdit, SIGNAL(textChanged(const QString &)), this, SLOT(updateOpts()));
    curLayout->addWidget(otherOptEdit);

    box->setLayout(curLayout);

    mainLayout->addWidget(box);

    // The bottom line.
    QHBoxLayout *bottomSection = new QHBoxLayout;

    testButton = new QPushButton(tr("Test"), this);
    bottomSection->addWidget(testButton);
    connect(testButton, SIGNAL(clicked()), this, SLOT(test()));
    bottomSection->addStretch();
    doneButton = new QPushButton(tr("Done"), this);
    connect(doneButton, SIGNAL(clicked()), this, SLOT(hide()));
    bottomSection->addWidget(doneButton);

    mainLayout->addLayout(bottomSection);

    setLayout(mainLayout);

    test();
}

void EnvironmentConfigurationWidget::getCcPath() {
    QString path = QFileDialog::getOpenFileName(this, "DevEditor", QFileInfo(env->getCc()).absolutePath());

    if (!path.isEmpty()) {
        cc = path;
        ccEdit->setText(path);

        if (env->isExe(path))
            ccStatusLabel->setPixmap(QPixmap(":/apply.xpm").scaledToHeight(cppEdit->height() - 2));
        else
            ccStatusLabel->setPixmap(QPixmap(":/history_clear.xpm").scaledToHeight(cppEdit->height() - 2));

        emit ccChanged(path);
    }
}

void EnvironmentConfigurationWidget::getCppPath() {
    QString path = QFileDialog::getOpenFileName(this, "DevEditor", QFileInfo(env->getCpp()).absolutePath());

    if (!path.isEmpty()) {
        cpp = path;
        cppEdit->setText(path);

        if (env->isExe(path))
            cppStatusLabel->setPixmap(QPixmap(":/apply.xpm").scaledToHeight(cppEdit->height() - 2));
        else
            cppStatusLabel->setPixmap(QPixmap(":/history_clear.xpm").scaledToHeight(cppEdit->height() - 2));
    }
}

void EnvironmentConfigurationWidget::test() {
    cc = ccEdit->text();
    cpp = cppEdit->text();

    if (env->exists(cc) && env->isExe(cc))
        ccStatusLabel->setPixmap(QPixmap(":/apply.xpm").scaledToHeight(ccEdit->height() - 2));
    else
        ccStatusLabel->setPixmap(QPixmap(":/history_clear.xpm").scaledToHeight(ccEdit->height() - 2));

    if (env->exists(cpp) && env->isExe(cpp))
        cppStatusLabel->setPixmap(QPixmap(":/apply.xpm").scaledToHeight(cppEdit->height() - 2));
    else
        cppStatusLabel->setPixmap(QPixmap(":/history_clear.xpm").scaledToHeight(cppEdit->height() - 2));
}

void EnvironmentConfigurationWidget::updateOpts() {
    emit optsChanged(compileOptEdit->text(), linkOptEdit->text(), otherOptEdit->text().split(" "));
}

QString ProgInfo::exe() {
    QStringList dirs = QFileInfo(progName).absolutePath().split('/');
    QString binName = progName + dirs[dirs.count() - 1];

#ifdef Q_WS_WIN
    return binName + ".exe";
#endif

#ifdef Q_WS_X11
    return binName;
#endif

    return "";
}

Environment::Environment(QWidget *parent) : QObject(parent), mb(0) {
    badExts << "o" << "exe" << "log" << "xpm"; //TODO Add all extensions of images, etc.

    isWindows = false;
    isUnix = false;

#ifdef Q_WS_WIN
    isWindows = true;
    qWarning("This is Windows");
#endif

#ifdef Q_WS_X11
    isUnix = true;
    qWarning("This is *nix");
#endif

    if (isWindows) {
        defaultCc = "C:\\MinGW\\bin\\gcc.exe";
        defaultCpp = "C:\\MinGW\\bin\\g++.exe";
    } else if (isUnix) {
        defaultCc = "/usr/bin/gcc";
        defaultCpp = "/usr/bin/g++";
    }

    readSettings();

    checkCompiler();

    ecw = new EnvironmentConfigurationWidget(this, parent);
    connect(ecw, SIGNAL(ccChanged(const QString &)), this, SLOT(setCc(const QString &)));
    connect(ecw, SIGNAL(cppChanged(const QString &)), this, SLOT(setCpp(const QString &)));
    connect(ecw, SIGNAL(optsChanged(const QString &, const QString &, const QStringList &)), this, SLOT(setOpts(const QString&, const QString&, const QStringList&)));

    configureAct = new QAction(QIcon(":/configure.xpm"), tr("Configure compiler"), 0);
    configureAct->setStatusTip(tr("Configure the environment"));
    connect(configureAct, SIGNAL(triggered()), ecw, SLOT(show()));

    bProc = new QProcess(this);
}

Environment::~Environment() {
    delete configureAct;
}

QString Environment::strippedName(const QString &fullFileName) {
    return QFileInfo(fullFileName).fileName();
}

QString Environment::lastDir(const QString &fullFileName) {
    if (!exists(fullFileName)) {
        QString path = fullFileName;
        if (path.data()[path.length() - 1] == '/')
            path.chop(1);
        return path;
    }

    QStringList dirs = QFileInfo(fullFileName).absolutePath().split('/');
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

bool Environment::isExe(const QString &path) {
    if (QFileInfo(path).isExecutable())
        return true;
    return false;
}

bool Environment::mkfile(const QString &dir, const QString &name, bool useTemplate) {
    if (!isDir(dir))
        return false;

    QString path = dir + '/' + name;
    if (exists(path))
        return false;

    if (useTemplate) {
        QFileInfo info(name);

        QString templateName = ":/" + info.fileName() + ".template";

        if (!exists(templateName)) {
            templateName = ":/" + info.completeSuffix() + ".template";
            if (!exists(templateName))
                goto create_empty_file;
        }

        QFile ifile(templateName);
        ifile.open(QFile::ReadOnly | QFile::Text);

        QFile ofile(path);
        ofile.open(QFile::WriteOnly | QFile::Text);

        QTextStream fi(&ifile);
        QTextStream fo(&ofile);

        fo << fi.readAll();

        return true;
    }

create_empty_file:
    QFile file(path);
    if (!file.open(QFile::WriteOnly | QFile::Text))
        return false;

    return true;
}

QString Environment::home() const {
    return QDir::toNativeSeparators(QDir::homePath());
}

QStringList Environment::listViewableFiles(const QString &dir) {
    if (!isDir(dir))
        return QStringList();

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
        if ((i != list.count() - 1) && (result.data()[result.length() - 1] != '/'))
            result += '/';
    }
    return result;
}

bool Environment::isReadableFile(const QString &path) {
    QFileInfo info(path);

    if (info.exists() && info.isFile() && info.isReadable())
        return true;

    return false;
}

void Environment::compileFile(const QString &path, bool partOfBulkJob) {
    if (!canCompile)  {
        emit compileFailed();
        return;
    }

    if (!partOfBulkJob) {
        connect(this, SIGNAL(compileFailed()), this, SLOT(compilationFailed()));
    } else {
        disconnect(this, SIGNAL(compileFailed()), this, SLOT(compilationFailed()));
    }

    if (!exists(path)) {
        emit compileFailed();
        return;
    }

    if (!partOfBulkJob)
        if (mb)
            mb->reset();

    QFileInfo info = QFileInfo(path);
    QDir::setCurrent(info.absolutePath());

    disconnect(bProc, 0, 0, 0);
    connect(bProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(compileProcessExited(int, QProcess::ExitStatus)));

    QString command;
    QString shortCommand;

    if (info.suffix() == "cpp") {
        command = cpp; // + " -Wall -c " + " -o " + info.baseName() + ".o " + info.fileName();
        shortCommand = QFileInfo(cpp).fileName();
    } else if (info.suffix() == "c") {
        command = cc; // + " -Wall -c " + " -o " + info.baseName() + ".o " + info.fileName();
        shortCommand = QFileInfo(cc).fileName();
    } else {
        if (mb)
            mb->warn(tr("Don't know how to compile ") + path);

        emit compileFailed();
        return;
    }

    QStringList arguments;
    arguments << otherOpt
        << compileOpt
        << outputOpt << info.baseName() + ".o"
        << info.fileName();

    if (mb)
        mb->message(tr("compiling %1 (%2)").arg(info.fileName()).arg(shortCommand));

    bProc->start(command, arguments);
}

void Environment::compileProcessExited(int exitCode, QProcess::ExitStatus exitStatus) {
    if (mb) {
        QStringList msgs = QString(bProc->readAllStandardError()).split("\n");
        for (int i(0); i < msgs.count(); ++i) {
            mb->error(msgs[i]);
        }

        msgs = QString(bProc->readAllStandardOutput()).split("\n");
        for (int i(0); i < msgs.count(); ++i) {
            mb->message(msgs[i]);
        }
    }

    if ((exitStatus == QProcess::CrashExit) || (exitCode != 0))
        emit compileFailed();

    emit compileSuccesful();
}

void Environment::compilationFailed() {
    if (mb)
        mb->error(tr("*** Failed ***"));
}

void Environment::setMessageBox(MessageBox *_mb) {
    mb = _mb;
}

void Environment::linkObjects(const QString &path, bool dontClear) {
    if (!canCompile) {
        emit linkFailed();
        return;
    }

    if (!exists(path)) {
        emit linkFailed();
        return;
    }

    if (!dontClear)
        if (mb)
            mb->reset();

    ProgInfo pi(path);
    QFileInfo info = QFileInfo(path);
    QDir::setCurrent(info.absolutePath());

    disconnect(bProc, 0, 0, 0);
    connect(bProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(linkProcessExited(int, QProcess::ExitStatus)));

    mb->message("------- Linking -------");

    QStringList objects = pi.objectFiles();
    if (objects.isEmpty()) {
        emit linkFailed();
        return;
    }

    bool isCppProject = false;
    QFileInfoList aux = QDir(path).entryInfoList();
    for (int i(0); i < aux.count(); ++i)
        if (aux[i].suffix() == "cpp") {
            isCppProject = true;
            break;
        }

    QString command = cc;
    if (isCppProject)
        command = cpp;

    QString exeName = lastDir(path);
    if (isWindows)
        exeName += ".exe";

    QStringList arguments;
    arguments << outputOpt << exeName
        << objects;

    bProc->start(command, arguments);

    if (mb)
        mb->message(tr("linking ") + exeName + " (" + QFileInfo(command).fileName() + ")");
}

void Environment::linkProcessExited(int exitCode, QProcess::ExitStatus exitStatus) {
    if (mb) {
        QStringList msgs = QString(bProc->readAllStandardError()).split("\n");
        for (int i(0); i < msgs.count(); ++i) {
            mb->error(msgs[i]);
        }

        msgs = QString(bProc->readAllStandardOutput()).split("\n");
        for (int i(0); i < msgs.count(); ++i) {
            mb->message(msgs[i]);
        }
    }

    if ((exitStatus == QProcess::CrashExit) || (exitCode != 0))
        emit linkFailed();

    emit linkSuccesful();
}

void Environment::run(const QString &path) {
    QString command;
    QStringList arguments;
    if (isUnix) {
        command = "konsole";
        arguments << "--noclose" << "-e";
    } else if (isWindows) {
    }

    arguments << path;

    static QProcess *proc = new QProcess(this);
    disconnect(proc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SIGNAL(runDone()));
    connect(proc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SIGNAL(runDone()));
    proc->start(command, arguments);

//     if (isUnix) {
//         return (system(QString("xterm -e \"%1 && echo \\\"Press return to continue\\\" && read\"").arg(path).toStdString().c_str()) == 0);
//     } else if (isWindows) {
//         return (system(QString("\"%1\" && pause").arg(path).toStdString().c_str()) == 0);
//     }
}

QAction* Environment::getConfigureAct() {
    return configureAct;
}

QString Environment::getCc() const {
    return cc;
}

QString Environment::getCpp() const {
    return cpp;
}

void Environment::setCc(const QString &path) {
    cc = path;
    writeSettings();

    checkCompiler();

    writeSettings();
}

void Environment::setCpp(const QString &path) {
    cpp = path;
    writeSettings();

    checkCompiler();

    writeSettings();
}

void Environment::writeSettings() {
    QSettings settings("ScvTech", "DevEditor Environment");
    settings.setValue("cc", cc);
    settings.setValue("cpp", cpp);
    settings.setValue("compileOpt", compileOpt);
    settings.setValue("outputOpt", outputOpt);
    settings.setValue("linkOpt", linkOpt);
    settings.setValue("otherOpt", otherOpt);
}

void Environment::readSettings() {
    QSettings settings("ScvTech", "DevEditor Environment");
    cc = settings.value("cc", "").toString();
    cpp = settings.value("cpp", "").toString();

    if (cc.isEmpty())
        cc = defaultCc;

    if (cpp.isEmpty())
        cpp = defaultCpp;

    compileOpt = settings.value("compileOpt", "-c").toString();
    outputOpt = settings.value("outputOpt", "-o").toString();
    linkOpt = settings.value("linkOpt", "").toString();
    otherOpt = settings.value("otherOpt", "-Wall").toStringList();
}

bool Environment::checkCompiler() {
    bool aux = canCompile;

    if (exists(cc) && isExe(cc) && exists(cpp) && isExe(cpp))
        canCompile = true;
    else
        canCompile = false;

    if (aux != canCompile)
        emit canCompileChanged(canCompile);

    return canCompile;
}

bool Environment::getCanCompile() const {
    return canCompile;
}

QString Environment::getCompileOpt() const {
    return compileOpt;
}

QString Environment::getLinkOpt() const {
    return linkOpt;
}

QStringList Environment::getOtherOpt() const {
    return otherOpt;
}

void Environment::setOpts(const QString &co, const QString &lo, const QStringList &oo) {
    compileOpt = co;
    linkOpt = lo;

    otherOpt.clear();
    for (int i(0); i < oo.count(); ++i) {
        if ((!oo[i].isEmpty()) && (oo[i] != " "))
            otherOpt << oo[i];
    }

    writeSettings();
}
