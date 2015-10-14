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
#ifndef WIDGETFACTORY_H
#define WIDGETFACTORY_H

#include <QString>

class QWidget;

class WidgetFactory
{
public:
    WidgetFactory();
    static QWidget *createWidget(int id, QWidget *parent = 0);
    static QWidget *createWidget(QString name, QWidget *parent);
    static QString getDynamicClassName(QWidget *widget);

    static const QString mimeString() {
        return QString(QObject::tr("application/x-geh-designer-widget"));
    }

private:
    static int sm_labelCounter;
    static int sm_lineEditCounter;
    static int sm_checkBoxCounter;
    static int sm_comboBoxCounter;
    static int sm_pushButtonCounter;
    static int sm_radioButtonCounter;
    static int sm_groupBoxCounter;
    static int sm_unknownCounter;
public:
    static void resetNamingCounters();
};

#endif // WIDGETFACTORY_H
