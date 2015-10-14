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
#include "virtualdesktop.h"
#include "../designersurface/designersurface.h"

VirtualDesktop *VirtualDesktop::sm_instance = nullptr;

VirtualDesktop::VirtualDesktop(QWidget *parent) :
    QWidget(parent)
{
    sm_instance = this;
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    m_animTimer.setInterval(50);
    connect(&m_animTimer, SIGNAL(timeout()), this, SLOT(updateAnim()));
    m_isAttended = false;
}

void VirtualDesktop::addClick(QPoint at)
{
    ClickAnim a;
    a.duration = 5;
    a.center = at;
    a.radius = 2;
    m_clickAnims.push_back(a);
    if (!m_animTimer.isActive())
        m_animTimer.start();
}

void VirtualDesktop::lookAt(QPoint at)
{
    QWidget *to = DesignerSurface::get()->childAt(at);
    if (to) {
        m_eyeRect = to->geometry().adjusted(-3, -3, 3, 3);
        m_isAttended = true;
        update();
    }
}

void VirtualDesktop::clearVision()
{
    if (m_isAttended) {
        m_isAttended = false;
        update();
    }
}

void VirtualDesktop::clear()
{
    m_isAttended = false;
    m_clickAnims.clear();
}

void VirtualDesktop::updateAnim()
{
    QList<int> removes;
    for (int i = 0; i < m_clickAnims.size(); i++) {
        if (m_clickAnims[i].duration > 0) {
            m_clickAnims[i].duration--;
            m_clickAnims[i].radius += 3;
        } else {
            removes.push_back(i);
        }
    }
    foreach (int k, removes) {
        m_clickAnims.removeAt(k);
    }
    update();
    if (m_clickAnims.empty())
        m_animTimer.stop();
}

bool VirtualDesktop::eventFilter(QObject *obj, QEvent *event)
{
    if (!obj->isWidgetType())
        return false;

    QWidget *w = static_cast<QWidget*>(obj);

    if (event->type() == QEvent::Resize) {
        if (w == parent())
            setGeometry(w->geometry());
    }
    return false;

}

void VirtualDesktop::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setBrush(QColor(255,0,0,120));
    for (int i = 0; i < m_clickAnims.size(); i++) {
        const int radius = m_clickAnims[i].radius;
        p.drawEllipse(m_clickAnims[i].center, radius, radius);
    }

    if (m_isAttended) {
        p.setBrush(QColor(0,255,0,70));
        p.setPen(Qt::green);
        p.drawRect(m_eyeRect);
    }
}

