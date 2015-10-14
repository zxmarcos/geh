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
#ifndef UTIL_H
#define UTIL_H

#include <ostream>
#include <QRect>
#include <QSize>
#include <QPoint>
#include <QWidget>
#include <QObject>
#include <QString>

std::ostream &operator<<(std::ostream &stream, const QRect &rect);
std::ostream &operator<<(std::ostream &stream, const QPoint &point);
std::ostream &operator<<(std::ostream &stream, const QSize &size);

QRect pointsToRect(const QPoint &p1, const QPoint &p2);
inline QString getClassName(QObject *object) {
   return object->metaObject()->className();
}

QString qpointToString(const QPoint &point);
QString qsizeToString(const QSize &size);
QString qrectToString(const QRect &rect);

bool isComboBox(QObject* object);
bool isCheckBox(QObject* object);
bool isGroupBox(QObject* object);
bool isLabel(QObject* object);
bool isPushButton(QObject* object);
bool isRadioButton(QObject* object);
bool isLineEdit(QObject* object);

#endif // UTIL_H
