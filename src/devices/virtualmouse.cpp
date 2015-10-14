// =============================================================================
// Copyright (C) 2015    Marcos Medeiros
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
#include <QtTestGui>
#include "virtualmouse.h"
#include "virtualdesktop.h"
#include "../designersurface/designersurface.h"

VirtualMouse *VirtualMouse::sm_instance = nullptr;

VirtualMouse::VirtualMouse(QWidget *parent) :
    QWidget(parent)
{
    sm_instance = this;
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    m_arrow = QPixmap(":/images/mouse.png");
    setFixedSize(m_arrow.width(), m_arrow.height());
    m_anim.setTargetObject(this);
}

void VirtualMouse::moveTo(int x, int y)
{
    m_anim.setPropertyName("pos");
    m_anim.setDuration(100);
    m_anim.setStartValue(pos());

    QWidget *to = DesignerSurface::get()->childAt(x, y);
    if (to) {
        m_anim.setEndValue(to->geometry().center());
    } else {
        m_anim.setEndValue(QPoint(x,y));
    }
    m_anim.start();
}

void VirtualMouse::click()
{
    QWidget *to = DesignerSurface::get()->childAt(pos());
    if (to) {
        qDebug() << "Clicando em" << to->objectName();
        QTest::mouseClick(to, Qt::LeftButton);
        VirtualDesktop::get()->addClick(to->geometry().center());
    } else {
        qDebug() << "Widget não encontrado!";
    }
}

void VirtualMouse::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.drawPixmap(m_arrow.rect(), m_arrow);
}

