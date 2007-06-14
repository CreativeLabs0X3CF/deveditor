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

#include "texteditwidget.h"

#include "highlighter.h"

NumberBar::NumberBar(QWidget *parent) : QWidget(parent), edit(0) {
  setFixedWidth(fontMetrics().width(QString("00") + 3)); // Changed during paintEvent()
}

NumberBar::~NumberBar() {
}

void NumberBar::setTextEdit(QTextEdit *_edit) {
  edit = _edit;
  connect(edit->document()->documentLayout(), SIGNAL(update(const QRectF &)), this, SLOT(update()));
  connect(edit->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(update()));
}

void NumberBar::paintEvent(QPaintEvent *) {
  QAbstractTextDocumentLayout *layout = edit->document()->documentLayout();
  int contentsY = edit->verticalScrollBar()->value();
  qreal pageBottom = contentsY + edit->viewport()->height();
  const QFontMetrics fm = fontMetrics();
  const int ascent = fm.ascent() + 1;
  int lineCount = 1;

  QPainter p(this);

  //TODO This loops around too much for large files. Make it more efficient.
  for (QTextBlock block = edit->document()->begin(); block.isValid(); block = block.next(), ++lineCount) {
    const QRectF boundingRect = layout->blockBoundingRect(block);

    QPointF position = boundingRect.topLeft();
    if (position.y() + boundingRect.height() < contentsY)
      continue;
    if (position.y() > pageBottom)
      break;

    const QString txt = QString::number(lineCount);
    p.drawText(width() - fm.width(txt) - 2, qRound(position.y()) - contentsY + ascent, txt);
  }

  const QString txt = QString::number(lineCount);
  if (txt.length() >= 2)
    setFixedWidth(fontMetrics().width(txt) + 3);
  else
    setFixedWidth(fontMetrics().width("00") + 3);
}

bool NumberBar::event(QEvent *event) {
//   if (event->type() == QEvent::ToolTip) {
//     QHelpEvent *helpEvent = (QHelpEvent*)event;
//
//     if (currentRect.contains(helpEvent->pos()))
//       QToolTip::showText(helpEvent->globalPos(), "Current line");
//   }

  return QWidget::event(event);
}

TextEditWidget::TextEditWidget(QWidget *parent) : QFrame(parent), curFile(""), shownName("") {
  setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
  setLineWidth(2);

  // Setup the main QTextEdit
  view = new QTextEdit(this);
  view->setTabStopWidth(fontMetrics().width(QString("00")));

  font.setFamily("Monospace");
  font.setFixedPitch(true);
  font.setPointSize(12);
  view->setFont(font);

  view->setLineWrapMode(QTextEdit::NoWrap);
  view->setFrameStyle(QFrame::NoFrame);
  view->installEventFilter(this);

  connect(view, SIGNAL(cursorPositionChanged()), this, SLOT(cursorChanged()));

  // Setup the line number pane
  numbers = new NumberBar(this);
  numbers->setTextEdit(view);

  box = new QHBoxLayout(this);
  box->setSpacing(0);
  box->setMargin(0);
  box->addWidget(numbers);
  box->addWidget(view);

  highlighter = new Highlighter(view->document());
  highlighterOn = true;

  cursorChanged();
}

TextEditWidget::~TextEditWidget() {
}

QTextEdit* TextEditWidget::getTextEdit() const {
  return view;
}

void TextEditWidget::toggleHighlighting() {
  if (highlighterOn) {
    highlighter->setDocument((QTextDocument *)0);
    highlighterOn = false;
  } else {
    highlighter->setDocument(view->document());
    highlighterOn = true;
  }

  emit highlighting(highlighterOn);
}

void TextEditWidget::updateFont() {
  view->setFont(font);
}

void TextEditWidget::setLineNumbering(bool _state) {
  numbers->setVisible(_state);
}

QString TextEditWidget::getCurFile() const {
  return curFile;
}

QString TextEditWidget::getShownName() const {
  return shownName;
}

bool TextEditWidget::getHighlighting() const {
  return highlighterOn;
}

void TextEditWidget::setCurFile(QString _curFile) {
  curFile = _curFile;
}

void TextEditWidget::setShownName(QString _shownName) {
  shownName = _shownName;
}

Highlighter* TextEditWidget::getHighlighter() const {
  return highlighter;
}

QFont* TextEditWidget::getFont() {
  return &font;
}

void TextEditWidget::cursorChanged() {
  static QTextBlock cblock;
  static int lineCount;

  //TODO Fix erronous delete row after bug.
  //TODO Optimize.

  currentLine = view->textCursor().blockNumber() + 1;

  if ((currentLine == lineCount) && (currentLine != 1))
    return;

  bool mod = getDocument()->isModified();

  QTextBlock block = highlight.block();
  QTextBlockFormat fmt = block.blockFormat();
  QColor bg = view->palette().base().color();
  fmt.setBackground(bg);
  highlight.setBlockFormat(fmt);

//   if (cblock.isValid()) {
//     qWarning("Previous block is valid. Attempting to remove highlight");
//     highlight = QTextCursor(cblock);
//     highlight.setBlockFormat(fmt);
//     qWarning("Highlight removed");
//   }

  lineCount = 1;
  for (cblock = view->document()->begin(); cblock.isValid(); cblock = cblock.next(), ++lineCount)
    if (lineCount == currentLine) {
      fmt = cblock.blockFormat();
      QColor bg = QColor(192, 192, 192, 100);
      fmt.setBackground(bg);

      highlight = QTextCursor(cblock);
      highlight.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
      highlight.setBlockFormat(fmt);

      break;
    } else {
      fmt = cblock.blockFormat();
      QColor bg = view->palette().base().color();
      fmt.setBackground(bg);

      highlight = QTextCursor(cblock); // I'll go to Hell for this.
      highlight.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
      highlight.setBlockFormat(fmt);
    }

  getDocument()->setModified(mod);

  emit cursorPositionChanged(currentLine, view->textCursor().columnNumber() + 1);
}

bool TextEditWidget::eventFilter(QObject *obj, QEvent *event) {
  if (obj != view)
    return QFrame::eventFilter(obj, event);

  if (event->type() == QEvent::ToolTip) {
    QHelpEvent *helpEvent = (QHelpEvent*)event;

    QTextCursor cursor = view->cursorForPosition(helpEvent->pos());
    cursor.movePosition(QTextCursor::StartOfWord, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);

    QString word = cursor.selectedText();
    emit mouseHover(word);
    emit mouseHover(helpEvent->pos(), word);

//     QToolTip::showText(helpEvent->globalPos(), word);
  }

  return false;
}

QTextDocument* TextEditWidget::getDocument() {
  return view->document();
}
