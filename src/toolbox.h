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
#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QListWidget>

class MainWindow;
class QSize;

class ToolBox : public QListWidget
{
	Q_OBJECT
public:
    explicit ToolBox(MainWindow *main, QWidget *parent = 0);
    QWidget *lastWidget();

protected:
    void startDrag(Qt::DropActions supportedActions);

private:
	MainWindow *mainWindow;
    QWidget *m_widget;
};

#endif
