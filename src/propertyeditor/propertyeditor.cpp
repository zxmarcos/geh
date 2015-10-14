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
#include <ctime>
#include "propertyeditor.h"
#include "qpointedit.h"
#include "qsizeedit.h"
#include "qrectedit.h"
#include "qstringlistedit.h"
#include "qbooledit.h"
#include "util.h"
#include "designersurface/designersurface.h"
#include "comboitemeditor.h"

using namespace std;

// =============================================================================
// Editor de propriedades
// =============================================================================
PropertyEditor::PropertyEditor(QWidget *parent) :
    QTableWidget(parent),
    m_genericProperties(DesignerSurface::getGenericProperties())
{
    QItemEditorFactory *factory = new QItemEditorFactory;

    QItemEditorCreatorBase *pointEditCreator =
        new QStandardItemEditorCreator<QPointEdit>();
    QItemEditorCreatorBase *lineEditCreator =
        new QStandardItemEditorCreator<QLineEdit>();
    QItemEditorCreatorBase *sizeEditCreator =
        new QStandardItemEditorCreator<QSizeEdit>();
    QItemEditorCreatorBase *rectEditCreator =
        new QStandardItemEditorCreator<QRectEdit>();
    QItemEditorCreatorBase *boolEditCreator =
        new QStandardItemEditorCreator<QBoolEdit>();

    factory->registerEditor(QVariant::Point, pointEditCreator);
    factory->registerEditor(QVariant::String, lineEditCreator);
    factory->registerEditor(QVariant::Size, sizeEditCreator);
    factory->registerEditor(QVariant::Rect, rectEditCreator);
    factory->registerEditor(QVariant::Bool, boolEditCreator);

    QItemEditorFactory::setDefaultFactory(factory);

    m_comboEditor = new ComboItemEditor(this);

    m_currentWidget = NULL;
    verticalHeader()->setVisible(false);
    setSelectionBehavior(SelectRows);
    setSelectionMode(SingleSelection);

    m_alternateColor = QColor(200, 255, 200);
    m_baseColor = QColor(240, 244, 210);

    m_propertyMap["objectName"] = "Name";
    m_propertyMap["enabled"] = "Enabled";
    m_propertyMap["pos"] = "Position";
    m_propertyMap["size"] = "Size";
    m_propertyMap["visible"] = "Visible";
    m_propertyMap["toolTip"] = "Tooltip";
    m_propertyMap["text"] = "Text";
    m_propertyMap["title"] = "Title";

}


// =============================================================================
// Retorna o editor de combobox
// =============================================================================
ComboItemEditor *PropertyEditor::comboEditor()
{
    return m_comboEditor;
}

// =============================================================================
// Define o widget para listar as propriedades
// =============================================================================
void PropertyEditor::setWidget(QWidget *widget)
{
    // se for o mesmo widget não fazemos nada
    if (m_currentWidget == widget)
        return;

    horizontalHeader()->setStretchLastSection(false);

    // altera o widget e emite o sinal
    m_currentWidget = widget;
    emit widgetChanged(widget);

    clear();
    setEnabled(true);
    horizontalHeader()->setVisible(true);
    setColumnCount(2);
    setHorizontalHeaderLabels(QStringList() << tr("Property") <<
                              tr("Value"));

    const QMetaObject *meta = widget->metaObject();
    int propCount = 0;

    // primeiro vamos procurar quantas propriedades são editáveis
    for (int idx = 0; idx < meta->propertyCount(); idx++) {
        QMetaProperty property = meta->property(idx);
        const char *cstrName = property.name();
        if (!m_genericProperties.contains(cstrName))
            continue;
        propCount++;
    }

    setRowCount(propCount);

    // Percorre a lista de propriedades
    for (int idx = 0, propCount = 0; idx < meta->propertyCount(); idx++) {

        QMetaProperty property = meta->property(idx);
        const char *const cstrName = property.name();

        if (!m_genericProperties.contains(cstrName))
            continue;

        QString s = m_propertyMap.value(cstrName);
        QTableWidgetItem *nameItem = new QTableWidgetItem(s);
        BindPropertyItem *valueItem = new BindPropertyItem;

        // Deixa que o Qt tome conta da edição dos valores
        //valueItem->setData(Qt::EditRole, m_currentWidget->property(cstrName));
        valueItem->bind(m_currentWidget, cstrName, idx);
        valueItem->setEditorData(m_currentWidget->property(cstrName));

        // o nome não é edtitável
        nameItem->setFlags(nameItem->flags() ^ Qt::ItemIsEditable);

        if (propCount & 1) {
            nameItem->setBackground(m_alternateColor);
            valueItem->setBackground(m_alternateColor);
        } else {
            nameItem->setBackground(m_baseColor);
            valueItem->setBackground(m_baseColor);
        }
        setItem(propCount, 0, nameItem);
        setItem(propCount, 1, valueItem);
        propCount++;
    }

    // redimensiona as linhas e colunas
    resizeColumnToContents(0);
    resizeRowsToContents();
}

// =============================================================================
// Exibe somente uma mensagem no editor
// =============================================================================
void PropertyEditor::setMessage(QString message)
{
    m_currentWidget = NULL;
    clear();
    setEnabled(false);
    horizontalHeader()->setVisible(false);
    setColumnCount(1);
    setRowCount(1);
    horizontalHeader()->setStretchLastSection(true);
    QTableWidgetItem *msgItem = new QTableWidgetItem(message);
    setItem(0, 0, msgItem);
}

// =============================================================================
// Limpa o editor de propriedades
// =============================================================================
void PropertyEditor::clear()
{
    while (rowCount() > 0) {
        removeRow(0);
    }
}

void PropertyEditor::checkDeletedWidgets(const QList<QWidget*> &list)
{
    if (list.contains(m_currentWidget)) {
        setMessage(tr(""));
    }
}

void PropertyEditor::widgetsModified(const QList<QWidget*> &list)
{
    if (list.contains(m_currentWidget)) {
        QWidget *w = m_currentWidget;
        m_currentWidget = NULL;
        setWidget(w);
    }
}

// =============================================================================
// Item utilizado para fazer a ligação do valor de uma propriedade do editor
// ao widget original.
// =============================================================================
BindPropertyItem::BindPropertyItem()
    : QTableWidgetItem()
{
    m_propertyIndex = 0;
    m_propertyName = NULL;
    m_data = QVariant();
}

// =============================================================================
// Define os dados do item
// =============================================================================
void BindPropertyItem::setData(int role, const QVariant &value)
{
    if (role == Qt::EditRole) {
        switch (value.type()) {
        case QVariant::Point:
        case QVariant::Size:
        case QVariant::Rect:
        case QVariant::String:
        case QVariant::Bool:
            m_data = value;
            updateWidgetProperty();
            return;
        default:
            break;
        }
    }
    QTableWidgetItem::setData(role, value);
}

// =============================================================================
// Retorna dados do item
// =============================================================================
QVariant BindPropertyItem::data(int role) const
{
    if (role == Qt::DisplayRole) {
        switch (m_data.type()) {
        case QVariant::Point:
            return qpointToString(m_data.toPoint());

        case QVariant::Size:
            return qsizeToString(m_data.toSize());

        case QVariant::Rect:
            return qrectToString(m_data.toRect());

        case QVariant::String:
            return m_data.toString();

        case QVariant::Bool:
            if (m_data.toBool())
                return QString("Yes");
            return QString("No");

        default:
            break;
        }
    } else if (role == Qt::EditRole) {
        return m_data;
    }
    return QTableWidgetItem::data(role);
}

// =============================================================================
// Define o dado original do editor
// =============================================================================
void BindPropertyItem::setEditorData(const QVariant &value)
{
    m_data = value;
    QTableWidgetItem::setData(Qt::EditRole, value);
    updateWidgetProperty();
}

// =============================================================================
// Faz a ligação desse item com alguma propriedade de um QWidget
// =============================================================================
void BindPropertyItem::bind(QWidget *widget, const char *name, int index)
{
    m_propertyIndex = index;
    m_widget = widget;
    m_propertyName = name;
}

// =============================================================================
// Atualiza as propriedades
// =============================================================================
void BindPropertyItem::updateWidgetProperty()
{
    if (m_propertyName) {
        QVariant current = m_widget->property(m_propertyName);
        // apenas atualizamos se os dados são diferentes
        if (current != m_data) {
            m_widget->setProperty(m_propertyName, m_data);

            QWidget *parent = qobject_cast<QWidget *>(m_widget->parent());
            if (parent)
                parent->update();
        }
    }
}


