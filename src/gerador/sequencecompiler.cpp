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
#include "sequencecompiler.h"
#include <QDebug>

/*
Neste módulo compilamos nossa linguagem para definir as tarefas,
Botão:
    EVENTOS:
        pressionar

EntradaTexto:
    EVENTOS:
        preencher

CaixaChecagem:
    EVENTOS:
        checar
        deschecar

BotãoRadio:
    EVENTOS:
        selecionar

CaixaSeleção
    EVENTOS:
        escolher
*/

SequenceCompiler::SequenceCompiler()
{
}

void SequenceCompiler::setProgram(const QString &program)
{
    m_errors.clear();
    m_tokenizer.setBuffer(program);
}

TaskSequence SequenceCompiler::compile()
{
    TaskSequence sequence;
    m_errors.clear();

    while (m_tokenizer.getToken(true)) {
        //        qDebug() << "Token:" << m_tokenizer.token() << m_tokenizer.type();
        if (m_tokenizer.type() == Tokenizer::UNKNOWN) {
            //            qDebug() << "Erro!";
            continue;
        }

        if (m_tokenizer.type() == Tokenizer::SELECT) {
            m_tokenizer.consume();
            qDebug() <<"SELECIONAR";
            if (m_tokenizer.getToken() == Tokenizer::OBJECT) {
                qDebug() << "OBJETO";
                m_tokenizer.consume();
                SequenceNode node;
                node.action = SequenceNode::SELECT;
                node.object = m_tokenizer.token();
                sequence.push_back(node);
            } else {
                m_errors << "Esperado nome do objeto na cláusula 'selecionar'";
            }
        } else if (m_tokenizer.type() == Tokenizer::FILL) {
            m_tokenizer.consume();
            if (m_tokenizer.getToken() == Tokenizer::OBJECT) {
                m_tokenizer.consume();
                QString obj = m_tokenizer.token();
                if (m_tokenizer.getToken() == Tokenizer::STRING) {
                    m_tokenizer.consume();
                    SequenceNode node;
                    node.action = SequenceNode::FILL;
                    node.object = obj;
                    node.arg = m_tokenizer.token();
                    sequence.push_back(node);
                } else {
                    m_errors << "Esperado texto depois do comando 'preencher'";
                }
            } else {
                m_errors << "Esperado nome do objeto na cláusula 'preencher'";
            }
        } else if (m_tokenizer.type() == Tokenizer::PRESS) {
            m_tokenizer.consume();
            if (m_tokenizer.getToken() == Tokenizer::OBJECT) {
                m_tokenizer.consume();
                SequenceNode node;
                node.action = SequenceNode::PRESS;
                node.object = m_tokenizer.token();
                sequence.push_back(node);
            } else {
                m_errors << "Esperado nome do objeto na cláusula 'pressionar'";
            }
        } else if (m_tokenizer.type() == Tokenizer::CHECK) {
            m_tokenizer.consume();
            if (m_tokenizer.getToken() == Tokenizer::OBJECT) {
                m_tokenizer.consume();
                SequenceNode node;
                node.action = SequenceNode::CHECK;
                node.object = m_tokenizer.token();
                sequence.push_back(node);
            } else {
                m_errors << "Esperado nome do objeto na cláusula 'marcar'";
            }
        } else if (m_tokenizer.type() == Tokenizer::UNCHECK) {
            m_tokenizer.consume();
            if (m_tokenizer.getToken() == Tokenizer::OBJECT) {
                m_tokenizer.consume();
                SequenceNode node;
                node.action = SequenceNode::UNCHECK;
                node.object = m_tokenizer.token();
                sequence.push_back(node);
            } else {
                m_errors << "Esperado nome do objeto na cláusula 'desmarcar'";
            }
        } else if (m_tokenizer.type() == Tokenizer::CHOOSE) {
            m_tokenizer.consume();
            if (m_tokenizer.getToken() == Tokenizer::STRING) {
                m_tokenizer.consume();
                QString name = m_tokenizer.token();
                if (m_tokenizer.getToken() == Tokenizer::OBJECT) {
                    SequenceNode node;
                    node.action = SequenceNode::CHOOSE;
                    node.object = m_tokenizer.token();
                    node.arg = name;
                    sequence.push_back(node);
                } else {
                    m_errors << "Esperado nome do objeto na cláusula 'escolher'";
                }
            } else {
                m_errors << "Esperado nome do campo na cláusula 'escolher'";
            }
        } else {
            m_errors << m_tokenizer.token() + " não definido!";
            m_tokenizer.consume();
        }
    }

    return sequence;
}

QStringList SequenceCompiler::errors()
{
    return m_errors;
}


QString SequenceNode::describe()
{
    switch (action) {
    case INVALID: {
        return QString("passo inválido");
    }
    case SELECT: {
        return QString("selecionar %0").arg(object);
    }
    case FILL: {
        return QString("preencher %0 com \"%1\"").arg(object).arg(arg);
    }
    case PRESS: {
        return QString("pressionar %0").arg(object);
    }
    case CHOOSE: {
        return QString("escolher %0").arg(object);
    }
    case CHECK: {
        return QString("marcar %0").arg(object);
    }
    case UNCHECK: {
        return QString("desmarcar %0").arg(object);
    }
    }
}
