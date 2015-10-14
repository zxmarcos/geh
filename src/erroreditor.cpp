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
#include "erroreditor.h"
#include "ui_erroreditor.h"
#include <QDebug>
#include <QComboBox>

ErrorEditor::ErrorEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorEditor)
{
    ui->setupUi(this);
    setWindowTitle("Selecionar erros...");
}

ErrorEditor::~ErrorEditor()
{
    delete ui;
}

static QComboBox *createComboBox(QWidget *parent) {
    QComboBox *cb = new QComboBox(parent);
    cb->addItem("Nenhum", HERR_NONE);
    cb->addItem("Omissão", HERR_OMISSION);
    cb->addItem("Inversão", HERR_INVERSION);
    cb->addItem("Repetição", HERR_REPEAT);
    cb->addItem("Confusão Perceptiva", HERR_PERCEPT_CONFUSION);
    cb->addItem("Redução de Intencionalidade", HERR_REDUCTION_INTENT);
    return cb;
}

int ErrorEditor::exec(TaskSequence *seq)
{
    while (ui->treeView->topLevelItemCount() > 0) {
        delete ui->treeView->takeTopLevelItem(0);
    }
    QVector<QComboBox*> combos(seq->size());

    do {
        int i = 0;
        foreach (SequenceNode node, *seq) {
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0, node.describe());

            QComboBox *cb = createComboBox(this);
            ui->treeView->addTopLevelItem(item);
            ui->treeView->setItemWidget(item, 1, cb);
            combos[i] = cb;
            i++;
        }
    } while (0);

    QDialog::exec();
    for (int i = 0; i < combos.size(); i++) {
        QComboBox *cb = combos[i];
        SequenceNode& node = (*seq)[i];
        node.error = cb->currentIndex();
    }
}
