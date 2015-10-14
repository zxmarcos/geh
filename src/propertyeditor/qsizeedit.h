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
#ifndef QSIZEEDIT_H
#define QSIZEEDIT_H

#include <QLabel>
#include <QWidget>
#include <QSpinBox>
#include <QLineEdit>

class QSizeEditPopup;

class QSizeEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(QSize m_size READ sizeValue WRITE setSizeValue USER true)
public:
    explicit QSizeEdit(QWidget *parent = 0);

    ~QSizeEdit();
    QSize sizeValue() const;

public slots:
    void setSizeValue(QSize size);
    void setWidth(int width);
    void setHeight(int height);

protected:
    void mousePressEvent(QMouseEvent *event);
private:
    QSize m_size;
    QSizeEditPopup *m_popup;
};

class QSizeEditPopup : public QWidget
{
    Q_OBJECT
public:
    QSizeEditPopup(QWidget *widget = 0);
    ~QSizeEditPopup();
    QSize sizeValue() const;
    void setSizeValue(QSize size);
    void popup(QWidget *parent);

signals:
    void widthValueChanged(int x);
    void heightValueChanged(int y);

protected:
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void closeEvent(QCloseEvent *);
private:
    QSpinBox *m_width;
    QSpinBox *m_height;
    QWidget *popupParent;
};

#endif // QSIZEEDIT_H
