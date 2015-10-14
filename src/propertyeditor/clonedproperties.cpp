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
#include <iostream>
#include "clonedproperties.h"
#include "../designersurface/designersurface.h"

using namespace std;

ClonedProperties::ClonedProperties(const QWidget *from)
{
    const QStringList &genericProps = DesignerSurface::getGenericProperties();
    foreach (QString name, genericProps) {
        properties[name] = from->property(name.toUtf8().constData());
    }
}

ClonedProperties::ClonedProperties(const ClonedProperties &other)
{
    properties = other.properties;
}

void ClonedProperties::applyTo(QWidget *to)
{
    foreach (QString name, properties.keys()) {
        to->setProperty(name.toUtf8().constData(), properties[name]);
    }
}
