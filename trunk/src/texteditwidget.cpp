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

NumberBar::NumberBar(QWidget *parent) : QWidget(parent), edit(0), currentLine(-1) {
  setFixedWidth(fontMetrics().width(QString("0000") + 3)); //TODO Make with dynamic.
  currentMarker = QPixmap(":next.xpm");
}

NumberBar::~NumberBar() {
}

void NumberBar::setCurrentLine(int _line) {
  currentLine = _line;
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

  currentRect = QRect();

  for (QTextBlock block = edit->document()->begin(); block.isValid(); block = block.next(), ++lineCount) {
    const QRectF boundingRect = layout->blockBoundingRect(block);

    QPointF position = boundingRect.topLeft();
    if (position.y() + boundingRect.height() < contentsY)
      continue;
    if (position.y() > pageBottom)
      break;

    const QString txt = QString::number(lineCount);
    p.drawText(width() - fm.width(txt) - 2, qRound(position.y()) - contentsY + ascent, txt);

    if (currentLine == lineCount) {
      p.drawPixmap(19, qRound(position.y()) - contentsY, currentMarker);
      currentRect = QRect(19, qRound(position.y()) - contentsY, currentMarker.width(), currentMarker.height());
    }
  }
}

bool NumberBar::event(QEvent *event) {
  if (event->type() == QEvent::ToolTip) {
    QHelpEvent *helpEvent = (QHelpEvent*)event;

    if (currentRect.contains(helpEvent->pos()))
      QToolTip::showText(helpEvent->globalPos(), "Current line");
  }

  return QWidget::event(event);
}

TextEditWidget::TextEditWidget(QWidget *parent) : QFrame(parent), curFile(""), shownName("") {
  setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
  setLineWidth(2);

  // Setup the main QTextEdit
  view = new QTextEdit(this);

  font.setFamily("Monospace");
  font.setFixedPitch(true);
  font.setPointSize(12);
  view->setFont(font);

  view->setLineWrapMode(QTextEdit::NoWrap);
  view->setFrameStyle(QFrame::NoFrame);
  view->installEventFilter(this);

  connect(view->document(), SIGNAL(contentsChange(int, int, int)), this, SLOT(textChanged(int, int, int)));

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

void TextEditWidget::setCurrentLine(int _line) {
  currentLine = _line;
  numbers->setCurrentLine(currentLine);
  textChanged(0, 0, 1);
}

void TextEditWidget::textChanged(int pos, int removed, int added) {
  Q_UNUSED(pos); // This stops the compiler form issueing warnings like "pos declared but never used"

  if ((removed == 0) && (added == 0))
    return;

  QTextBlock block = highlight.block();
  QTextBlockFormat fmt = block.blockFormat();
  QColor bg = view->palette().base().color();
  fmt.setBackground(bg);
  highlight.setBlockFormat(fmt);

  int lineCount = 1;
  for (QTextBlock block = view->document()->begin(); block.isValid(); block = block.next(), ++lineCount)
    if (lineCount == currentLine) {
      fmt = block.blockFormat();
      QColor bg = view->palette().highlight().color().light(175);
      fmt.setBackground(bg);

      highlight = QTextCursor(block);
      highlight.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
      highlight.setBlockFormat(fmt);

      break;
    }
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

    QToolTip::showText(helpEvent->globalPos(), word);
  }

  return false;
}

QTextDocument* TextEditWidget::getDocument() {
  return view->document();
}
