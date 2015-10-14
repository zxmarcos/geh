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
#ifndef QBOOLEDIT_H
#define QBOOLEDIT_H

#include <QComboBox>

class QBoolEdit : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(bool m_value READ value WRITE setValue USER true)
public:
    explicit QBoolEdit(QWidget *parent = 0);

    bool value() const;
public slots:
    void setValue(bool value);
};

#endif // QBOOLEDIT_H
