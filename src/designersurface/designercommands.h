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
#ifndef DESIGNERCOMMANDS_H
#define DESIGNERCOMMANDS_H
#include <iostream>
using namespace std;
#include <QUndoCommand>
#include <QWidget>
#include <QMap>
#include <QPoint>
#include <QSize>
#include <QList>
#include "../propertyeditor/clonedproperties.h"
#include "../util.h"
#include "../widgetfactory.h"

class DesignerSurface;

namespace DesignerSurfaceCommands
{

class Resize : public QUndoCommand
{
public:
    Resize(DesignerSurface *ds, QList<QWidget*> &widgets,
           QMap<QWidget*, QSize> &old, QUndoCommand *parent = 0);
    ~Resize();

    void undo();
    void redo();
private:
    DesignerSurface *m_designer;
    QMap<QWidget*, QSize> m_oldSize;
    QMap<QWidget*, QSize> m_newSize;
    QList<QWidget*> m_widgetList;
};

class Move : public QUndoCommand
{
public:
    Move(DesignerSurface *ds, QList<QWidget*> &widgets,
         QMap<QWidget*, QPoint> &old, QUndoCommand *parent = 0);
    ~Move();

    void undo();
    void redo();
private:
    DesignerSurface *m_designer;
    QMap<QWidget*, QPoint> m_oldPos;
    QMap<QWidget*, QPoint> m_newPos;
    QList<QWidget*> m_widgetList;
};

class ResizeForm : public QUndoCommand
{
public:
    ResizeForm(DesignerSurface *ds, QSize oldSize, QUndoCommand *parent = 0);
    bool mergeWith(const QUndoCommand *command);
    int id() const;
    void undo();
    void redo();
private:
    DesignerSurface *m_designer;
    QSize m_newSize;
    QSize m_originalSize;
};

class AddWidget : public QUndoCommand
{
public:
    AddWidget(DesignerSurface *ds, QWidget *newWidget, QUndoCommand *parent = 0);
    ~AddWidget();

    void undo();
    void redo();
private:
    DesignerSurface *m_designer;
    QWidget *m_widget;
    QWidget *m_parent;
};

class DeleteWidgets : public QUndoCommand
{
public:
    DeleteWidgets(DesignerSurface *ds, QList<QWidget*> &widgets, QUndoCommand *parent = 0);
    ~DeleteWidgets();

    void undo();
    void redo();
private:
    struct ClonedWidget {
        ClonedProperties properties;
        QWidget *parent;
        QString name;
        ClonedWidget(QWidget *from, QWidget *parent) :
            properties(ClonedProperties(from)),
            parent(parent)
        {
            name = WidgetFactory::getDynamicClassName(from);
            cout << "Name: " << name.toStdString() << endl;
        }
    };
    DesignerSurface *m_designer;
    QList<ClonedWidget> m_info;
    QList<QWidget*> m_list;
};

class ReparentWidget : public QUndoCommand
{
public:
    ReparentWidget(DesignerSurface *ds, QWidget *widget,
                  QWidget *oldParent, QWidget *newParent,
                   const QPoint& pos,
                  QUndoCommand *parent = 0);
    ~ReparentWidget();
    void undo();
    void redo();
private:
    DesignerSurface *m_designer;
    QWidget *m_widget;
    QWidget *m_oldParent;
    QWidget *m_newParent;
    QPoint m_newPos;
    QPoint m_oldPos;
};

}

#endif // DESIGNERCOMMANDS_H
