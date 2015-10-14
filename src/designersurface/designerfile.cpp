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
#include <QtXml>
#include "designersurface.h"
#include "widgetfactory.h"
#include "mainwindow.h"
#include "util.h"

// =============================================================================
// LÓGICA PARA SALVAR INTERFACES
// =============================================================================


// =============================================================================
// Escreve os items de um ComboBox no XML
// =============================================================================
static void writeComboItems(QComboBox* widget, QXmlStreamWriter& writer)
{
    QAbstractItemModel *model = widget->model();
    const int rows = model->rowCount();

    if (rows <= 0)
        return;

    writer.writeStartElement("items");

    for (int i = 0; i < rows; i++) {
        QModelIndex index = model->index(i, 0);
        writer.writeStartElement("item");
        writer.writeAttribute("name", model->data(index).toString());
        writer.writeEndElement();
    }

    writer.writeEndElement();
}


// =============================================================================
// Escreve o estado de um CheckBox no XML
// =============================================================================
static void writeCheckBoxStatus(QCheckBox* widget, QXmlStreamWriter& writer)
{
    writer.writeStartElement("checked");
    writer.writeAttribute("value", widget->isChecked() ? "true" : "false");
}

// =============================================================================
// Salva as propriedades genrenciadas de um objeto no arquivo XML
// =============================================================================
static void writeProperties(QWidget* object, QXmlStreamWriter& writer)
{
    const QString valueName("value");
    const QString typeName("type");

    writer.writeStartElement("properties");


    foreach (QString propName, DesignerSurface::getGenericProperties()) {
        QVariant property = object->property(propName.toStdString().c_str());

        writer.writeStartElement("property");
        writer.writeAttribute("name", propName);

        if ((!propName.compare(QString("text")) || (!propName.compare("title")))) {
            property = property.toString();
        }

        switch (property.type()) {
        case QVariant::String:
            writer.writeAttribute(typeName, "string");
            writer.writeAttribute(valueName, property.toString());
            break;
        case QVariant::Bool:
            writer.writeAttribute(typeName, "bool");
            writer.writeAttribute(valueName, property.toBool() ? "true" : "false");
            break;
        case QVariant::Int:
            writer.writeAttribute(typeName, "int");
            writer.writeAttribute(valueName, QString::number(property.toInt()));
            break;
        case QVariant::Size: {
            writer.writeAttribute(typeName, "size");
            QSize size = property.toSize();
            writer.writeAttribute("width", QString::number(size.width()));
            writer.writeAttribute("height", QString::number(size.height()));
            break;
        }
        case QVariant::Point: {
            writer.writeAttribute(typeName, "point");
            QPoint point = property.toPoint();
            writer.writeAttribute("x", QString::number(point.x()));
            writer.writeAttribute("y", QString::number(point.y()));
            break;
        }
        default:
            break;
        }
        writer.writeEndElement();
    }

    if (isComboBox(object)) {
        writeComboItems(static_cast<QComboBox*>(object), writer);
    } else if (isCheckBox(object)) {
        writeCheckBoxStatus(static_cast<QCheckBox*>(object), writer);
    }

    writer.writeEndElement();
}

// =============================================================================
// Salva os widgets recursivamente no arquivo XML
// =============================================================================
void DesignerSurface::saveWidgets(QWidget *object, QXmlStreamWriter& writer)
{
    const QObjectList &widgets = object->children();

    if (object == this) {
        writer.writeStartElement("designer-surface");
    } else {
        writer.writeStartElement("widget");
        writer.writeAttribute("class", getClassName(object));
    }

    writeProperties(object, writer);

    foreach (QObject* child, widgets) {
        // verifica se este filho é o redimensionador do formulário
        if (child == m_sizeGrip)
            continue;

        // só escrevemos os widgets que são filtrados
        if (isFilteredWidget(child))
            saveWidgets(qobject_cast<QWidget*>(child), writer);
    }

    writer.writeEndElement();
}


// =============================================================================
// Salva o documento atual em XML
// =============================================================================
bool DesignerSurface::saveDocument(const QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Erro", "Não foi possível criar o arquivo!");
        return false;
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();

    // salva os widgets recursivamente
    saveWidgets(this, writer);
    writer.writeEndDocument();

    return true;
}

// =============================================================================
// LÓGICA PARA CARREGAR INTERFACES
// =============================================================================

// =============================================================================
// Dado um objeto e um elemento da árvore DOM, aplica a property ao objeto
// =============================================================================
static void applyProperty(QWidget* object, const QDomElement& elem)
{
    const QString& type = elem.attribute("type");
    QVariant value;

    if (!type.compare("string")) {
        value = elem.attribute("value");

    } else if (!type.compare("bool")) {
        const QString t = elem.attribute("value");
        if (!t.compare("true", Qt::CaseInsensitive))
            value = true;
        else
            value = false;

    } else if (!type.compare("int")) {
        value = elem.attribute("value").toInt();

    } else if (!type.compare("point")) {
        QPoint point;
        point.setX(elem.attribute("x").toInt());
        point.setY(elem.attribute("y").toInt());
        value = point;

    } else if (!type.compare("size")) {
        QSize size;
        size.setWidth(elem.attribute("width").toInt());
        size.setHeight(elem.attribute("height").toInt());
        value = size;

    } else {
        qDebug() << "applyProperty:: tipo desconhecido!";
    }


    const char* name = elem.attribute("name").toStdString().c_str();
    object->setProperty(name, value);
}


// =============================================================================
// Insere um item diretamente em um ComboBox
// =============================================================================
static inline void insertComboItem(QComboBox* widget, QString name)
{
    QAbstractItemModel *model = widget->model();
    int row = model->rowCount();

    model->insertRow(row);
    QModelIndex index = model->index(row, 0);
    model->setData(index, name);
}


// =============================================================================
// Carrega os elementos de um ComboBox do XML
// =============================================================================
static void loadComboItems(QComboBox* widget, const QDomElement& element)
{
    QDomNode entry = element.firstChildElement("item");

    while (!entry.isNull()) {
        QDomElement el = entry.toElement();
        insertComboItem(widget, el.attribute("name"));
        entry = entry.nextSiblingElement();
    }
}


// =============================================================================
// Carrega o estado de um checkbox
// =============================================================================
static void loadCheckBoxStatus(QCheckBox* widget, const QDomElement& element)
{
    const QString t = element.attribute("value");
    bool value = false;
    if (!t.compare("true", Qt::CaseInsensitive))
        value = true;
    widget->setChecked(value);
}

// =============================================================================
// Carrega os widgets do XML
// =============================================================================
void DesignerSurface::loadWidgets(QWidget* parent, QDomElement& element)
{
    // lê as propriedades do widget atual
    QDomElement properties = element.firstChildElement("properties");
    QDomNode entry = properties.firstChildElement("property");

    while (!entry.isNull()) {
        QDomElement el = entry.toElement();

        if (!el.attribute("name").isEmpty())
            applyProperty(parent, el);

        entry = entry.nextSiblingElement();
    }

    // se for uma ComboBox, carrega os itens
    if (isComboBox(parent)) {
        loadComboItems(static_cast<QComboBox*>(parent),
                       properties.firstChildElement("items"));
    } else if (isCheckBox(parent)) {
        loadCheckBoxStatus(static_cast<QCheckBox*>(parent),
                       properties.firstChildElement("checked"));
    }


    // lê os widgets filhos...
    entry = element.firstChildElement("widget");
    while (!entry.isNull()) {
        QDomElement inner = entry.toElement();

        QWidget* widget = WidgetFactory::createWidget(inner.attribute("class"), parent);
        if (widget == nullptr) {
            qDebug() << "Falha ao criar o widget " << inner.attribute("class");
        } else {
            loadWidgets(widget, inner);
        }
        entry = entry.nextSiblingElement();
    }
}

// =============================================================================
// Carrega um documento
// =============================================================================
bool DesignerSurface::loadDocument(const QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erro", "Não foi possível abrir o arquivo!");
        return false;
    }

    QDomDocument dom;
    if (!dom.setContent(&file)) {
        QMessageBox::critical(this, "Erro", "Erro ao fazer o parse do arquivo!");
        return false;
    }

    QDomElement root = dom.documentElement();
    if (root.nodeName() != "designer-surface") {
        QMessageBox::critical(this, "Erro", "O arquivo não descreve um projeto do GEH!");
        return false;
    }

    closeDocument();

    loadWidgets(this, root);

    return true;
}

// =============================================================================
// Fecha o documento atual
// =============================================================================
void DesignerSurface::closeDocument()
{
    emit selectedNoneWidget(tr("Nenhum controle selecionado."));
    setUpdatesEnabled(false);
    const QObjectList& widgets = children();

    m_currentWidget = NULL;
    m_lowerWidget = this;
    m_widgetSelection.clear();

    foreach (QObject *child, widgets) {
        if (!child->isWidgetType())
            continue;
        if (child->objectName() == sm_dsSizeGripName)
            continue;
        // cout << child->objectName().toStdString() << ": delete" << endl;
        delete child;
    }

    setUpdatesEnabled(true);
    // TODO: verificar memory leak aqui!!!
    m_application->undoStack()->clear();
    WidgetFactory::resetNamingCounters();
}
