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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTemporaryFile>
#include "gerador/sequencecompiler.h"
#include "gerador/modelcompiler.h"
#include "devices/virtualkeyboard.h"
#include "devices/virtualmouse.h"
#include "devices/virtualdesktop.h"

class QMenu;
class QAction;
class QSize;
class QScrollArea;
class QCloseEvent;
class DesignerSurface;
class AboutDialog;
class ToolBox;
class QFrame;
class PropertyEditor;
class QToolBar;
class QUndoStack;
class QDockWidget;
class QTabWidget;
class QTextEdit;
class Highlighter;
class ActrManager;
class TaskBuilder;
class QPlainTextEdit;
class ErrorEditor;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
    virtual ~MainWindow();

    QUndoStack *undoStack();
    DesignerSurface *designer();
    PropertyEditor *propertyEditor();
    QTextEdit *editor();
    ToolBox *toolBox();

private slots:
	void about();
    void saveProject();
    void loadProject();
    void generate();
    void run();

protected:
	void closeEvent(QCloseEvent *event);

private:
	void createActions();
	void createMenus();
	void createControls();
    void createToolbars();
    void connectActions();

    QUndoStack *m_appUndoStack;
	// menus
    QMenu *m_projectMenu;
    QMenu *m_editMenu;
    QMenu *m_errorMenu;
    QMenu *m_viewMenu;
    QMenu *m_aboutMenu;

    // toolbars
    QToolBar *m_projectToolbar;

	// ações
    QAction *m_newProjectAction;
    QAction *m_saveProjectAction;
    QAction *m_closeProjectAction;
    QAction *m_openProjectAction;
    QAction *m_closeApplicationAction;
    QAction *m_aboutApplicationAction;
    QAction *m_aboutQtAction;
    QAction *m_viewToolboxAction;
    QAction *m_viewPropertyEditorAction;
    QAction *m_viewGridAction;
    QAction *m_snapGridAction;
    QAction *m_editModeAction;

    QAction *m_openTaskBuilder;
    QAction *m_generateErrors;
    QAction *m_runSimulation;

    QAction *m_undoAction;
    QAction *m_redoAction;

	// diálogos
    AboutDialog *m_aboutDialog;

	// Controles
    QFrame *m_designerContainer;
    QScrollArea *m_designerArea;
    DesignerSurface *m_designer;
    ToolBox *m_toolbox;
    PropertyEditor *m_propertyEditor;
    QDockWidget *m_dockToolbox;
    QDockWidget *m_dockPropertyEditor;
    QTabWidget *m_bottomTabs;
    QTabWidget *m_upperTabs;
    QPlainTextEdit *m_lispOutputModel;
    QTextEdit *m_editor;
    Highlighter *m_ldtHighlighter;
    ActrManager *m_actrManager;
    TaskBuilder *m_taskBuilder;
    VirtualKeyboard *m_virtKeyboard;
    VirtualMouse *m_virtMouse;
    VirtualDesktop *m_desktop;

    ErrorEditor *m_errorEditor;
    SequenceCompiler m_seqCompiler;
    ModelCompiler m_modelCompiler;
    QTemporaryFile m_tmpModelFile;
    static MainWindow *sm_instance;
public:
    static MainWindow *get() {
        return sm_instance;
    }
};

#endif
