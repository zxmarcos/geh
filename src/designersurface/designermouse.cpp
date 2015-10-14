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
#include "mainwindow.h"
#include "propertyeditor/propertyeditor.h"
#include "propertyeditor/comboitemeditor.h"
#include "designersurface.h"
#include "widgetfactory.h"
#include "util.h"

// =============================================================================
// Gerencia as ações tomados com o movimento do mouse, lembre-se que como nosso
// widget está marcado para receber todos os movimentos do mouse, então é
// necessário saber quando o mouse está pressionado.
// =============================================================================
void DesignerSurface::mouseMoveEvent(QMouseEvent* event)
{
    EDIT_MODE_ONLY(event);

    // verifica se o mouse está sobre um redimensionador e altera o cursor
    if (isMouseOverSizer(event->pos()) || m_mouseState == Scaling) {
        if (cursor().shape() != Qt::SizeFDiagCursor)
            setCursor(Qt::SizeFDiagCursor);
    } else {
        if (cursor().shape() != Qt::ArrowCursor)
            setCursor(Qt::ArrowCursor);
    }

    QPoint delta = event->pos() - m_startPos.toPoint();

    switch (m_mouseState) {
    case Moving: {
        // verifica se alguma coisa se moveu de fato :P
        if (delta != QPoint(0, 0)) {
            QRect lastRect = childrenRect();
            moveSelection(delta);
            m_startPos = event->pos();

           update(lastRect.united(childrenRect()).adjusted(-20, -20, 20, 20));

        }
        break;
    }
    case Scaling: {
        if (delta != QPoint(0, 0)) {
            QRect lastRect = childrenRect();
            resizeSelection(delta);
            m_startPos = event->pos();
            update(lastRect.united(childrenRect()).adjusted(-20, -20, 20, 20));
        }
        break;
    }
    case Selecting: {

        // Vamos redesenhar apenas o que é necessário
        QRect last = pointsToRect(m_startPos.toPoint(), m_endPos.toPoint());
        QRect now = pointsToRect(m_startPos.toPoint(), event->pos());
        update(last.united(now).adjusted(-3, -3, 3, 3));
        m_endPos = event->pos();
        break;
    }

    case Reparenting: {
        if ((event->pos() - m_startPos).manhattanLength() < QApplication::startDragDistance())
            break;

        startReparentDrag();
        update();
        break;
    }

    default:
        break;
    }
    event->accept();
}


// =============================================================================
// Gerencia os eventos dos botões do mouse
// =============================================================================
void DesignerSurface::mousePressEvent(QMouseEvent* event)
{
    EDIT_MODE_ONLY(event);

    event->accept();
    m_startPos = event->pos();
    m_endPos = event->pos();

    // primeiro verificamos se está posiciona em cima de um redimensionador
    if (isMouseOverSizer(event->pos())) {
        // cout << "Redimensionar" << endl;
        m_mouseState = Scaling;
        startSelectionResize();
        return;
    }

    m_currentWidget = childAt(event->pos());

    // Reparentear
    if (QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier) &&
            event->button() == Qt::LeftButton) {
        m_widgetSelection.clear();

        if (m_currentWidget == nullptr) {
            event->ignore();
            return;
        }

        event->accept();
        m_mouseState = Reparenting;
        m_widgetSelection.push_back(m_currentWidget);
        update();

        return;
    }

    // só vamos redesenhar ao redor do widget selecionado
    if (m_currentWidget) {
        // se o widget não estiver em alguma seleção...
        if (!m_widgetSelection.contains(m_currentWidget)) {

            // se CTRL não estiver pressionado...
            if (!QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
                m_widgetSelection.clear();
                emit selectedOneWidget(m_currentWidget);
            }
            m_widgetSelection.push_back(m_currentWidget);
        }
        // cout << "Mover" << endl;
        m_mouseState = Moving;
        startSelectionMove();

        update();
    } else {
        // cout << "Selecionar" << endl;
        m_mouseState = Selecting;
    }
}

// =============================================================================
// Gerencia os cliques duplos
// =============================================================================
void DesignerSurface::mouseDoubleClickEvent(QMouseEvent* event)
{
    m_mouseState = Normal;

    m_currentWidget = childAt(event->pos());
    if (m_currentWidget != nullptr) {
        if (isComboBox(m_currentWidget)) {
            ComboItemEditor *editor = m_application->propertyEditor()->comboEditor();
            editor->setComboBox(static_cast<QComboBox*>(m_currentWidget));
            m_mouseState = Normal;
            editor->exec();
        } else if (isCheckBox(m_currentWidget)) {
            toogleCheckBox(static_cast<QCheckBox*>(m_currentWidget));
        }
    }
}

// =============================================================================
// Gerencia os eventos relacionados ao liberar um botão do mouse
// =============================================================================
void DesignerSurface::mouseReleaseEvent(QMouseEvent* event)
{
    EDIT_MODE_ONLY(event);

    switch (m_mouseState) {
    case Scaling:
        if (m_snapToGrid)
            snapWidgetsSizeToGrid();
        endSelectionResize();
        break;
    case Selecting:
        findSelection();
        break;
    case Moving:
        if (m_snapToGrid)
            snapWidgetsToGrid();
        endSelectionMove();
        break;
    }
    if (m_mouseState == Scaling || m_mouseState == Moving)
        emit widgetsModified(m_widgetSelection);
    update();
    event->accept();
    m_mouseState = Normal;
}


// =============================================================================
// Verifica se o mouse está em cima de algum sizer
// =============================================================================
bool DesignerSurface::isMouseOverSizer(const QPoint &mouse)
{
    foreach (QWidget *widget, m_widgetSelection) {
        if (getSizerRect(widget).contains(mouse))
            return true;
    }
    return false;
}

