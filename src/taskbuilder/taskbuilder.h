// =============================================================================
// Copyright (C) 2014    Marcos Medeiros
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
#ifndef TASKBUILDER_H
#define TASKBUILDER_H

#include <QDialog>
#include <QVariant>

class QPushButton;
class TaskBuilderSurface;
class MainWindow;
class QTextEdit;
class Highlighter;

class TaskBuilder : public QDialog
{
    Q_OBJECT
public:
    explicit TaskBuilder(MainWindow *parent = 0);

signals:

public slots:
    virtual int exec();
    void sendCode();
private:
    TaskBuilderSurface *m_surface;
    QPushButton *m_generate;
    QPushButton *m_cancel;
    QPushButton *m_send;
    MainWindow *m_app;
    QTextEdit *m_editor;
    Highlighter *m_ldtHighlighter;
};

#endif // TASKBUILDER_H
