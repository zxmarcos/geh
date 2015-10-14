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
#ifndef QRECTEDIT_H
#define QRECTEDIT_H

#include <QLineEdit>
#include <QRect>
#include <QSpinBox>

class QRectEditPopup;

class QRectEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QRect m_rect READ rectValue WRITE setRectValue USER true)
public:
    explicit QRectEdit(QWidget *parent = 0);

    QRect rectValue() const;

public slots:
    void setRectValue(QRect rect);
    void setX(int x);
    void setY(int y);
    void setWidth(int width);
    void setHeight(int height);

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    QRectEditPopup *m_popup;
    QRect m_rect;
};

class QRectEditPopup : public QWidget
{
    Q_OBJECT
public:
    QRectEditPopup(QWidget *widget = 0);
    ~QRectEditPopup();
    QRect rectValue() const;
    void setRectValue(QRect size);
    void popup(QWidget *parent);

signals:
    void xValueChanged(int x);
    void yValueChanged(int y);
    void widthValueChanged(int x);
    void heightValueChanged(int y);

protected:
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void closeEvent(QCloseEvent *);
private:
    QSpinBox *m_xCoord;
    QSpinBox *m_yCoord;
    QSpinBox *m_width;
    QSpinBox *m_height;
    QWidget *popupParent;
};

#endif // QRECTEDIT_H
