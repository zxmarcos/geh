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
#include "designersurface.h"
#include "designercommands.h"
#include "globalids.h"
#include "widgetfactory.h"

using namespace std;

namespace DesignerSurfaceCommands
{

// =============================================================================
// Redimensiona os widgets
// =============================================================================
Resize::Resize(DesignerSurface* ds,
               QList<QWidget*>& widgets,
               QMap<QWidget*, QSize>& old,
               QUndoCommand* parent)
    : QUndoCommand(QObject::tr("Redimensionar objetos"), parent),
      m_designer(ds),
      m_oldSize(old),
      m_widgetList(widgets)
{
    foreach (QWidget* widget, m_widgetList) {
        m_newSize[widget] = widget->size();
    }
}

// =============================================================================
//
// =============================================================================
Resize::~Resize()
{
    cout << "Destruindo resize" << endl;
}

// =============================================================================
//
// =============================================================================
void Resize::undo()
{
    cout << "Desfazendo redimensionar" << endl;
    foreach (QWidget* widget, m_widgetList) {

        const QSize& size = m_oldSize[widget];
        if (widget->size() != size)
            widget->resize(size);
    }
    m_designer->update();
}

// =============================================================================
//
// =============================================================================
void Resize::redo()
{
    cout << "Refazendo redimensionar" << endl;
    foreach (QWidget* widget, m_widgetList) {

        const QSize& size = m_newSize[widget];
        if (widget->size() != size)
            widget->resize(size);
    }
    m_designer->update();
}


// =============================================================================
// Move os widgets
// =============================================================================
Move::Move(DesignerSurface* ds,
           QList<QWidget*>& widgets,
           QMap<QWidget*, QPoint>& old,
           QUndoCommand* parent)
    : QUndoCommand(QObject::tr("Redimensionar objetos"), parent),
      m_designer(ds),
      m_oldPos(old), m_widgetList(widgets)
{
    // salva as novas posições
    foreach (QWidget* widget, m_widgetList) {
        m_newPos[widget] = widget->pos();
    }
}

// =============================================================================
//
// =============================================================================
Move::~Move()
{
    cout << "Destruindo move" << endl;
}

// =============================================================================
//
// =============================================================================
void Move::undo()
{
    cout << "Desfazendo mover" << endl;
    foreach (QWidget* widget, m_widgetList) {
        const QPoint& pos = m_oldPos[widget];
        if (widget->pos() != pos)
            widget->move(pos);
    }
    m_designer->update();
}

// =============================================================================
//
// =============================================================================
void Move::redo()
{
    cout << "Refazendo mover" << endl;
    foreach (QWidget* widget, m_widgetList) {
        const QPoint& pos = m_newPos[widget];
        if (widget->pos() != pos)
            widget->move(pos);
    }
    m_designer->update();
}


// =============================================================================
//
// =============================================================================
ResizeForm::ResizeForm(DesignerSurface* ds,
                       QSize oldSize,
                       QUndoCommand* parent)
    : QUndoCommand(QObject::tr("Redimensiona o formulário"), parent),
      m_designer(ds),
      m_originalSize(oldSize)
{
    m_newSize = m_designer->size();
}

// =============================================================================
//
// =============================================================================
bool ResizeForm::mergeWith(const QUndoCommand* command)
{
    if (command->id() != GEH_CMDID_RESIZEFORM)
        return false;
    m_newSize = m_designer->size();
    return true;
}

// =============================================================================
//
// =============================================================================
int ResizeForm::id() const
{
    return GEH_CMDID_RESIZEFORM;
}

// =============================================================================
//
// =============================================================================
void ResizeForm::undo()
{
    m_designer->resize(m_originalSize);
}

// =============================================================================
//
// =============================================================================
void ResizeForm::redo()
{
    m_designer->resize(m_newSize);
}


// =============================================================================
//
// =============================================================================
AddWidget::AddWidget(DesignerSurface* ds,
                     QWidget* newWidget,
                     QUndoCommand* parent)
    : QUndoCommand(parent),
      m_designer(ds),
      m_widget(newWidget)
{
    m_parent = newWidget->parentWidget();
}

// =============================================================================
//
// =============================================================================
AddWidget::~AddWidget()
{
    //cout << "Deletando widget" << endl;
}

// =============================================================================
//
// =============================================================================
void AddWidget::undo()
{
    //cout << "Desfazendo addWidget" << endl;
    m_widget->hide();
    m_widget->setParent(NULL);
    m_designer->update();
}

// =============================================================================
//
// =============================================================================
void AddWidget::redo()
{
    //cout << "Refazendo addWidget" << endl;
    m_widget->setParent(m_parent);
    m_widget->show();

    m_designer->update();
}

// =============================================================================
//
// =============================================================================
DeleteWidgets::DeleteWidgets(DesignerSurface* ds,
                             QList<QWidget*>& widgets,
                             QUndoCommand* parent)
    : QUndoCommand(parent),
      m_designer(ds),
      m_list(widgets)
{
    cout << "DeleteWidgets::ctor" << endl;
    foreach (QWidget* w, widgets) {
        m_info.push_back(ClonedWidget(w, w->parentWidget()));
    }
}

// =============================================================================
//
// =============================================================================
DeleteWidgets::~DeleteWidgets()
{
    cout << "DeleteWidgets::dtor()" << endl;
}

// =============================================================================
//
// =============================================================================
void DeleteWidgets::undo()
{
    cout << "DeleteWidgets::undo()" << endl;
    m_list.clear();
    foreach (ClonedWidget w, m_info) {
        QWidget* widget = WidgetFactory::createWidget(w.name, w.parent);
        w.properties.applyTo(widget);
        m_list.push_back(widget);
        cout << widget->metaObject()->className() << "::create" << endl;
    }
    m_designer->update();
}

// =============================================================================
//
// =============================================================================
void DeleteWidgets::redo()
{
    cout << "DeleteWidgets::redo()" << endl;
    m_designer->setUpdatesEnabled(false);
    foreach (QWidget* widget, m_list) {
        cout << widget->metaObject()->className() << "::delete" << endl;
        m_designer->remove(widget);
        delete widget;
    }
    m_list.clear();
    m_designer->setUpdatesEnabled(true);
    m_designer->update();
}


// =============================================================================
//
// =============================================================================
ReparentWidget::ReparentWidget(DesignerSurface* ds,
                               QWidget* widget,
                               QWidget* oldParent,
                               QWidget* newParent,
                               const QPoint& pos,
                               QUndoCommand* parent)
    : QUndoCommand(QObject::tr("Reparentear objeto"), parent),
      m_designer(ds),
      m_widget(widget),
      m_oldParent(oldParent),
      m_newParent(newParent),
      m_newPos(pos)
{
    m_oldPos = widget->mapTo(m_designer, QPoint(0, 0));
}

// =============================================================================
//
// =============================================================================
ReparentWidget::~ReparentWidget()
{
}

// =============================================================================
//
// =============================================================================
void ReparentWidget::undo()
{
    qDebug() << "Desfazendo reparentar";
    m_widget->setParent(m_oldParent);

    QPoint p = m_oldParent->mapFrom(m_designer, m_oldPos);
    m_widget->move(p);

    m_designer->update();
    m_widget->setVisible(true);
}

// =============================================================================
//
// =============================================================================
void ReparentWidget::redo()
{
    qDebug() << "Reparenteando widget";
    m_widget->setParent(m_newParent);

    QPoint p = m_newParent->mapFrom(m_designer, m_newPos);
    m_widget->move(p);

    m_designer->update();
    m_widget->setVisible(true);
}

};
