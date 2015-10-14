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
#include "qrectedit.h"
#include "../util.h"

QRectEdit::QRectEdit(QWidget *parent) :
    QLineEdit(parent)
{
    setFrame(false);
    setReadOnly(true);
    setAutoFillBackground(true);
    m_popup = NULL;
}

QRect QRectEdit::rectValue() const
{
    return m_rect;
}

void QRectEdit::setRectValue(QRect rect)
{
    m_rect = rect;
    setText(qrectToString(rect));
}

void QRectEdit::setX(int x)
{
    m_rect.setX(x);
    setText(qrectToString(m_rect));
}

void QRectEdit::setY(int y)
{
    m_rect.setY(y);
    setText(qrectToString(m_rect));
}

void QRectEdit::setWidth(int width)
{
    m_rect.setWidth(width);
    setText(qrectToString(m_rect));
}

void QRectEdit::setHeight(int height)
{
    m_rect.setHeight(height);
    setText(qrectToString(m_rect));
}

void QRectEdit::mousePressEvent(QMouseEvent *event)
{
    event->accept();
    if (!m_popup) {
        m_popup = new QRectEditPopup(this);
        connect(m_popup, SIGNAL(xValueChanged(int)), this, SLOT(setX(int)));
        connect(m_popup, SIGNAL(yValueChanged(int)), this, SLOT(setY(int)));
        connect(m_popup, SIGNAL(widthValueChanged(int)), this, SLOT(setWidth(int)));
        connect(m_popup, SIGNAL(heightValueChanged(int)), this, SLOT(setHeight(int)));
        m_popup->setRectValue(m_rect);
    }
    m_popup->popup(this);
}

QRectEditPopup::QRectEditPopup(QWidget *parent) :
    QWidget(parent, Qt::Popup)
{
    setAttribute(Qt::WA_NoMouseReplay);
    setAttribute(Qt::WA_WindowPropagation);

    QVBoxLayout *layout = new QVBoxLayout;

    m_xCoord = new QSpinBox;
    m_yCoord = new QSpinBox;
    m_width = new QSpinBox;
    m_height = new QSpinBox;

    m_xCoord->setMaximum(SHRT_MAX);
    m_yCoord->setMaximum(SHRT_MAX);
    m_xCoord->setMinimum(-SHRT_MAX);
    m_yCoord->setMinimum(-SHRT_MAX);

    m_width->setMaximum(SHRT_MAX);
    m_height->setMaximum(SHRT_MAX);
    m_width->setMinimum(-SHRT_MAX);
    m_height->setMinimum(-SHRT_MAX);

    QHBoxLayout *layout1 = new QHBoxLayout;
    QHBoxLayout *layout2 = new QHBoxLayout;

    layout1->addWidget(new QLabel(tr("x:")));
    layout1->addWidget(m_xCoord);
    layout1->addWidget(new QLabel(tr("y:")));
    layout1->addWidget(m_yCoord);
    layout2->addWidget(new QLabel(tr("w:")));
    layout2->addWidget(m_width);
    layout2->addWidget(new QLabel(tr("h:")));
    layout2->addWidget(m_height);

    layout->addLayout(layout1);
    layout->addLayout(layout2);
    setLayout(layout);

    connect(m_xCoord, SIGNAL(valueChanged(int)), this, SIGNAL(xValueChanged(int)));
    connect(m_yCoord, SIGNAL(valueChanged(int)), this, SIGNAL(yValueChanged(int)));
    connect(m_width, SIGNAL(valueChanged(int)), this, SIGNAL(widthValueChanged(int)));
    connect(m_height, SIGNAL(valueChanged(int)), this, SIGNAL(heightValueChanged(int)));
}

QRectEditPopup::~QRectEditPopup()
{
}

// =============================================================================
// Retorna o valor do QRect
// =============================================================================
QRect QRectEditPopup::rectValue() const
{
    return QRect(m_xCoord->value(), m_yCoord->value(),
                 m_width->value(), m_height->value());
}

// =============================================================================
// Define os valores do retângulo
// =============================================================================
void QRectEditPopup::setRectValue(QRect rect)
{
    m_xCoord->setValue(rect.x());
    m_yCoord->setValue(rect.y());
    m_width->setValue(rect.width());
    m_height->setValue(rect.height());
}

void QRectEditPopup::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
}

void QRectEditPopup::mouseReleaseEvent(QMouseEvent *e)
{
    if (rect().contains(e->pos()))
        close();
}

void QRectEditPopup::closeEvent(QCloseEvent *e)
{
    e->accept();
}

void QRectEditPopup::popup(QWidget *parent)
{
    popupParent = parent;
    if (popupParent)
        move(popupParent->mapToGlobal(popupParent->rect().bottomLeft()));
    raise();
    show();
}

