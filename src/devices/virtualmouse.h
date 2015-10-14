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
#ifndef VIRTUALMOUSE_H
#define VIRTUALMOUSE_H

#include <QWidget>
#include <QPropertyAnimation>

class VirtualMouse : public QWidget
{
    Q_OBJECT
public:
    explicit VirtualMouse(QWidget *parent = 0);

    void moveTo(int x, int y);
    void click();
signals:

public slots:
protected:
    virtual void paintEvent(QPaintEvent *event);
private:
    QPixmap m_arrow;
    static VirtualMouse *sm_instance;
    QPropertyAnimation m_anim;
public:
    static VirtualMouse *get() {
        return sm_instance;
    }
};

#endif // VIRTUALMOUSE_H
