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
#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <QString>
#include <QStringList>
#include <QRegExp>

class Tokenizer
{
public:
    Tokenizer();
    void setBuffer(const QString &str);
    int getToken(bool consume=false);

    enum {
        EOS = 0,
        SELECT,
        FILL,
        PRESS,
        CHOOSE,
        CHECK,
        UNCHECK,
        OBJECT,
        STRING,
        UNKNOWN,
    };
private:
    QRegExp m_delimiters;
    QRegExp m_strings;
    QRegExp m_objetos;
    QString m_currentToken;
    int m_currentType;
    bool m_inBuffer;
    QStringList m_tokens;
    QString unquote(const QString& str) {
        int idx = m_strings.indexIn(str);
        if (idx > -1)
            return m_strings.cap(1);
        return str;
    }

public:
    void consume() {
        m_inBuffer = true;
    }

    QString token() {
        if (type() == STRING)
            return unquote(m_currentToken);
        return m_currentToken;
    }
    int type() const {
        return m_currentType;
    }
};

#endif // TOKENIZER_H
