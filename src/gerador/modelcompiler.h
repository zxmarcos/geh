// =============================================================================
// Copyright (C) 2015    Marcos Medeiros
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
#ifndef MODELCOMPILER_H
#define MODELCOMPILER_H

#include "sequencecompiler.h"
#include <QList>
#include <QWidget>

class ModelCompiler
{
public:
    ModelCompiler();

    void compile(const TaskSequence &cseq);
    QString result();
private:
    QString m_result;
    void emitOmission(SequenceNode& node);
    void emitRepeat(SequenceNode& node);
    void emitInversion(SequenceNode& node);
    void emitPerceptConfusion(SequenceNode& node);
    void emitReductionIntent(SequenceNode& node);
    void emitProlog();
    void emitForm();
    void emitFormLocs();
    void emitDM();
    void emitEpilog();
    void emitFill(SequenceNode& node);
    void emitPress(SequenceNode& node);
    void emitLookAt(QWidget *widget);
    void emitHandToMouse();
    void emitGehDevice();
    void emitGehCommon();
    void emitStartTask();
    void emitVisLocsChunks();
    void emitVisObjsChunks();
    QString generateStepPName();
    int m_currentStep;
    QList<QWidget*> m_widgets;
};

#endif // MODELCOMPILER_H
