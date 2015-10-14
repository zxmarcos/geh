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
#include "qstringlistedit.h"

QStringListEdit::QStringListEdit(QWidget *parent) :
    QPushButton(parent)
{
    setText(tr("Editar"));
    m_editor = new QStringListEditDialog(this);

    connect(this, SIGNAL(pressed()), m_editor, SLOT(show()));
}

QStringList QStringListEdit::list() const
{
    return m_list;
}

void QStringListEdit::setList(const QStringList &list)
{
    m_list = list;
}

QStringListEditDialog::QStringListEditDialog(QWidget *parent) :
    QDialog(parent)
{
    m_textEdit = new QTextEdit;
    m_ok = new QPushButton(tr("Ok"));
    m_cancel = new QPushButton(tr("Cancelar"));

    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *buttons = new QHBoxLayout;
    buttons->setAlignment(Qt::AlignRight);

    layout->addWidget(m_textEdit);
    buttons->addWidget(m_cancel);
    buttons->addWidget(m_ok);

    layout->addLayout(buttons);

    connect(m_ok, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_cancel, SIGNAL(clicked()), this, SLOT(close()));
}
