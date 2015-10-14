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
#ifndef VIRTUALKEYBOARD_H
#define VIRTUALKEYBOARD_H

#include <QWidget>
#include <QPainter>
#include <QMap>

namespace detail {

const int vkHeight = 20;
const int vkWidth = 24;

class VirtualKey : public QWidget
{
    Q_OBJECT

public:
    VirtualKey(int row, int column, QString name, int nslots=1, QWidget *parent = 0);

public slots:
    void release();
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
private:
    int m_row;
    int m_column;
    QString m_char;
    enum State {
        NORMAL = 0,
        PRESSED,
        HOVERED,
    };
    int m_state;
};

class SpacerKey : public QWidget
{
public:
    SpacerKey(int nslots=1, QWidget *parent = 0) : QWidget(parent) {
        setFixedWidth(nslots*vkWidth+nslots-2);
    }

protected:
};

}

class VirtualKeyboard : public QWidget
{
    Q_OBJECT

public:
    explicit VirtualKeyboard(QWidget *parent = 0);
    ~VirtualKeyboard();

    void press(QString key);
protected:
    virtual void paintEvent(QPaintEvent *event);
private:
    static VirtualKeyboard *sm_instance;
    QMap<QString, detail::VirtualKey*> m_keys;
public:
    static VirtualKeyboard *get() {
        return sm_instance;
    }
};


#endif // VIRTUALKEYBOARD_H
