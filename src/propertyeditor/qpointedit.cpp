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
#include <iostream>
#include <stdint.h>
#include "qpointedit.h"
#include "../util.h"

using namespace std;

QPointEdit::QPointEdit(QWidget *parent) :
    QLineEdit(parent)
{
    setAutoFillBackground(true);
    setReadOnly(true);
    m_popup = NULL;
    setFrame(false);
}

QPointEdit::~QPointEdit()
{
}

void QPointEdit::mousePressEvent(QMouseEvent *event)
{
    event->accept();
    if (!m_popup) {
        m_popup = new QPointEditPopup(this);
        connect(m_popup, SIGNAL(xValueChanged(int)), this, SLOT(setX(int)));
        connect(m_popup, SIGNAL(yValueChanged(int)), this, SLOT(setY(int)));
        m_popup->setPoint(m_point);
    }
    m_popup->popup(this);

}

QPoint QPointEdit::point() const
{
    return m_point;
}

void QPointEdit::setPoint(QPoint point)
{
    m_point = point;
    setText(qpointToString(m_point));
}

void QPointEdit::setX(int x)
{
    m_point.setX(x);
    setText(qpointToString(m_point));
}

void QPointEdit::setY(int y)
{
    m_point.setY(y);
    setText(qpointToString(m_point));
}

// =============================================================================
// Editor de QPoint
// =============================================================================
QPointEditPopup::QPointEditPopup(QWidget *parent) :
    QWidget(parent, Qt::Popup)
{
    //setAttribute(Qt::WA_WindowPropagation);
    setAttribute(Qt::WA_NoMouseReplay);
    setAttribute(Qt::WA_WindowPropagation);

    QHBoxLayout *layout = new QHBoxLayout;

    m_xCoord = new QSpinBox;
    m_yCoord = new QSpinBox;

    m_xCoord->setMaximum(SHRT_MAX);
    m_yCoord->setMaximum(SHRT_MAX);

    m_xCoord->setMinimum(-SHRT_MAX);
    m_yCoord->setMinimum(-SHRT_MAX);

    layout->addWidget(new QLabel(tr("x:")));
    layout->addWidget(m_xCoord);
    layout->addWidget(new QLabel(tr("y:")));
    layout->addWidget(m_yCoord);

    setLayout(layout);

    connect(m_xCoord, SIGNAL(valueChanged(int)), this, SIGNAL(xValueChanged(int)));
    connect(m_yCoord, SIGNAL(valueChanged(int)), this, SIGNAL(yValueChanged(int)));
}

QPointEditPopup::~QPointEditPopup()
{
}

// =============================================================================
// Retorna o valor do QPoint
// =============================================================================
QPoint QPointEditPopup::point() const
{
    return QPoint(m_xCoord->value(), m_yCoord->value());
}

// =============================================================================
// Define os valores de x e y através de um QPoint
// =============================================================================
void QPointEditPopup::setPoint(QPoint point)
{
    m_xCoord->setValue(point.x());
    m_yCoord->setValue(point.y());
}

void QPointEditPopup::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
}

void QPointEditPopup::mouseReleaseEvent(QMouseEvent *e)
{
    if (rect().contains(e->pos()))
        close();
}

void QPointEditPopup::closeEvent(QCloseEvent *e)
{
    e->accept();
}

void QPointEditPopup::popup(QWidget *parent)
{
    popupParent = parent;
    if (popupParent)
        move(popupParent->mapToGlobal(popupParent->rect().bottomLeft()));
    raise();
    show();
}
