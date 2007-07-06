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


#ifndef NEWPROJECTWIDGET_H
#define NEWPROJECTWIDGET_H

#include <QDialog>

class QLineEdit;
class QLabel;
class QPushButton;
class QString;
class QModelIndex;
class QTextBrowser;

//! A programme name generator.
class NameGenerator {
public:
    NameGenerator();
    ~NameGenerator();

    /*!
        Generates a name and returns it. The name does not appear in dir.
    */
    QString generateName(const QString &dir);

private:
    //! List of words used in random name generation.
    QStringList adjectives, nouns, objects, almostVerb;
};

/**
  @author Alexandru Scvortov <scvalex@gmail.com>
*/
class NewProjectWidget : public QDialog {
  Q_OBJECT

public:
    NewProjectWidget(QWidget *parent = 0);

    ~NewProjectWidget();

signals:
    /*!
        Emitted when create is clicked.

        @param dir The directory in which to create the new programme.
        @param templ The directory in which the template resides.
    */
    void createProgramme(const QString &dir, const QString &templ);

private slots:
    //! Pops up a file dialog asking for the location directory.
    void getLocationPath();

    //! Is called whenever progNameEdit or locationEdit's text changes.
    void locationChanged();

    //! Regenerates the programme name displayed in the edit box.
    void regenerateProgName();

    /*!
        Activated when a new template is selected.

        Changes the description box and updates currentTemplate.
    */
    void templateSelectionChanged(const QModelIndex &index);

    //! @internal Called when create is clicked.
    void createProg();

private:
    void setupUI();

    QLineEdit *progNameEdit;
    QLineEdit *locationEdit;
    QLabel *finalLocationDisplay;
    QPushButton *createButton;
    QTextBrowser *tDesc;

    QString progDir;

    void writeSettings();
    void readSettings();

    NameGenerator *ng;

    QStringList templates;

    QString currentTemplate;
};

#endif
