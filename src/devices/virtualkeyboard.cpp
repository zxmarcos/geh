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
#include <QtTest>
#include "virtualkeyboard.h"

using namespace detail;

VirtualKeyboard* VirtualKeyboard::sm_instance = nullptr;

VirtualKeyboard::VirtualKeyboard(QWidget *parent) :
    QWidget(parent)
{
    sm_instance = this;
    const int keysPerRow = 23;
    setFixedWidth(23 * vkWidth + keysPerRow - 1);
    setFixedHeight(7 * vkHeight);

    QVBoxLayout *main = new QVBoxLayout;
    main->setMargin(3);
    main->setSpacing(1);

    QHBoxLayout *layout;


    layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    auto addKey = [&](QHBoxLayout *layout, QString str, int nslots=1) {
        VirtualKey *key = new VirtualKey(0, 0, str, nslots, this);
        layout->addWidget(key);
        m_keys[str.toLower()] = key;
    };

    addKey(layout, "ESC");
    layout->addWidget(new SpacerKey());
    addKey(layout, "F1");
    addKey(layout, "F2");
    addKey(layout, "F3");
    addKey(layout, "F4");
    layout->addWidget(new SpacerKey());
    addKey(layout, "F5");
    addKey(layout, "F6");
    addKey(layout, "F7");
    addKey(layout, "F8");
    layout->addWidget(new SpacerKey());
    addKey(layout, "F9");
    addKey(layout, "F10");
    addKey(layout, "F11");
    addKey(layout, "F12");
    layout->addWidget(new SpacerKey());
    addKey(layout, "F13");
    addKey(layout, "F14");
    addKey(layout, "F15");
    layout->addWidget(new SpacerKey(5));
    main->addLayout(layout);

    layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    addKey(layout, "`");
    layout->addWidget(new SpacerKey());
    addKey(layout, "1");
    addKey(layout, "2");
    addKey(layout, "3");
    addKey(layout, "4");
    addKey(layout, "5");
    addKey(layout, "6");
    addKey(layout, "7");
    addKey(layout, "8");
    addKey(layout, "9");
    addKey(layout, "0");
    addKey(layout, "-");
    addKey(layout, "=");
    addKey(layout, "DEL");
    layout->addWidget(new SpacerKey());
    addKey(layout, "HELP");
    addKey(layout, "HOME");
    addKey(layout, "PGUP");
    layout->addWidget(new SpacerKey());
    addKey(layout, "CLR");
    addKey(layout, "=");
    addKey(layout, "/");
    addKey(layout, "*");
    main->addLayout(layout);

    layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    addKey(layout, "Tab", 2);
    addKey(layout, "Q");
    addKey(layout, "W");
    addKey(layout, "E");
    addKey(layout, "R");
    addKey(layout, "T");
    addKey(layout, "Y");
    addKey(layout, "U");
    addKey(layout, "I");
    addKey(layout, "O");
    addKey(layout, "P");
    addKey(layout, "[");
    addKey(layout, "]");
    addKey(layout, "\\");
    layout->addWidget(new SpacerKey());
    addKey(layout, "FDEL");
    addKey(layout, "END");
    addKey(layout, "PGDN");
    layout->addWidget(new SpacerKey());
    addKey(layout, "7");
    addKey(layout, "8");
    addKey(layout, "9");
    addKey(layout, "-");
    main->addLayout(layout);

    layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    addKey(layout, "Cps Lk", 2);
    addKey(layout, "A");
    addKey(layout, "S");
    addKey(layout, "D");
    addKey(layout, "F");
    addKey(layout, "G");
    addKey(layout, "H");
    addKey(layout, "J");
    addKey(layout, "K");
    addKey(layout, "L");
    addKey(layout, ";");
    addKey(layout, "'");
    addKey(layout, "Ret", 2);
    layout->addWidget(new SpacerKey(5));
    addKey(layout, "4");
    addKey(layout, "5");
    addKey(layout, "6");
    addKey(layout, "+");
    main->addLayout(layout);

    layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    addKey(layout, "Shift", 2);
    addKey(layout, "Z");
    addKey(layout, "X");
    addKey(layout, "C");
    addKey(layout, "V");
    addKey(layout, "B");
    addKey(layout, "N");
    addKey(layout, "M");
    addKey(layout, ",");
    addKey(layout, ".");
    addKey(layout, "RShift", 2);
    layout->addWidget(new SpacerKey(4));
    addKey(layout, "UP");
    layout->addWidget(new SpacerKey(2));
    addKey(layout, "1");
    addKey(layout, "2");
    addKey(layout, "3");
    layout->addWidget(new SpacerKey());
    main->addLayout(layout);

    layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    addKey(layout, "Ctrl", 2);
    addKey(layout, "Opt");
    addKey(layout, "Cmd");
    addKey(layout, "Space", 8);
    addKey(layout, "Cmd");
    addKey(layout, "Opt");
    addKey(layout, "RCtrl");
    layout->addWidget(new SpacerKey());
    addKey(layout, "LEFT");
    addKey(layout, "DOWN");
    addKey(layout, "RIGHT");
    layout->addWidget(new SpacerKey());
    addKey(layout, " ");
    addKey(layout, "0");
    addKey(layout, ".");
    layout->addWidget(new SpacerKey());
    main->addLayout(layout);

    setLayout(main);
}

VirtualKeyboard::~VirtualKeyboard()
{
}

int strToQtKey(QString const& skey)
{
    QKeySequence seq(skey);
    if (seq.count())
        return seq[0];
    return 0;
}

void VirtualKeyboard::press(QString key)
{
    int vk = strToQtKey(key);
    QTest::keyPress(QApplication::focusWidget(), vk, Qt::NoModifier);
    if (m_keys.find(key) != m_keys.end()) {
        QWidget *w = m_keys[key];
        QTest::mouseClick(w, Qt::LeftButton);
    }
    QTest::keyRelease(QApplication::focusWidget(), vk, Qt::NoModifier, 10);
}

void VirtualKeyboard::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setClipRect(event->rect());

    p.fillRect(rect(), Qt::darkGray);
    p.setPen(Qt::black);
    p.drawRect(rect().adjusted(0,0,-1,-1));
}

namespace detail {

VirtualKey::VirtualKey(int row, int column, QString name, int nslots, QWidget *parent) :
    QWidget(parent), m_row(row), m_column(column), m_char(name)
{
    m_state = NORMAL;
    setFixedHeight(vkHeight);
    setFixedWidth(nslots*vkWidth+nslots-1);
}

void VirtualKey::release()
{
    m_state = NORMAL;
    update();
}

void VirtualKey::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setClipRect(event->rect());

    if (m_state == NORMAL) {
        p.setPen(Qt::black);
        p.setBrush(Qt::lightGray);
    } else {
        p.setPen(Qt::lightGray);
        p.setBrush(Qt::black);
    }
    QFont f;
    switch (m_char.size()) {
    case 1:
        f.setPointSize(9);
        break;
    case 2:
    case 3:
        f.setPixelSize(10);
        break;
    default:
        f.setPixelSize(7);
        break;
    }
    p.setFont(f);
    p.drawRoundRect(rect().adjusted(0, 0, -1, -1), 0, 0);
    p.drawText(rect(), Qt::AlignHCenter | Qt::AlignVCenter, m_char);
}

void VirtualKey::mousePressEvent(QMouseEvent *event)
{
    m_state = PRESSED;
    update();
    QTimer::singleShot(200, this, SLOT(release()));
}

void VirtualKey::mouseReleaseEvent(QMouseEvent *event)
{
}


}
