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
#include "designersurface.h"
#include "designercommands.h"
#include "mainwindow.h"
#include "util.h"

// o namespace dos comandos para a stack de reversão
namespace Cmd = DesignerSurfaceCommands;

// =============================================================================
// Move a seleção de widgets
// =============================================================================
void DesignerSurface::moveSelection(const QPoint& delta)
{
    foreach (QWidget* widget, m_widgetSelection) {
        widget->move(widget->pos() + delta);
    }
}

// =============================================================================
// Redimensiona a seleção de widgets
// =============================================================================
void DesignerSurface::resizeSelection(const QPoint& delta)
{
    foreach (QWidget* widget, m_widgetSelection) {
        widget->resize(widget->size() + QSize(delta.x(), delta.y()));
    }
}


// =============================================================================
// Encontra os widgets selecionados
// =============================================================================
void DesignerSurface::findSelection()
{
    const QObjectList& widgets = children();

    m_widgetSelection.clear();

    // cria o retângulo da seleção
    QRect selectionRect = QRect(m_startPos.toPoint(), m_endPos.toPoint());

    foreach (QObject* object, widgets) {

        // verifica se o objeto é do tipo QWidget
        if (object->objectName() == sm_dsSizeGripName)
            continue;
        if (object->isWidgetType()) {
            QWidget* widget = qobject_cast<QWidget*>(object);

            // verifica se as caixas se cruzam
            if (selectionRect.intersects(widget->geometry())) {
                m_widgetSelection.push_back(widget);
            }
        }
    }
    if (m_widgetSelection.size() <= 0)
        emit selectedNoneWidget(tr("Nenhum controle selecionado."));
    else if (m_widgetSelection.size() == 1)
        emit selectedOneWidget(m_widgetSelection.at(0));
    else
        emit selectedMultipleWidgets(tr("Multiplos objetos selecionados."));
}


// =============================================================================
// Verifica se um widget está selecionado no momento
// =============================================================================
bool DesignerSurface::isWidgetSelected(QWidget* widget)
{
    return m_widgetSelection.contains(widget);
}


// =============================================================================
// Inicia o redimensionamento de uma seleção
// =============================================================================
void DesignerSurface::startSelectionResize()
{
    m_oldSize.clear();
    if (m_widgetSelection.empty())
        return;

    // salva o tamanho original dos widgets
    foreach (QWidget* widget, m_widgetSelection) {
        m_oldSize[widget] = widget->size();
    }
}


// =============================================================================
// Finaliza o redimensionamento de uma seleção
// =============================================================================
void DesignerSurface::endSelectionResize()
{
    foreach (QWidget* widget, m_widgetSelection) {
        // verifica se houve pelo menos uma modificação
        if (widget->size() != m_oldSize[widget]) {
            Cmd::Resize* resizeCmd = new Cmd::Resize(this,
                                                     m_widgetSelection,
                                                     m_oldSize);
            m_application->undoStack()->push(resizeCmd);
            return;
        }
    }
}


// =============================================================================
// Inicia o movimento de uma seleção
// =============================================================================
void DesignerSurface::startSelectionMove()
{
    m_oldPos.clear();
    if (m_widgetSelection.empty())
        return;

    // salva a posição original dos widgets
    foreach (QWidget* widget, m_widgetSelection) {
        m_oldPos[widget] = widget->pos();
    }
}


// =============================================================================
// Finaliza o movimento de uma seleção
// =============================================================================
void DesignerSurface::endSelectionMove()
{
    foreach (QWidget* widget, m_widgetSelection) {
        // verifica se houve pelo menos uma modificação
        if (widget->pos() != m_oldPos[widget]) {
            Cmd::Move *moveCmd = new Cmd::Move(this,
                                               m_widgetSelection,
                                               m_oldPos);
            m_application->undoStack()->push(moveCmd);
            return;
        }
    }
}

