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

ProgInfo::~ProgInfo() {
}

QStringList ProgInfo::sourceFiles() {
  QFileInfoList aux = QDir(progName).entryInfoList();

  QStringList list;
  for (int i(0); i < aux.count(); ++i)
    if ((aux[i].suffix() == "cpp") || (aux[i].suffix() == "c"))
      list.push_back(progName + QDir::separator() + aux[i].fileName());

  return list;
}

QStringList ProgInfo::objectFiles() {
  QFileInfoList aux = QDir(progName).entryInfoList();

  QStringList list;
  for (int i(0); i < aux.count(); ++i)
    if (aux[i].suffix() == "o")
      list.push_back(progName + QDir::separator() + aux[i].fileName());

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

EnvironmentConfigurationWidget::~EnvironmentConfigurationWidget() {
}

void EnvironmentConfigurationWidget::setupUI() {
  QVBoxLayout *mainLayout = new QVBoxLayout(this);

  QAction *action;
  QHBoxLayout *curLayout;
  QLabel *nameLabel;
  QToolButton *browseButton;

  // The C compiler line.
  curLayout = new QHBoxLayout;

  nameLabel = new QLabel(tr("C Compiler"), this);
  nameLabel->setFixedWidth(fontMetrics().width(QString("C++ Compiler")));
  curLayout->addWidget(nameLabel);

  ccEdit = new QLineEdit(env->getCc(), this);
  //TODO See if a QCompleter is a good idea.
  curLayout->addWidget(ccEdit);

  browseButton = new QToolButton(this);
  action = new QAction(QIcon(":/package_system.xpm"), "", this);
  browseButton->setDefaultAction(action);
  connect(browseButton, SIGNAL(pressed()), this, SLOT(getCcPath()));
  curLayout->addWidget(browseButton);

  ccStatusLabel = new QLabel(this);
  if (env->exists(env->getCc()) && env->isExe(env->getCc()))
    ccStatusLabel->setPixmap(QPixmap(":/apply.xpm").scaledToHeight(ccEdit->height() - 4));
  else
    ccStatusLabel->setPixmap(QPixmap(":/history_clear.xpm").scaledToHeight(ccEdit->height() - 4));
  curLayout->addWidget(ccStatusLabel);

  mainLayout->addLayout(curLayout);

  // The CPP compiler line.
  curLayout = new QHBoxLayout;

  nameLabel = new QLabel(tr("C++ Compiler"), this);
  nameLabel->setFixedWidth(fontMetrics().width(QString("C++ Compiler")));
  curLayout->addWidget(nameLabel);

  cppEdit = new QLineEdit(env->getCpp(), this);
  curLayout->addWidget(cppEdit);

  browseButton = new QToolButton(this);
  action = new QAction(QIcon(":/package_system.xpm"), "", this);
  browseButton->setDefaultAction(action);
  connect(browseButton, SIGNAL(pressed()), this, SLOT(getCppPath()));
  curLayout->addWidget(browseButton);

  cppStatusLabel = new QLabel(this);
  if (env->exists(env->getCpp()) && env->isExe(env->getCpp()))
    cppStatusLabel->setPixmap(QPixmap(":/apply.xpm").scaledToHeight(cppEdit->height() - 4));
  else
    cppStatusLabel->setPixmap(QPixmap(":/history_clear.xpm").scaledToHeight(cppEdit->height() - 4));
  curLayout->addWidget(cppStatusLabel);

  mainLayout->addLayout(curLayout);

  // The bottom line.
  QHBoxLayout *bottomSection = new QHBoxLayout;

  testButton = new QPushButton(tr("Test"), this);
  bottomSection->addWidget(testButton);
  connect(testButton, SIGNAL(released()), this, SLOT(test()));
  bottomSection->addStretch();
  doneButton = new QPushButton(tr("Done"), this);
  connect(doneButton, SIGNAL(released()), this, SLOT(hide()));
  bottomSection->addWidget(doneButton);

  mainLayout->addLayout(bottomSection);

  setLayout(mainLayout);
}

void EnvironmentConfigurationWidget::getCcPath() {
  QString path = QFileDialog::getOpenFileName(this, "DevEditor", QFileInfo(env->getCc()).absolutePath());

  if (!path.isEmpty()) {
    ccEdit->setText(path);

    if (env->isExe(path))
      ccStatusLabel->setPixmap(QPixmap(":/apply.xpm").scaledToHeight(cppEdit->height() - 4));
    else
      ccStatusLabel->setPixmap(QPixmap(":/history_clear.xpm").scaledToHeight(cppEdit->height() - 4));

    env->setCc(path);
  }
}

void EnvironmentConfigurationWidget::getCppPath() {
    QString path = QFileDialog::getOpenFileName(this, "DevEditor", QFileInfo(env->getCpp()).absolutePath());

  if (!path.isEmpty()) {
    cppEdit->setText(path);

    if (env->isExe(path))
      cppStatusLabel->setPixmap(QPixmap(":/apply.xpm").scaledToHeight(cppEdit->height() - 4));
    else
      cppStatusLabel->setPixmap(QPixmap(":/history_clear.xpm").scaledToHeight(cppEdit->height() - 4));

    env->setCpp(path);
  }
}

void EnvironmentConfigurationWidget::test() {
  if (env->exists(env->getCc()) && env->isExe(env->getCc()))
    ccStatusLabel->setPixmap(QPixmap(":/apply.xpm").scaledToHeight(ccEdit->height() - 4));
  else
    ccStatusLabel->setPixmap(QPixmap(":/history_clear.xpm").scaledToHeight(ccEdit->height() - 4));

  if (env->exists(env->getCpp()) && env->isExe(env->getCpp()))
    cppStatusLabel->setPixmap(QPixmap(":/apply.xpm").scaledToHeight(cppEdit->height() - 4));
  else
    cppStatusLabel->setPixmap(QPixmap(":/history_clear.xpm").scaledToHeight(cppEdit->height() - 4));
}

QString ProgInfo::exe() {
  QStringList dirs = QFileInfo(progName).absolutePath().split(QDir::separator());
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

  ecw = new EnvironmentConfigurationWidget(this, parent);

  configureAct = new QAction(QIcon(":/configure.xpm"), tr("Configure"), 0);
  configureAct->setStatusTip(tr("Configure the environment"));
  connect(configureAct, SIGNAL(triggered()), ecw, SLOT(show()));
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
    if (path.data()[path.length() - 1] == QDir::separator())
      path.chop(1);
    return path;
  }

  QStringList dirs = QFileInfo(fullFileName).absolutePath().split(QDir::separator());
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

  QString path = dir + separator() + name;
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

QChar Environment::separator() const {
  return QDir::separator();
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
    if ((i != list.count() - 1) && (result.data()[result.length() - 1] != separator()))
      result += separator();
  }
  return result;
}

bool Environment::isReadableFile(const QString &path) {
  QFileInfo info(path);

  if (info.exists() && info.isFile() && info.isReadable())
    return true;

  return false;
}

bool Environment::compileFile(const QString &path, bool partOfBulkJob) {
  if (!exists(path))
    return false;

  if (!partOfBulkJob)
    if (mb)
      mb->reset();

  QFileInfo info = QFileInfo(path);
  QDir::setCurrent(info.absolutePath());

  QString command;
  QString shortCommand;

  if (info.suffix() == "cpp") {
    command = cpp + " -c " + " -o " + info.baseName() + ".o " + info.fileName() + ">& compile.log";
    shortCommand = QFileInfo(cpp).fileName();
  } else if (info.suffix() == "c") {
    command = cc + " -c " + " -o " + info.baseName() + ".o " + info.fileName()  + ">& compile.log";
    shortCommand = QFileInfo(cc).fileName();
  } else {
    if (mb)
      mb->warn(tr("Don't know how to compile ") + path);

    return false;
  }

  if (mb)
    mb->message(tr("compiling %1 (%2)").arg(info.fileName()).arg(shortCommand));

  int ret = system(command.toStdString().c_str());

  QFile fclog("compile.log");
  fclog.open(QFile::ReadOnly | QFile::Text);
  QTextStream clog(&fclog);

  while (!clog.atEnd()) {
    QString line = clog.readLine();

    mb->error(line);
  }

  fclog.remove();

  if (mb) {
    if (ret == 0) {
      if (!partOfBulkJob)
        mb->good(tr("*** Succes ***"));
    }else
      mb->error(tr("*** Exited with status: %1 ***").arg(ret));
  }

  return (ret == 0);
}

void Environment::setMessageBox(MessageBox *_mb) {
  mb = _mb;
}

bool Environment::linkObjects(const QString &path) {
  if (!exists(path))
    return false;

  if (mb)
    mb->reset();

  ProgInfo pi(path);
  QFileInfo info = QFileInfo(path);
  QDir::setCurrent(info.absolutePath());

  mb->message("Will try to link every object file in " + lastDir(path));

  QString objects = pi.objectFiles().join(" ");

//   if (mb)
//     mb->message("Object files: " + objects);

  if (objects.isEmpty())
    return false;

  bool isCppProject = false;
  QFileInfoList aux = QDir(path).entryInfoList();
  for (int i(0); i < aux.count(); ++i)
    if (aux[i].suffix() == "cpp") {
      isCppProject = true;
      break;
    }

  QString shortCommand = cc;
  if (isCppProject)
    shortCommand = cpp;

  QString exeName = lastDir(path);
  if (isWindows)
    exeName += ".exe";

  QString command = shortCommand + " -o " + exeName + " " + objects + " >& link.log";

//   if (mb)
//     mb->message(command);

  if (mb)
    mb->message(tr("linking ") + exeName + " (" + QFileInfo(shortCommand).fileName() + ")");

  int ret = system(command.toStdString().c_str());

  QFile fclog("link.log");
  fclog.open(QFile::ReadOnly | QFile::Text);
  QTextStream clog(&fclog);

  while (!clog.atEnd()) {
    QString line = clog.readLine();

    mb->error(line);
  }

  fclog.remove();

  if (mb)
    if (ret == 0)
      mb->good(tr("*** Succes ***"));
    else
      mb->error(tr("*** Problems linking objects ***"));

  return (ret == 0);
}

bool Environment::run(const QString &path) {
  if (isUnix) {
    qWarning("About to run %s", QString("xterm -e \".%1 && echo \\\"Press return to continue\\\" && read\"").arg(path).toStdString().c_str());
    return (system(QString("xterm -e \"%1 && echo \\\"Press return to continue\\\" && read\"").arg(path).toStdString().c_str()) == 0);
  } else if (isWindows) {
  }

  return false;
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
}

void Environment::setCpp(const QString &path) {
  cpp = path;
  writeSettings();
}

void Environment::writeSettings() {
  QSettings settings("ScvTech", "DevEditor Environment");
  settings.setValue("cc", cc);
  settings.setValue("cpp", cpp);
}

void Environment::readSettings() {
  QSettings settings("ScvTech", "DevEditor Environment");
  cc = settings.value("cc", "").toString();
  cpp = settings.value("cpp", "").toString();

  if (cc.isEmpty())
    cc = defaultCc;

  if (cpp.isEmpty())
    cpp = defaultCpp;
}
