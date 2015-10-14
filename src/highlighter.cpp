// =============================================================================
// Copyright (C) 2014    Marcos Medeiros
//
// This file is part of "SGEH - Sistema Gerador de Erro Humano"
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
// =============================================================================
#include "highlighter.h"

Highlighter::Highlighter(QTextDocument* parent) :
    QSyntaxHighlighter(parent)
{
    Rule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywords;
    keywords << "\\bselecionar\\b" << "\\bpressionar\\b" << "\\bmarcar\\b"
                    << "\\bdesmarcar\\b" << "\\bescolher\\b" << "\\bpreencher\\b";

    foreach (const QString& pattern, keywords) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        m_rules.append(rule);
    }


    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    m_rules.append(rule);


    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    m_rules.append(rule);

}


void Highlighter::highlightBlock(const QString& text)
{
    foreach (const Rule& rule, m_rules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}
