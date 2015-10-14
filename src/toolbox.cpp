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
#include <QtWidgets>
#include <iostream>
#include "toolbox.h"
#include "mainwindow.h"
#include "globalids.h"
#include "widgetfactory.h"

using namespace std;

ToolBox::ToolBox(MainWindow *main, QWidget *parent)
    : QListWidget(parent),
      mainWindow(main),
      m_widget(nullptr)
{
    // Ajusta o controle para permitir drag 'n drop
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDropIndicatorShown(true);
    setMovement(Static);
    setDragDropMode(QAbstractItemView::DragOnly);

    // Coloca os controles na lista
    addItem(new QListWidgetItem(QIcon(tr(":/images/widgets/label.png")),
                                tr("Label"), this, GEH_CTRL_LABEL));
    addItem(new QListWidgetItem(QIcon(tr(":/images/widgets/pushbutton.png")),
                                tr("Botão"), this, GEH_CTRL_BUTTON));
    addItem(new QListWidgetItem(QIcon(tr(":/images/widgets/lineedit.png")),
                                tr("Caixa de Texto"), this, GEH_CTRL_EDITBOX));
    addItem(new QListWidgetItem(QIcon(tr(":/images/widgets/checkbox.png")),
                                tr("Check Box"), this, GEH_CTRL_CHECKBOX));
    addItem(new QListWidgetItem(QIcon(tr(":/images/widgets/radiobutton.png")),
                                tr("Botão Radio"), this, GEH_CTRL_RADIOBUTTON));
    addItem(new QListWidgetItem(QIcon(tr(":/images/widgets/groupbox.png")),
                                tr("Grupo"), this, GEH_CTRL_GROUPBOX));
    addItem(new QListWidgetItem(QIcon(tr(":/images/widgets/combobox.png")),
                                tr("Caixa de Seleção"), this, GEH_CTRL_COMBOBOX));
}

// =============================================================================
// Retorna o último componente criado, note que aqui não sabemos se ele ainda
// existe, esteja avisado.
// =============================================================================
QWidget *ToolBox::lastWidget()
{
    return m_widget;
}

// =============================================================================
// Arrasta um componente
// =============================================================================
void ToolBox::startDrag(Qt::DropActions /* supportedActions */)
{
    QListWidgetItem *item = currentItem();
    //cout << "Arrastando " << item->type() << endl;

    m_widget = WidgetFactory::createWidget(item->type());
    QPixmap pixmap =  m_widget->grab();

    QMimeData *mimeData = new QMimeData;

    mimeData->setData(WidgetFactory::mimeString(), QByteArray());

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));
    drag->setPixmap(pixmap);

    // Se não foi aceito pelo designer, então descartamos o widget
    if (drag->exec(Qt::MoveAction) == Qt::IgnoreAction) {
        delete m_widget;
    }
}
