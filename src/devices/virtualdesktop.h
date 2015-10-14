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
#ifndef VIRTUALDESKTOP_H
#define VIRTUALDESKTOP_H

#include <QWidget>
#include <QVector>
#include <QTimer>

class VirtualDesktop : public QWidget
{
    Q_OBJECT
public:
    explicit VirtualDesktop(QWidget *parent = 0);
    void addClick(QPoint at);
    void lookAt(QPoint at);
    void clearVision();
    void clear();
signals:

public slots:
private slots:
    void updateAnim();
protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void paintEvent(QPaintEvent *event);
private:
    struct ClickAnim {
        QPoint center;
        int duration;
        qreal radius;
    };
    QVector<ClickAnim> m_clickAnims;
    static VirtualDesktop *sm_instance;
    QTimer m_animTimer;
    bool m_isAttended;
    QRect m_eyeRect;
public:
    static VirtualDesktop *get() {
        return sm_instance;
    }
};

#endif // VIRTUALDESKTOP_H
