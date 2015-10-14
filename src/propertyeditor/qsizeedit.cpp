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
#include "qsizeedit.h"
#include "../util.h"

using namespace std;

QSizeEdit::QSizeEdit(QWidget *parent) :
    QLineEdit(parent)
{
    setAutoFillBackground(true);
    setReadOnly(true);
    m_popup = NULL;
    setFrame(false);
}

QSizeEdit::~QSizeEdit()
{
}

void QSizeEdit::mousePressEvent(QMouseEvent *event)
{
    event->accept();
    if (!m_popup) {
        m_popup = new QSizeEditPopup(this);
        connect(m_popup, SIGNAL(widthValueChanged(int)), this, SLOT(setWidth(int)));
        connect(m_popup, SIGNAL(heightValueChanged(int)), this, SLOT(setHeight(int)));
        m_popup->setSizeValue(m_size);
    }
    m_popup->popup(this);

}

QSize QSizeEdit::sizeValue() const
{
    return m_size;
}

void QSizeEdit::setSizeValue(QSize size)
{
    m_size = size;
    setText(qsizeToString(m_size));
}

void QSizeEdit::setWidth(int width)
{
    m_size.setWidth(width);
    setText(qsizeToString(m_size));
}

void QSizeEdit::setHeight(int height)
{
    m_size.setHeight(height);
    setText(qsizeToString(m_size));
}

// =============================================================================
// Editor de QPoint
// =============================================================================
QSizeEditPopup::QSizeEditPopup(QWidget *parent) :
    QWidget(parent, Qt::Popup)
{
    //setAttribute(Qt::WA_WindowPropagation);
    setAttribute(Qt::WA_NoMouseReplay);
    setAttribute(Qt::WA_WindowPropagation);

    QHBoxLayout *layout = new QHBoxLayout;

    m_width = new QSpinBox;
    m_height = new QSpinBox;

    m_width->setMaximum(SHRT_MAX);
    m_height->setMaximum(SHRT_MAX);

    m_width->setMinimum(-SHRT_MAX);
    m_height->setMinimum(-SHRT_MAX);

    layout->addWidget(new QLabel(tr("w:")));
    layout->addWidget(m_width);
    layout->addWidget(new QLabel(tr("h:")));
    layout->addWidget(m_height);

    setLayout(layout);

    connect(m_width, SIGNAL(valueChanged(int)), this, SIGNAL(widthValueChanged(int)));
    connect(m_height, SIGNAL(valueChanged(int)), this, SIGNAL(heightValueChanged(int)));
}

QSizeEditPopup::~QSizeEditPopup()
{
}

// =============================================================================
// Retorna o valor do QPoint
// =============================================================================
QSize QSizeEditPopup::sizeValue() const
{
    return QSize(m_width->value(), m_height->value());
}

// =============================================================================
// Define os valores de x e y através de um QPoint
// =============================================================================
void QSizeEditPopup::setSizeValue(QSize size)
{
    m_width->setValue(size.width());
    m_height->setValue(size.height());
}

void QSizeEditPopup::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
}

void QSizeEditPopup::mouseReleaseEvent(QMouseEvent *e)
{
    if (rect().contains(e->pos()))
        close();
}

void QSizeEditPopup::closeEvent(QCloseEvent *e)
{
    e->accept();
}

void QSizeEditPopup::popup(QWidget *parent)
{
    popupParent = parent;
    if (popupParent)
        move(popupParent->mapToGlobal(popupParent->rect().bottomLeft()));
    raise();
    show();
}

