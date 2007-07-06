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


#include "newprojectwidget.h"

#include <QtGui>

NameGenerator::NameGenerator() {
    adjectives << "Dynamic" << "Worldwide" << "Advanced"  << "Strategic"
               << "Rapid" << "Competitive" << "Distributed" << "Peer-Distributed";

    nouns << "Revenue" << "Market" << "Technology" << "Multimedia"
          << "Audio" << "Network" << "Management";

    almostVerb << "Enhancing" << "Improving" << "Investigating"
               << "Augmenting" << "Observing" << "Upgrading";

    objects << "System" << "Application" << "Project" << "Utility"
            << "Taskforce";
}

NameGenerator::~NameGenerator() {
}

QString NameGenerator::generateName(const QString &_dir) {
    QDir dir(_dir);

    QStringList files;
    if (dir.exists())
        files = dir.entryList();

    QString progName = adjectives[qrand() % adjectives.count()] + " " +
                       nouns[qrand() % nouns.count()] + " " +
                       almostVerb[qrand() % almostVerb.count()] + " " +
                       objects[qrand() % objects.count()];

    if (!files.contains(progName, Qt::CaseInsensitive))
        return progName;

    int i(1);
    while (files.contains(QString("%1 Mk %2").arg(progName).arg(i), Qt::CaseInsensitive))
        ++i;

    return QString("%1 Mk %2").arg(progName).arg(i);
}

NewProjectWidget::NewProjectWidget(QWidget *parent) : QDialog(parent) {
    ng = new NameGenerator;

    setWindowTitle("Create New Programme");

    // Generate the list of templates.
    QStringList files = QDir(":/").entryList();
    for (int i(0); i < files.count(); ++i) {
        if (QDir(":/" + files[i]).exists() && files[i].endsWith(".template"))
            templates << files[i].left(files[i].indexOf(".template"));
    }

    readSettings();

    setupUI();
}

NewProjectWidget::~NewProjectWidget() {
    delete ng;

    writeSettings();
}

void NewProjectWidget::setupUI() {
    // Create the model used to display the templates.

    QStringListModel *model = new QStringListModel(this);
    model->setStringList(templates);

    // Setup the user interface.
    QVBoxLayout *mainLayout = new QVBoxLayout;

        QLabel *descLabel = new QLabel(tr("<strong>Programme information</strong>"), this);

    mainLayout->addWidget(descLabel);

        QFrame *sep = new QFrame(this);
        sep->setFrameShape(QFrame::HLine);
        sep->setFrameShadow(QFrame::Sunken);

    mainLayout->addWidget(sep);

        QHBoxLayout *curLayout = new QHBoxLayout;

            QGroupBox *tGroup = new QGroupBox(tr("Templates"), this);
            QVBoxLayout *lay = new QVBoxLayout;

                QListView *tList = new QListView(this);
                tList->setModel(model);
                tList->setEditTriggers(QAbstractItemView::NoEditTriggers);
                tList->setCurrentIndex(model->index(1, 0, QModelIndex()));
                currentTemplate = templates[1];
                connect(tList, SIGNAL(clicked(const QModelIndex&)), this, SLOT(templateSelectionChanged(const QModelIndex &)));

            lay->addWidget(tList);
            tGroup->setLayout(lay);

        curLayout->addWidget(tGroup);

            tDesc = new QTextBrowser(this);
            QFile descFile(QString(":/%1.template/desc.meta").arg(templates[1]));
            if (descFile.open(QFile::ReadOnly | QFile::Text))
                tDesc->setPlainText(QTextStream(&descFile).readAll());
            else
                tDesc->setPlainText("");

        curLayout->addWidget(tDesc);

    mainLayout->addLayout(curLayout);

        QGroupBox *properties = new QGroupBox(tr("Properties"), this);

        QVBoxLayout *propLayout = new QVBoxLayout;

            curLayout = new QHBoxLayout;
            QLabel *progNameLabel = new QLabel(tr("Programme name:"), this);
            progNameEdit = new QLineEdit(ng->generateName(progDir), this);
            connect(progNameEdit, SIGNAL(textChanged(QString)), this, SLOT(locationChanged()));
            QToolButton *button = new QToolButton(this);
            button->setToolTip(tr("Generate another name"));
            QAction *action = new QAction(QIcon(":/filenew.xpm"), "", this);
            button->setDefaultAction(action);
            connect(button, SIGNAL(clicked()), this, SLOT(regenerateProgName()));
            curLayout->addWidget(progNameLabel);
            curLayout->addWidget(progNameEdit);
            curLayout->addWidget(button);

        propLayout->addLayout(curLayout);

            curLayout = new QHBoxLayout;
            QLabel *locationLabel = new QLabel(tr("Location:"), this);
            locationLabel->setFixedWidth(fontMetrics().width(progNameLabel->text()));
            locationEdit = new QLineEdit(progDir, this);
            connect(locationEdit, SIGNAL(textChanged(QString)), this, SLOT(locationChanged()));
            QToolButton *browseButton = new QToolButton(this);
            action = new QAction(QIcon(":/package_system.xpm"), "", this);
            browseButton->setDefaultAction(action);
            connect(browseButton, SIGNAL(clicked()), this, SLOT(getLocationPath()));
            curLayout->addWidget(locationLabel);
            curLayout->addWidget(locationEdit);
            curLayout->addWidget(browseButton);

        propLayout->addLayout(curLayout);

            curLayout = new QHBoxLayout;
            QLabel *finalLocationLabel = new QLabel(tr("Final location:"), this);
            finalLocationLabel->setFixedWidth(fontMetrics().width(progNameLabel->text()));
            finalLocationDisplay = new QLabel(locationEdit->text(), this);
            curLayout->addWidget(finalLocationLabel);
            curLayout->addWidget(finalLocationDisplay);

        propLayout->addLayout(curLayout);

        properties->setLayout(propLayout);

    mainLayout->addWidget(properties);

        sep = new QFrame(this);
        sep->setFrameShape(QFrame::HLine);
        sep->setFrameShadow(QFrame::Sunken);

    mainLayout->addWidget(sep);

        curLayout = new QHBoxLayout;
        curLayout->addStretch();

            createButton = new QPushButton(QIcon(":/window_new.xpm"), tr("&Create"), this);
            createButton->setEnabled(false);
            connect(createButton, SIGNAL(clicked()), this, SLOT(createProg()));

        curLayout->addWidget(createButton);

            QPushButton *cancelButton = new QPushButton(QIcon(":/fileclose.xpm"), tr("&Cancel"), this);
            connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

        curLayout->addWidget(cancelButton);

    mainLayout->addLayout(curLayout);

    setLayout(mainLayout);

    locationChanged();
}

void NewProjectWidget::getLocationPath() {
    QString path = QFileDialog::getExistingDirectory(this, "PEditor", locationEdit->text());

    locationEdit->setText(path);
}

void NewProjectWidget::locationChanged() {
    if (locationEdit->text() != progDir)
        progDir = locationEdit->text();

    QString fl = progDir + progNameEdit->text();

    finalLocationDisplay->setText(fl);

    if (QDir(progDir).exists() && !finalLocationDisplay->text().isEmpty() && !QFile(finalLocationDisplay->text()).exists()) {
        createButton->setEnabled(true);
    } else {
        createButton->setEnabled(false);
    }

    writeSettings();
}

void NewProjectWidget::writeSettings() {
    QSettings settings("ScvTech", "PEditor");
    settings.setValue("progDir", progDir);
}

void NewProjectWidget::readSettings() {
    QSettings settings("ScvTech", "PEditor");
    progDir = settings.value("progDir", ".").toString();

    if (progDir == ".")
        progDir = QDir::homePath();

    progDir = QDir(progDir).absolutePath() + "/";
}

void NewProjectWidget::regenerateProgName() {
    progNameEdit->setText(ng->generateName(progDir));
}

void NewProjectWidget::templateSelectionChanged(const QModelIndex &index) {
    currentTemplate = index.data().toString();

    QFile descFile(QString(":/%1.template/desc.meta").arg(currentTemplate));

    if (descFile.open(QFile::ReadOnly | QFile::Text))
        tDesc->setPlainText(QTextStream(&descFile).readAll());
    else
        tDesc->setPlainText("");
}

void NewProjectWidget::createProg() {
    emit createProgramme(QString("%1%2").arg(progDir).arg(progNameEdit->text()), QString(":/%1.template").arg(currentTemplate));

    close();
}
