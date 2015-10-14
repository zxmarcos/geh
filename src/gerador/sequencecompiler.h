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
#ifndef SEQUENCECOMPILER_H
#define SEQUENCECOMPILER_H

#include <QString>
#include <QList>
#include "tokenizer.h"

enum {
    HERR_NONE = 0,
    HERR_OMISSION,
    HERR_INVERSION,
    HERR_REPEAT,
    HERR_PERCEPT_CONFUSION,
    HERR_REDUCTION_INTENT,
};

struct SequenceNode {
    enum {
        INVALID = 0,
        SELECT,
        FILL,
        PRESS,
        CHOOSE,
        CHECK,
        UNCHECK,
    };
    int action;
    QString object;
    QString arg;
    int error;
    SequenceNode() : error(HERR_NONE) {}
    SequenceNode(const SequenceNode& rhs) {
        action = rhs.action;
        object = rhs.object;
        arg = rhs.arg;
        error = rhs.error;
    }

    QString describe();
};

typedef QList<SequenceNode> TaskSequence;

class SequenceCompiler
{
public:
    SequenceCompiler();

    void setProgram(const QString &program);
    TaskSequence compile();
    QStringList errors();
private:
    Tokenizer m_tokenizer;
    QStringList m_errors;
};

#endif // SEQUENCECOMPILER_H
