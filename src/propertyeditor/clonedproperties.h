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
#ifndef CLONEDPROPERTIES_H
#define CLONEDPROPERTIES_H

#include <QVariant>
#include <QMap>
#include <QString>
#include <QWidget>

class ClonedProperties
{
public:
    ClonedProperties(const QWidget *from);
    ClonedProperties(const ClonedProperties &other);
    void applyTo(QWidget *to);
private:
    QMap<QString, QVariant> properties;
};

#endif // CLONEDPROPERTIES_H
