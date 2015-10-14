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
#include <QtWidgets>
#include "taskbuilder.h"
#include "taskbuildersurface.h"
#include "mainwindow.h"
#include "highlighter.h"
#include "designersurface/designersurface.h"
#include "gerador/sequencecompiler.h"

TaskBuilder::TaskBuilder(MainWindow *parent) :
    QDialog(parent),
    m_app(parent)
{
    m_surface = new TaskBuilderSurface(this);
    m_editor = new QTextEdit();
    m_editor->setReadOnly(true);

    m_ldtHighlighter = new Highlighter(m_editor->document());

    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *upper = new QHBoxLayout();

    upper->addWidget(m_surface);
    upper->addWidget(m_editor);

    m_cancel = new QPushButton("Cancelar");
    m_generate = new QPushButton("Gerar");
    m_send = new QPushButton("Pronto");

    m_generate->setToolTip("Gera o código da tarefa através da sequência do editor.");
    m_send->setToolTip("Envia o código gerado para o editor principal e retorna.");

    connect(m_cancel, SIGNAL(clicked()), this, SLOT(accept()));
    QHBoxLayout *bottom = new QHBoxLayout();
    bottom->addWidget(m_generate);
    bottom->addWidget(m_cancel);
    QSpacerItem *spacer = new QSpacerItem(10, 10, QSizePolicy::Expanding);
    bottom->addSpacerItem(spacer);
    bottom->addWidget(m_send);

    layout->addLayout(upper);
    layout->addLayout(bottom);

    setLayout(layout);

    connect(m_surface, SIGNAL(doneCode(QString)), m_editor, SLOT(setText(QString)));
    connect(m_generate, SIGNAL(clicked()), m_surface, SLOT(build()));
    connect(m_send, SIGNAL(clicked()), this, SLOT(sendCode()));
    connect(m_send, SIGNAL(clicked()), this, SLOT(accept()));

    setWindowTitle("Editor visual de tarefas");
}


int TaskBuilder::exec()
{
    DesignerSurface *designer = m_app->designer();
    bool edit = designer->isEditMode();
    if (edit)
        designer->setEditMode(false);

    m_editor->clear();
    m_surface->setBackgroundWidget(designer);
    int ret = QDialog::exec();

    if (edit)
        designer->setEditMode(true);

    return ret;
}

void TaskBuilder::sendCode()
{
    m_app->editor()->setText(m_editor->toPlainText());
}
