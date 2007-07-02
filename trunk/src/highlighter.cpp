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

#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
    HighlightingRule rule;

//     functionFormat.setFontWeight(QFont::Bold); //TODO Make this functional.
//     functionFormat.setForeground(Qt::darkRed);
//     rule.pattern = QRegExp("\\b([A-Za-z\\d_]*[:]{,2}[A-Za-z\\d_]+(?=[\\s\\S]*\\())"
//                            "((?=[\\s\\S]*\\)))"
//                            "((?![\\s\\S]*;))");
//     rule.format = functionFormat;
//     highlightingRules.append(rule);

    QStringList patterns;

    keywordFormat.setFontWeight(QFont::Bold);

    patterns << patterns << "\\bclass\\" << "\\basm\\b" << "\\bbreak\\b" <<
    "\\bcase\\b" << "\\bcatch\\b" << "\\bclass\\b" << "\\bconst_cast\\b" <<
    "\\bcontinue\\b" << "\\bdefault\\b" << "\\bdelete\\b" << "\\bdo\\b" <<
    "\\bdynamic_cast\\b" << "\\belse\\b" << "\\benum\\b" << "\\bexplicit\\b" <<
    "\\bexport\\b" << "\\bextern\\b" << "\\bfalse\\b" << "\\bfriend\\b" <<
    "\\bfor\\b" << "\\bgoto\\b" << "\\bif\\b" << "\\binline\\b" <<
    "\\bnamespace\\b" << "\\bnew\\b" << "\\boperator\\b" << "\\bprivate\\b" <<
    "\\bprotected\\b" << "\\bpublic\\b" << "\\bqobject_cast\\b" <<
    "\\breinterpret_cast\\b" << "\\breturn\\b" << "\\bsizeof\\b" <<
    "\\bstatic_cast\\b" << "\\bstruct\\b" << "\\bswitch\\b" <<
    "\\btemplate\\b" << "\\bthis\\b" << "\\bthrow\\b" << "\\btrue\\b" <<
    "\\btry\\b" << "\\btypedef\\b" << "\\btypeid\\b" << "\\btype_info\\b" <<
    "\\btypename\\b" << "\\bunion\\b" << "\\busing\\b" << "\\bvirtual\\b" <<
    "\\bwhile\\b" << "\\band\\b" << "\\band_eq\\b" << "\\bbad_cast\\b" <<
    "\\bbad_typeid\\b" << "\\bbitand\\b" << "\\bbitor\\b" << "\\bcompl\\b" <<
    "\\bnot\\b" << "\\bnot_eq\\b" << "\\bor\\b" << "\\bor_eq\\b" <<
    "\\bxor\\b" << "\\bxor_eq\\b" << "\\bexcept\\b" << "\\bfinally\\b" <<
    "\\bxalloc\\b" << "\\bconst\\b";

    foreach(QString pattern, patterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    patterns.clear();

    preprocessorFormat.setFontWeight(QFont::Bold);
    preprocessorFormat.setForeground(QColor(7 * 16, 8 * 16, 9 * 16));

    rule.pattern = QRegExp("#[a-zA-Z]+\\b");
    rule.format = preprocessorFormat;
    highlightingRules.append(rule);

    dataTypeFormat.setFontWeight(QFont::Bold);
    dataTypeFormat.setForeground(QColor(5 * 16 + 9, 7 * 16 + 7, 9 * 16 + 5));

    patterns << "\\bauto\\b" << "\\bbool\\b" << "\\bchar\\b" << "\\bconst\\b"
    << "\\bdouble\\b" << "\\bfloat\\b" << "\\bint\\b" << "\\blong\\b"
    << "\\bmutable\\b" << "\\bregister\\b" << "\\bshort\\b" << "\\bsigned\\b"
    << "\\bstatic\\b" << "\\bunsigned\\b" << "\\bvoid\\b" << "\\bvolatile\\b"
    << "\\buchar\\b" << "\\buint\\b" << "\\bint8_t\\b" << "\\bint16_t\\b"
    << "\\bint32_t\\b" << "\\bint64_t\\b" << "\\buint8_t\\b" << "\\buint16_t\\b"
    << "\\buint32_t\\b" << "\\buint64_t\\b" << "\\bwchar_t\\b";

    foreach(QString pattern, patterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = dataTypeFormat;
        highlightingRules.append(rule);
    }

    //To colour things like Q_INIT_RESOURCE(application)
    qMacroFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\bQ_[A-Za-z_]+");
    rule.format = qMacroFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(QColor(11 * 16 + 11, 8 * 16 + 7, 4 * 16 + 5));
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    numberFormat.setForeground(Qt::darkBlue);
    rule.pattern = QRegExp("[0-9]+");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::darkGray);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::gray);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

void Highlighter::highlightBlock(const QString &text) {
    foreach (HighlightingRule rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = text.indexOf(expression);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = text.indexOf(expression, index + length);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        int endIndex = text.indexOf(commentEndExpression, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
