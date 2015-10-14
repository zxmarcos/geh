// =============================================================================
// Copyright (C) 2013    Marcos Medeiros
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
#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QDebug>
#include "tokenizer.h"

Tokenizer::Tokenizer()
{
    m_delimiters = QRegExp("(\\ |\\t|\\n|;)+");
    m_strings = QRegExp("\"(.*)\"");
    m_objetos = QRegExp("[a-zA-Z0-9]+");
    m_inBuffer = true;
}

void Tokenizer::setBuffer(const QString &str)
{
    m_tokens.clear();
    QString string(str);

    // remove os comentários
    string.replace(QRegExp("//[^\n]*"), "");
    m_tokens = string.split(m_delimiters);

    m_tokens.removeAll("");

    qDebug() << string;
}

int Tokenizer::getToken(bool consume)
{
    // está no buffer?
    if (m_inBuffer) {
        if (m_tokens.size() > 0) {
            m_currentToken = m_tokens.front();
            m_tokens.pop_front();
            if (m_strings.exactMatch(m_currentToken)) {
                m_currentType = STRING;
            } else if (m_currentToken == QString("selecionar")) {
                m_currentType = SELECT;
            } else if (m_currentToken == QString("escolher")) {
                m_currentType = CHOOSE;
            } else if (m_currentToken == QString("pressionar")) {
                m_currentType = PRESS;
            } else if (m_currentToken == QString("preencher")) {
                m_currentType = FILL;
            } else if (m_currentToken == QString("marcar")) {
                m_currentType = CHECK;
            } else if (m_currentToken == QString("desmarcar")) {
                m_currentType = UNCHECK;
            } else if (m_objetos.exactMatch(m_currentToken)) {
                m_currentType = OBJECT;
            } else {
                m_currentType = UNKNOWN;
            }
            if (consume) {
                m_inBuffer = true;
            } else {
                m_inBuffer = false;
            }
        } else {
            m_currentType = EOS;
            m_currentToken = "";
            return m_currentType;
        }
    }
    return m_currentType;
}

