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
#include <ostream>
#include <QRect>
#include <QSize>
#include <QPoint>
#include <QString>
#include "util.h"

std::ostream &operator<<(std::ostream &stream, const QRect &rect)
{
    stream << rect.x() << ", "
      << rect.y() << ", "
      << rect.width() << ","
      << rect.height();
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const QPoint &point)
{
    stream << point.x() << ", " << point.y();
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const QSize &size)
{
    stream << size.width() << ", " << size.height();
    return stream;
}

QRect pointsToRect(const QPoint &p1, const QPoint &p2)
{
    int width = abs(p1.x() - p2.x());
    int height = abs(p1.y() - p2.y());
    int x = std::min(p1.x(), p2.x());
    int y = std::min(p1.y(), p2.y());

    return QRect(x, y, width, height);
}

QString qpointToString(const QPoint &point)
{
    return QString("%1, %2").arg(point.x()).arg(point.y());
}

QString qsizeToString(const QSize &size)
{
    return QString("%1 x %2").arg(size.width()).arg(size.height());
}

QString qrectToString(const QRect &rect)
{
    return QString("[%1, %2, %3, %4]").arg(rect.x()).arg(rect.y())
            .arg(rect.width()).arg(rect.height());
}


bool isComboBox(QObject *object)
{
    static const QString name("QComboBox");
    return (getClassName(object) == name);
}

bool isCheckBox(QObject *object)
{
    static const QString name("QCheckBox");
    return (getClassName(object) == name);
}

bool isGroupBox(QObject *object)
{
    static const QString name("QGroupBox");
    return (getClassName(object) == name);
}

bool isLabel(QObject *object)
{
    static const QString name("QLabel");
    return (getClassName(object) == name);
}


bool isPushButton(QObject *object)
{
    static const QString name("QPushButton");
    return (getClassName(object) == name);
}


bool isRadioButton(QObject *object)
{
    static const QString name("QRadioButton");
    return (getClassName(object) == name);
}


bool isLineEdit(QObject *object)
{
    static const QString name("QLineEdit");
    return (getClassName(object) == name);
}
