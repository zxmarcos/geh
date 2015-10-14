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
#include <QtWidgets>
#include "comboitemeditor.h"

ComboItemEditor::ComboItemEditor(QWidget* parent) :
    QDialog(parent)
{
    setModal(true);

    QVBoxLayout *layout = new QVBoxLayout();

    m_list = new QListView(this);
    m_close = new QPushButton(tr("Fechar"));
    m_insert = new QPushButton(tr("Inserir"));
    m_remove = new QPushButton(tr("Remover"));

    QHBoxLayout *bottom = new QHBoxLayout();
    bottom->addWidget(m_insert);
    bottom->addWidget(m_remove);
    bottom->addWidget(m_close);

    layout->addWidget(m_list);
    layout->addLayout(bottom);

    connect(m_close, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_insert, SIGNAL(clicked()), this, SLOT(insert()));
    connect(m_remove, SIGNAL(clicked()), this, SLOT(remove()));

    setLayout(layout);
}

void ComboItemEditor::setComboBox(QComboBox *combo)
{
    m_list->setModel(combo->model());
}

void ComboItemEditor::insert()
{
    QAbstractItemModel *model = m_list->model();

    int row = model->rowCount();

    model->insertRow(row);
    QModelIndex index = model->index(row, 0);

    model->setData(index, "Novo");

    m_list->setCurrentIndex(index);
    m_list->edit(index);
}

void ComboItemEditor::remove()
{
    QAbstractItemModel *model = m_list->model();
    QModelIndex index = m_list->currentIndex();
    model->removeRow(index.row());
}
