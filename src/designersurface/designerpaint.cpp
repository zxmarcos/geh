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
#include "util.h"

// =============================================================================
// Nossa função de desenho :-)
// =============================================================================
void DesignerSurface::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setClipRect(event->rect());

    //cout << "Repaint: " << event->rect() << endl;

    painter.fillRect(rect(), palette().color(QPalette::Normal, QPalette::Window));

    // só desenhamos a grade em modo de edição :P
    if (m_editMode && m_showGrid)
        drawGrid(painter);

    if (m_lowerWidget == this) {
        painter.setPen(Qt::red);
        painter.drawRect(rect().adjusted(0, 0, -1, -1));
    } else {
        drawParentBox(m_lowerWidget, painter);
    }

    // Desenha a caixa de seleção
    if (m_mouseState == Selecting)
        drawSelectionRect(painter);

    drawSelectionBoxes(painter);
    event->accept();
}


// =============================================================================
// Desenha a grade
// =============================================================================
void DesignerSurface::drawGrid(QPainter& painter)
{
    const int xSpacing = m_gridSize.x();
    const int ySpacing = m_gridSize.y();
    painter.setPen(Qt::darkGray);
    for (int y = ySpacing; y < size().height(); y += ySpacing)
        for (int x = xSpacing; x < size().width(); x += xSpacing)
            painter.drawPoint(x, y);
}


// =============================================================================
// Desenha a caixa de seleção
// =============================================================================
void DesignerSurface::drawSelectionRect(QPainter& painter)
{
    QColor blue(0, 0, 255, 200);
    QColor blueAlpha(0, 0, 255, 20);

    // desenha a caixa
    painter.setPen(blue);
    QRect rect = QRect(m_startPos.toPoint(), m_endPos.toPoint());
    painter.drawRect(rect);
    painter.fillRect(rect.adjusted(1, 1, -1, -1), blueAlpha);
}


// =============================================================================
// Desenha as caixas de seleção ao redor dos widgets
// =============================================================================
void DesignerSurface::drawSelectionBoxes(QPainter& painter)
{
    QColor blue(0, 0, 255, 200);
    QPen pen(Qt::red, 1, Qt::DotLine);
    painter.setPen(blue);

    foreach (QWidget* widget, m_widgetSelection) {
        if (widget == m_lowerWidget)
            continue;

        if (m_mouseState == Moving) {
            painter.setPen(pen);
            painter.drawRect(mapToDesigner(widget));
        } else if (m_mouseState == Reparenting) {
            painter.setPen(QColor(255, 0, 200));
            painter.drawRect(mapToDesigner(widget).adjusted(m_selectionAdjust.x(),
                                                            m_selectionAdjust.y(),
                                                            m_selectionAdjust.width(),
                                                            m_selectionAdjust.height()));
        } else {
            painter.drawRect(mapToDesigner(widget).adjusted(m_selectionAdjust.x(),
                                                            m_selectionAdjust.y(),
                                                            m_selectionAdjust.width(),
                                                            m_selectionAdjust.height()));
            painter.fillRect(getSizerRect(widget), Qt::darkGreen);
            painter.drawRect(getSizerRect(widget));
        }

    }
}


// =============================================================================
// Desenha uma caixa ao redor de um widget
// =============================================================================
void DesignerSurface::drawParentBox(QWidget* widget, QPainter& painter)
{
    QPen pen(Qt::red, 2, Qt::DotLine);
    painter.setPen(pen);
    painter.drawRect(mapToDesigner(widget).adjusted(m_selectionAdjust.x(),
                                                    m_selectionAdjust.y(),
                                                    m_selectionAdjust.width(),
                                                    m_selectionAdjust.height()));
}

