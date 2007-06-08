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


#ifndef TEXTEDITWIDGET_H
#define TEXTEDITWIDGET_H

#include <QFrame>
#include <QPixmap>
#include <QTextCursor>

class QTextEdit;
class QHBoxLayout;
class Highlighter;

class NumberBar : public QWidget {
  Q_OBJECT

public:
  NumberBar(QWidget *parent);
  ~NumberBar();

  void setCurrentLine(int _line);

  void setTextEdit(QTextEdit *_edit);
  void paintEvent(QPaintEvent *event);

protected:
  bool event(QEvent *event);

  QTextEdit *edit;
  QPixmap currentMarker;
  int currentLine;
  QRect currentRect;
};

class TextEditWidget : public QFrame {
  Q_OBJECT

public:
  TextEditWidget(QWidget *parent = 0);
  ~TextEditWidget();

  QTextEdit* getTextEdit() const;

  QString getCurFile() const;
  QString getShownName() const;

  void setCurFile(QString _curFile);
  void setShownName(QString _shownName);

  Highlighter* getHighlighter() const;

  bool getHighlighting() const;

  /*!
    @return The font used for the text.
    @note No changes to font are displayed until updateFont() is called.
    @sa updateFont()
  */
  QFont* getFont();

  /*!
    Sets the line that should have the current line indicator.
    A value of -1 indicates no line should show the indicator.
  */
  void setCurrentLine(int _line);

  //! @internal Used to get tooltip events from the view for the hover signal.
  bool eventFilter(QObject *obj, QEvent *event);

  //! Returns the QTextEdit's document.
  QTextDocument* getDocument();

public slots:
  //! Toggles syntax highlighting.
  void toggleHighlighting();
  /*!
    Displays any changest to font.
    @sa getFont()
  */
  void updateFont();

protected slots:
  //! @internal Used to update the highlight on the current line.
  void textChanged(int pos, int added, int removed);

signals:
  /*!
    Emmited when the Highlighter is activated or deactivated.
    @param state The current state of the Highlighter.
  */
  void highlighting(bool state);

  /*!
    Emitted when the mouse is hovered over the text edit component.
    @param word The word under the mouse pointer.
  */
  void mouseHover(const QString &word);

  /*!
    Emmited when the mouse is hovered over the text edit component.
    @param pos The position of the mouse pointer.
    @param word The word under the mouse pointer.
  */
  void mouseHover(const QPoint &pos, const QString &word);

protected:
  QTextEdit *view;
  NumberBar *numbers;
  QHBoxLayout *box;
  int currentLine;
  QTextCursor highlight;

  QString curFile;
  QString shownName;

  Highlighter *highlighter;
  bool highlighterOn;

  QFont font;
};

#endif
