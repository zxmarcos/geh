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
#ifndef COMBOITEMEDITOR_H
#define COMBOITEMEDITOR_H

#include <QDialog>

class QListView;
class QAbstractItemModel;
class QPushButton;
class QComboBox;

class ComboItemEditor : public QDialog
{
    Q_OBJECT
public:
    explicit ComboItemEditor(QWidget* parent = 0);

    void setComboBox(QComboBox* combo);
signals:

public slots:
    void insert();
    void remove();
private:
    QListView* m_list;
    QPushButton* m_insert;
    QPushButton* m_remove;
    QPushButton* m_close;
};

#endif // COMBOITEMEDITOR_H
