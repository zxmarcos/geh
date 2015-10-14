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
#include <iostream>
#include "globalids.h"
#include "widgetfactory.h"

using namespace std;

// contadores
int WidgetFactory::sm_labelCounter = 0;
int WidgetFactory::sm_lineEditCounter = 0;
int WidgetFactory::sm_checkBoxCounter = 0;
int WidgetFactory::sm_comboBoxCounter = 0;
int WidgetFactory::sm_pushButtonCounter = 0;
int WidgetFactory::sm_radioButtonCounter = 0;
int WidgetFactory::sm_groupBoxCounter = 0;
int WidgetFactory::sm_unknownCounter = 0;

WidgetFactory::WidgetFactory()
{
}

// =============================================================================
// Cria widgets pelo Id
// =============================================================================
QWidget *WidgetFactory::createWidget(int id, QWidget *parent)
{
    QWidget *widget = NULL;
    QString name;

    switch (id) {
    case GEH_CTRL_BUTTON: {
        widget = new QPushButton(QObject::tr("Botão"), parent);
        name = QString("botao%1").arg(sm_pushButtonCounter++);
        break;
    }

    case GEH_CTRL_LABEL: {
        widget = new QLabel(QObject::tr("Label"), parent);
        name = QString("label%1").arg(sm_labelCounter++);
        break;
    }

    case GEH_CTRL_EDITBOX: {
        widget = new QLineEdit(QObject::tr(""), parent);
        name = QString("campo%1").arg(sm_lineEditCounter++);
        break;
    }

    case GEH_CTRL_CHECKBOX: {
        widget = new QCheckBox(QObject::tr("Caixa de Checagem"), parent);
        name = QString("checagem%1").arg(sm_checkBoxCounter++);
        break;
    }

    case GEH_CTRL_RADIOBUTTON: {
        widget = new QRadioButton(QObject::tr("Botão Radio"), parent);
        name = QString("radio%1").arg(sm_radioButtonCounter++);
        break;
    }

    case GEH_CTRL_GROUPBOX: {
        QGroupBox *box = new QGroupBox(QObject::tr("Caixa de Grupo"), parent);
        // Ajusta o tamanho da caixa :P
        box->resize(QSize(150, 50));
        widget = box;
        name = QString("grupo%1").arg(sm_groupBoxCounter++);
        break;
    }

    case GEH_CTRL_COMBOBOX: {
        widget = new QComboBox(parent);
        name = QString("combo%1").arg(sm_comboBoxCounter++);
        break;
    }

    default: {
        widget = new QWidget(parent);
        name = QString("widget%1").arg(sm_unknownCounter++);
        break;
    }

    }
    widget->setObjectName(name);
    return widget;
}

// =============================================================================
// Cria um widget pelo nome da classe
// =============================================================================
QWidget *WidgetFactory::createWidget(QString name, QWidget *parent)
{
    QWidget *widget = NULL;

    if (name == QString("QPushButton")) {
        widget = new QPushButton(QObject::tr("Botão"), parent);
    } else if (name == QString("QLabel")) {
        widget = new QLabel(QObject::tr("Label"), parent);
    } else if (name == QString("QLineEdit")) {
        widget = new QLineEdit(QObject::tr("Caixa de Texto"), parent);
    } else if (name == QString("QCheckBox")) {
        widget = new QCheckBox(QObject::tr("Caixa de Checagem"), parent);
    } else if (name == QString("QRadioButton")) {
        widget = new QRadioButton(QObject::tr("Botão Radio"), parent);
    } else if (name == QString("QComboBox")) {
        widget = new QComboBox(parent);
    } else if (name == QString("QGroupBox")) {
        QGroupBox *box = new QGroupBox(QObject::tr("Caixa de Grupo"), parent);
        // Ajusta o tamanho da caixa :P
        box->resize(QSize(150, 50));
        widget = box;
    } else {
        cout << "Falha ao encontrar tipo de widget" << name.toStdString() << endl;
        widget = new QWidget(parent);
    }
    return widget;
}

// =============================================================================
// Verifica em tempo de execução qual o tipo de widget e retorna o nome
// =============================================================================
QString WidgetFactory::getDynamicClassName(QWidget *widget)
{
    QWidget *casted = NULL;
    if ((casted = qobject_cast<QPushButton*>(widget)))
        return "QPushButton";
    if ((casted = qobject_cast<QLineEdit*>(widget)))
        return "QLineEdit";
    if ((casted = qobject_cast<QLabel*>(widget)))
        return "QLabel";
    if ((casted = qobject_cast<QCheckBox*>(widget)))
        return "QCheckBox";
    if ((casted = qobject_cast<QRadioButton*>(widget)))
        return "QRadioButton";
    if ((casted = qobject_cast<QGroupBox*>(widget)))
        return "QGroupBox";
    if ((casted = qobject_cast<QComboBox*>(widget)))
        return "QComboBox";
    return "QWidget";
}


// =============================================================================
// Reinicia a contagem de nomes
// =============================================================================
void WidgetFactory::resetNamingCounters()
{
    sm_labelCounter = 0;
    sm_lineEditCounter = 0;
    sm_checkBoxCounter = 0;
    sm_comboBoxCounter = 0;
    sm_pushButtonCounter = 0;
    sm_radioButtonCounter = 0;
    sm_groupBoxCounter = 0;
}
