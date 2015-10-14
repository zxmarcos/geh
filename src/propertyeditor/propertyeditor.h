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
#ifndef PROPERTYEDITOR_H
#define PROPERTYEDITOR_H
/*
#include <QDialog>
#include <QListWidget>
#include <QTableWidget>
#include <QColor>
*/
#include <QtWidgets>
#include <QStringList>
#include <QHash>

class ComboItemEditor;

class PropertyEditor : public QTableWidget
{
    Q_OBJECT
public:
    explicit PropertyEditor(QWidget* parent = 0);
    ComboItemEditor *comboEditor();
protected:
signals:
    void widgetChanged(QWidget *widget);

public slots:
    void setWidget(QWidget* widget);
    void setMessage(QString message);
    void checkDeletedWidgets(const QList<QWidget*>& list);
    void widgetsModified(const QList<QWidget*>& list);
private:
    QWidget* m_currentWidget;
    ComboItemEditor* m_comboEditor;

    QColor m_baseColor;
    QColor m_alternateColor;

    void clear();

    const QStringList& m_genericProperties;
    QHash<QString, QString> m_propertyMap;
};



class BindPropertyItem : public QTableWidgetItem
{
public:
    BindPropertyItem();
    QVariant data(int role) const;
    void setData(int role, const QVariant& value);
    void setEditorData(const QVariant& value);
    void bind(QWidget* widget, const char* name, int index);
private:
    void updateWidgetProperty();
    QVariant m_data;
    QWidget* m_widget;
    const char* m_propertyName;
    int m_propertyIndex;
};


#endif // PROPERTYEDITOR_H
