// =============================================================================
// Copyright (C) 2013    Marcos Medeiros
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
#ifndef QPOINTEDIT_H
#define QPOINTEDIT_H

#include <QLabel>
#include <QWidget>
#include <QSpinBox>
#include <QLineEdit>

class QPointEditPopup;

class QPointEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QPoint m_point READ point WRITE setPoint USER true)
public:
    explicit QPointEdit(QWidget *parent = 0);

    ~QPointEdit();
    QPoint point() const;

public slots:
    void setPoint(QPoint point);
    void setX(int x);
    void setY(int y);

protected:
    void mousePressEvent(QMouseEvent *event);
private:
    QPoint m_point;
    QPointEditPopup *m_popup;
};

class QPointEditPopup : public QWidget
{
    Q_OBJECT
public:
    QPointEditPopup(QWidget *widget = 0);
    ~QPointEditPopup();
    QPoint point() const;
    void setPoint(QPoint point);
    void popup(QWidget *parent);

signals:
    void xValueChanged(int x);
    void yValueChanged(int y);

protected:
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void closeEvent(QCloseEvent *);
private:
    QSpinBox *m_xCoord;
    QSpinBox *m_yCoord;
    QWidget *popupParent;
};

#endif // QPOINTEDIT_H
