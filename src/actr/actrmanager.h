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
#ifndef ACTRMANAGER_H
#define ACTRMANAGER_H

#include <QWidget>
#include "actrenvironment.h"

class QTextEdit;
class QLineEdit;
class QProcess;
class QPushButton;
class QAction;
class QToolBar;

class ActrManager : public QWidget
{
    Q_OBJECT
public:
    explicit ActrManager(QWidget *parent = 0);
    virtual ~ActrManager();
    bool init();
    void kill();
signals:
public slots:
    void readOutput();
    void readError();
    void writeInput();
    void writeInput(QString str);
    bool loadModel(QString name);
    bool openModel();
    void runModel(int ticks=100);
    void clear();
    void run();
private:
    void createActions();
    void createControls();
    void connectActions();
    void processInput(const QString& str);

    QTextEdit* m_output;
    QLineEdit* m_input;
    QProcess* m_proc;
    QPushButton* m_send;
    QToolBar* m_toolBar;
    QAction* m_openModel;
    QAction* m_runModel;
    QAction* m_clearConsole;
    ActrEnvironment* m_environment;
};

#endif // ACTRMANAGER_H
