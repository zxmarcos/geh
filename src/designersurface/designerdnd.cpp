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
#include "designercommands.h"
#include "mainwindow.h"
#include "widgetfactory.h"
#include "toolbox.h"
#include "util.h"

// o namespace dos comandos para a stack de reversão
namespace Cmd = DesignerSurfaceCommands;

// =============================================================================
// Evento recebido ao soltarem algo sobre o widget
// =============================================================================
void DesignerSurface::dropEvent(QDropEvent* event)
{
    EDIT_MODE_ONLY(event);
    // Só aceitamos nosso próprio tipo B)
    if (!event->mimeData()->hasFormat(WidgetFactory::mimeString())) {
        event->ignore();
        return;
    }

    // Se a requisição partiu da surface, então temos um movimento
    // para reparentear um widget
    if (event->source() == this) {
        dropAndReparent(event);
        return;
    }

    QWidget *widget = m_application->toolBox()->lastWidget();
    if (widget == nullptr) {
        event->ignore();
        return;
    }
    widget->setParent(m_lowerWidget);
    widget->setBackgroundRole(QPalette::Background);


    // Temos que habilitar o mousetracking em todos os widgets
    widget->setMouseTracking(true);

    const QSize size = widget->size();
    // posiciona o widget na posição correta
    QPoint gpos = mapToGlobal(event->pos() - QPoint(size.width() / 2,
                                                    size.height() / 2));

    widget->move(m_lowerWidget->mapFromGlobal(gpos));
    if (m_snapToGrid)
        widget->move(roundToGridSnap(widget->pos()));

    widget->show();

    // limpa a seleção e coloca o novo controle com selecionado
    m_currentWidget = widget;
    m_widgetSelection.clear();
    m_widgetSelection.push_back(widget);
    m_lowerWidget = this;

    event->accept();
    // redesenha tudo...
    update();

    emit selectedOneWidget(widget);

    Cmd::AddWidget *addWidgetCmd = new Cmd::AddWidget(this, widget);
    m_application->undoStack()->push(addWidgetCmd);

    setFocus();
}


// =============================================================================
//
// =============================================================================
void DesignerSurface::startReparentDrag()
{
    QPixmap pixmap =  m_currentWidget->grab();

    QMimeData* mimeData = new QMimeData;
    mimeData->setData(WidgetFactory::mimeString(), QByteArray());

    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));
    drag->setPixmap(pixmap);

//    m_currentWidget->hide();
    if (drag->exec(Qt::MoveAction) == Qt::MoveAction) {
        //qDebug() << "Moveu";
//        m_currentWidget->show();
    }
    m_mouseState = Normal;
}


// =============================================================================
//
// =============================================================================
void DesignerSurface::dropAndReparent(QDropEvent* event)
{
    QWidget* oldParent = m_currentWidget->parentWidget();
    QWidget* newParent = m_lowerWidget;

    event->acceptProposedAction();

    m_mouseState = Normal;
    m_widgetSelection.clear();
    m_widgetSelection.push_back(m_currentWidget);
    m_lowerWidget = this;

    // mesmo pai
    if (oldParent == newParent) {
        return;
    }

    QSize size = m_currentWidget->size();
    QPoint pos = event->pos() - QPoint(size.width() / 2, size.height() / 2);

    Cmd::ReparentWidget* cmd = new Cmd::ReparentWidget(this,
                                                       m_currentWidget,
                                                       oldParent,
                                                       newParent,
                                                       pos);
    m_application->undoStack()->push(cmd);
}


// =============================================================================
//
// =============================================================================
void DesignerSurface::dragEnterEvent(QDragEnterEvent* event)
{
    EDIT_MODE_ONLY(event);
    //cout << event->mimeData()->formats().at(0).toStdString() << endl;
    // só aceitamos nosso formato :P
    if (!event->mimeData()->hasFormat(WidgetFactory::mimeString())) {
        event->ignore();
        return;
    }
    event->accept();
}


// =============================================================================
//
// =============================================================================
void DesignerSurface::dragMoveEvent(QDragMoveEvent* event)
{
    EDIT_MODE_ONLY(event);

    m_lowerWidget = childAt(event->pos());
    if (!m_lowerWidget || !isContainerWidget(m_lowerWidget))
        m_lowerWidget = this;
    //event->acceptProposedAction();
    update();
}


// =============================================================================
// Inicia um evento de drag 'n drop interno
// =============================================================================
void DesignerSurface::dragFromSelection()
{

}
