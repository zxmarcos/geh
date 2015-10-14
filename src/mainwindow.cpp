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
#include <QUndoStack>
#include "mainwindow.h"
#include "aboutdialog.h"
#include "toolbox.h"
#include "designersurface/designersurface.h"
#include "propertyeditor/propertyeditor.h"
#include "gerador/sequencecompiler.h"
#include "highlighter.h"
#include "actr/actrmanager.h"
#include "taskbuilder/taskbuilder.h"
#include "devices/virtualkeyboard.h"
#include "erroreditor.h"

MainWindow *MainWindow::sm_instance = nullptr;

MainWindow::MainWindow()
{
    setWindowTitle(tr("HEG"));
    setContentsMargins(0, 0, 0, 0);
    sm_instance = this;

    m_appUndoStack = new QUndoStack(this);

    createActions();
    createMenus();
    createToolbars();

    createControls();

    statusBar();
    m_aboutDialog = NULL;

    m_propertyEditor->setMessage(tr("Nenhum controle selecionado."));

    connectActions();

    if (!m_actrManager->init()) {
        QMessageBox::critical(this, "Erro", "<html>O SBCL (compilador lisp) não foi encontrado!<br/>"
                              "Verifique se <b>sbcl</b> está no PATH.</html>",
                              QMessageBox::Ok);
    }
}

MainWindow::~MainWindow()
{
    m_actrManager->kill();
}

// =============================================================================
// Cria as ações da aplicação
// =============================================================================
void MainWindow::createActions()
{
    // instancia os objetos
    m_newProjectAction = new QAction(QIcon(tr(":/images/action/new.png")),
                                     tr("&Novo"), this);
    m_newProjectAction->setStatusTip(tr("Cria um novo projeto"));
    m_newProjectAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));

    m_openProjectAction = new QAction(QIcon(tr(":/images/action/open.png")),
                                      tr("&Abrir"), this);
    m_openProjectAction->setStatusTip(tr("Carrega um projeto"));
    m_openProjectAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));

    m_saveProjectAction = new QAction(QIcon(tr(":/images/action/save.png")),
                                      tr("&Salvar"), this);
    m_saveProjectAction->setStatusTip(tr("Salva o projeto atual"));
    m_saveProjectAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    m_closeProjectAction = new QAction(QIcon(tr(":/images/action/close.png")),
                                       tr("&Fechar"), this);
    m_closeProjectAction->setStatusTip(tr("Fecha o projeto atual"));
    m_closeProjectAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_W));

    m_closeApplicationAction = new QAction(QIcon(tr(":/images/action/exit.png")),
                                           tr("&Sair"), this);
    m_closeApplicationAction->setStatusTip(tr("Fecha a aplicação"));

    m_viewToolboxAction = new QAction(tr("&Caixa de Ferramentas"), this);
    m_viewToolboxAction->setStatusTip(tr("Exibe a caixa de ferramentas"));

    m_viewPropertyEditorAction = new QAction(tr("&Editor de Propriedades"), this);
    m_viewPropertyEditorAction->setStatusTip(tr("Exibe a caixa de propriedades"));

    m_viewGridAction = new QAction(tr("Exibir &Grade"), this);
    m_viewGridAction->setStatusTip(tr("Exibe a grade no formulário"));
    m_viewGridAction->setCheckable(true);
    m_viewGridAction->setChecked(true);

    m_snapGridAction = new QAction(tr("&Alinhar a grade"), this);
    m_snapGridAction->setStatusTip(tr("Habilita o alinhamento a grade"));
    m_snapGridAction->setCheckable(true);
    m_snapGridAction->setChecked(true);

    m_editModeAction = new QAction(QIcon(tr(":/images/action/application_form_edit.png")),
                                   tr("Modo de &Edição"), this);
    m_editModeAction->setStatusTip(tr("Habilita o modo de edição"));
    m_editModeAction->setCheckable(true);
    m_editModeAction->setChecked(true);
    m_editModeAction->setIconVisibleInMenu(false);

    m_undoAction = new QAction(QIcon(tr(":/images/action/arrow_undo.png")),
                               tr("Desfazer"), this);
    m_undoAction->setShortcuts(QKeySequence::Undo);
    m_undoAction->setStatusTip(tr("Desfaz a última operação"));

    m_redoAction = new QAction(QIcon(tr(":/images/action/arrow_redo.png")),
                               tr("Refazer"), this);
    m_redoAction->setShortcuts(QKeySequence::Redo);
    m_redoAction->setStatusTip(tr("Refaz a última operação"));


    m_aboutApplicationAction = new QAction(tr("&Sobre"), this);
    m_aboutApplicationAction->setStatusTip(
                tr("Sobre o projeto Sistema Gerador de Erro Humano"));


    m_aboutQtAction = new QAction(QIcon(tr(":/images/action/qtlogo.png")),
                                  tr("Sobre o &Qt..."), this);
    m_aboutQtAction->setStatusTip(
                tr("Sobre o toolkit utilizado para desenvolver essa aplicação"));

    m_generateErrors = new QAction(QIcon(tr(":/images/action/cog_go.png")), tr("&Gerar"), this);
    m_generateErrors->setStatusTip(tr("Gera a lista de erros baseado na tarefa descrita"));

    m_runSimulation = new QAction(tr("&Simular"), this);
    m_runSimulation->setStatusTip(tr("Simula a tarefa"));


    m_openTaskBuilder = new QAction(QIcon(tr(":/images/action/bricks.png")),
                                    tr("&Criar tarefa"), this);


}

// =============================================================================
// Cria os menus da aplicação
// =============================================================================
void MainWindow::createMenus()
{
    m_projectMenu = menuBar()->addMenu(tr("&Projeto"));
    m_projectMenu->addAction(m_newProjectAction);
    m_projectMenu->addAction(m_openProjectAction);
    m_projectMenu->addAction(m_saveProjectAction);
    m_projectMenu->addAction(m_closeProjectAction);
    m_projectMenu->addSeparator();
    m_projectMenu->addAction(m_closeApplicationAction);

    m_editMenu = menuBar()->addMenu(tr("&Editar"));
    m_editMenu->addAction(m_undoAction);
    m_editMenu->addAction(m_redoAction);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_editModeAction);

    m_errorMenu = menuBar()->addMenu(tr("E&rros"));
    m_errorMenu->addAction(m_openTaskBuilder);
    m_errorMenu->addAction(m_generateErrors);
    m_errorMenu->addAction(m_runSimulation);

    m_viewMenu = menuBar()->addMenu(tr("&Visualizar"));
    m_viewMenu->addAction(m_viewToolboxAction);
    m_viewMenu->addAction(m_viewPropertyEditorAction);
    m_viewMenu->addSeparator();
    m_viewMenu->addAction(m_viewGridAction);
    m_viewMenu->addAction(m_snapGridAction);

    m_aboutMenu = menuBar()->addMenu(tr("&Sobre"));
    m_aboutMenu->addAction(m_aboutApplicationAction);
    m_aboutMenu->addAction(m_aboutQtAction);
}

// =============================================================================
// Cria a barra de ferramentas
// =============================================================================
void MainWindow::createToolbars()
{
    m_projectToolbar = addToolBar(tr("Projeto"));
    m_projectToolbar->setMovable(false);
    m_projectToolbar->setFloatable(false);
    m_projectToolbar->setIconSize(QSize(20, 20));

    m_projectToolbar->addAction(m_newProjectAction);
    m_projectToolbar->addAction(m_openProjectAction);
    m_projectToolbar->addAction(m_saveProjectAction);
    m_projectToolbar->addAction(m_closeProjectAction);

    m_projectToolbar->addSeparator();
    m_projectToolbar->addAction(m_undoAction);
    m_projectToolbar->addAction(m_redoAction);

    m_projectToolbar->addAction(m_editModeAction);
    m_projectToolbar->addAction(m_generateErrors);
    m_projectToolbar->addAction(m_openTaskBuilder);

}

// =============================================================================
// Cria os controles
// =============================================================================
void MainWindow::createControls()
{
    QWidget *container = new QWidget;
    container->setObjectName("TabContainer");

    QSplitter *splitter = new QSplitter(Qt::Vertical, container);

    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(splitter);

    m_designerArea = new QScrollArea;
    m_designerArea->setAutoFillBackground(true);
    m_designerArea->setBackgroundRole(QPalette::Shadow);
    m_designer = new DesignerSurface(this);
    m_designerArea->setWidget(m_designer);
    // temos que redefinir nossa DesignerSurface como SubWindow
    m_designer->setWindowFlags(Qt::SubWindow);
    m_designer->show();
    m_designer->resize(350, 300);

    m_upperTabs = new QTabWidget();
    m_upperTabs->setTabPosition(QTabWidget::West);
    m_upperTabs->addTab(m_designerArea, "Designer");


    QFont fixedFont("Ubuntu Mono");// = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    fixedFont.setPointSize(12);

    m_lispOutputModel = new QPlainTextEdit(this);
    m_lispOutputModel->setReadOnly(true);
    m_lispOutputModel->setFont(fixedFont);
    m_upperTabs->addTab(m_lispOutputModel, "Model");

    splitter->addWidget(m_upperTabs);

    m_bottomTabs = new QTabWidget();
    m_bottomTabs->setTabPosition(QTabWidget::West);
    splitter->addWidget(m_bottomTabs);

    m_editor = new QTextEdit;
    fixedFont.setPointSize(13);
    QFontMetrics mt(fixedFont);
    m_editor->setFont(fixedFont);
    m_editor->setTabStopWidth(mt.charWidth("x", 0) * 4);

    m_ldtHighlighter = new Highlighter(m_editor->document());

    m_bottomTabs->addTab(m_editor, "Editor");

    m_actrManager = new ActrManager(this);
    m_bottomTabs->addTab(m_actrManager, "Console ACT-R");

    m_virtKeyboard = new VirtualKeyboard(this);
    m_bottomTabs->addTab(m_virtKeyboard, "Devices");

    m_desktop = new VirtualDesktop(m_designerArea);
    m_designerArea->installEventFilter(m_desktop);
    m_virtMouse = new VirtualMouse(m_desktop);
    m_virtMouse->move(m_desktop->rect().bottomRight());

    setCentralWidget(container);

    // cria a caixa de ferramentas
    m_dockToolbox = new QDockWidget(tr(" ToolBox"), this);
    m_toolbox = new ToolBox(this);
    m_dockToolbox->setAllowedAreas(Qt::LeftDockWidgetArea |
                                   Qt::RightDockWidgetArea);
    m_dockToolbox->setWidget(m_toolbox);
    addDockWidget(Qt::RightDockWidgetArea, m_dockToolbox);
    m_dockToolbox->setMinimumWidth(120);

    // cria o editor de propriedades
    m_dockPropertyEditor = new QDockWidget(tr(" Properties"), this);
    m_propertyEditor = new PropertyEditor(this);
    m_dockPropertyEditor->setAllowedAreas(Qt::LeftDockWidgetArea |
                                          Qt::RightDockWidgetArea);
    m_dockPropertyEditor->setWidget(m_propertyEditor);
    addDockWidget(Qt::RightDockWidgetArea, m_dockPropertyEditor);
    m_dockPropertyEditor->setMinimumWidth(120);

    m_taskBuilder = new TaskBuilder(this);

    m_errorEditor = new ErrorEditor(this);
}

// =============================================================================
// Connecta as ações
// =============================================================================
void MainWindow::connectActions()
{
    connect(m_designer, SIGNAL(widgetsDeleted(QList<QWidget*>)),
            m_propertyEditor, SLOT(checkDeletedWidgets(QList<QWidget*>)));
    connect(m_designer, SIGNAL(widgetsModified(QList<QWidget*>)),
            m_propertyEditor, SLOT(widgetsModified(QList<QWidget*>)));

    // conecta a ação de exibir a caixa de ferramentas
    connect(m_viewToolboxAction, SIGNAL(triggered()),
            m_dockToolbox, SLOT(show()));

    // conecta a ação de exibir a caixa de propriedades
    connect(m_viewPropertyEditorAction, SIGNAL(triggered()),
            m_dockPropertyEditor, SLOT(show()));

    connect(m_viewGridAction, SIGNAL(toggled(bool)),
            m_designer, SLOT(setGrid(bool)));

    connect(m_snapGridAction, SIGNAL(toggled(bool)),
            m_designer, SLOT(setSnapToGrid(bool)));

    connect(m_editModeAction, SIGNAL(toggled(bool)),
            m_designer, SLOT(setEditMode(bool)));

    connect(m_designer, SIGNAL(selectedNoneWidget(QString)),
            m_propertyEditor, SLOT(setMessage(QString)));
    connect(m_designer, SIGNAL(selectedMultipleWidgets(QString)),
            m_propertyEditor, SLOT(setMessage(QString)));
    connect(m_designer, SIGNAL(selectedOneWidget(QWidget*)),
            m_propertyEditor, SLOT(setWidget(QWidget*)));

    connect(m_newProjectAction, SIGNAL(triggered()),
            m_designer, SLOT(showTree()));

    connect(m_saveProjectAction, SIGNAL(triggered()),
            this, SLOT(saveProject()));
    connect(m_openProjectAction, SIGNAL(triggered()),
            this, SLOT(loadProject()));

    connect(m_closeProjectAction, SIGNAL(triggered()),
            m_designer, SLOT(closeDocument()));
    // Conecta as ações
    connect(m_closeApplicationAction, SIGNAL(triggered()),
            this, SLOT(close()));

    connect(m_aboutApplicationAction, SIGNAL(triggered()),
            this, SLOT(about()));

    connect(m_aboutQtAction, SIGNAL(triggered()),
            qApp, SLOT(aboutQt()));

    connect(m_redoAction, SIGNAL(triggered()),
            m_appUndoStack, SLOT(redo()));

    connect(m_undoAction, SIGNAL(triggered()),
            m_appUndoStack, SLOT(undo()));

    connect(m_generateErrors, SIGNAL(triggered()),
            this, SLOT(generate()));

    connect(m_runSimulation, SIGNAL(triggered(bool)),
            this, SLOT(run()));

    connect(m_openTaskBuilder, SIGNAL(triggered()),
            m_taskBuilder, SLOT(exec()));

}

void MainWindow::saveProject()
{
    QString name = QFileDialog::getSaveFileName(this, tr("Salvar projeto..."),
                                                "", "*.xml");
    if (!name.isEmpty()) {
        if (!name.endsWith(".xml"))
            name.append(".xml");
        m_designer->saveDocument(name);
    }
}

void MainWindow::loadProject()
{
    QString name = QFileDialog::getOpenFileName(this, tr("Abrir projeto..."),
                                                "", "*.xml");
    if (!name.isEmpty()) {
        m_designer->loadDocument(name);
    }
}

void MainWindow::generate()
{
    m_seqCompiler.setProgram(m_editor->toPlainText());
    TaskSequence sequence = m_seqCompiler.compile();
    QStringList errors = m_seqCompiler.errors();
    qDebug() << errors;
    if (errors.size()) {
        QMessageBox::critical(this, "Erro", "Foram encontrados erros no programa que descreve a tarefa!");
    } else {
        m_errorEditor->exec(&sequence);

        m_modelCompiler.compile(sequence);
        const QString fileName = "geh-output-model.lisp";
        QFile file(fileName);
        file.open(QFile::Truncate | QFile::WriteOnly);
        QFileInfo info(file);
        QTextStream out(&file);
        out << m_modelCompiler.result();
        file.close();
        m_lispOutputModel->clear();
        m_lispOutputModel->insertPlainText(m_modelCompiler.result());
        m_actrManager->loadModel(info.absoluteFilePath());
    }
}

void MainWindow::run()
{
    m_desktop->clear();
    m_editModeAction->setChecked(false);
    m_actrManager->run();
}

// =============================================================================
// Exibe o diálogo de informações da aplicação.
// =============================================================================
void MainWindow::about()
{
    if (!m_aboutDialog)
        m_aboutDialog = new AboutDialog(this);
    m_aboutDialog->show();
}

// =============================================================================
// Evento recebido ao receber a notificação para fechar o programa
// =============================================================================
void MainWindow::closeEvent(QCloseEvent *event)
{
#if 0
    if (QMessageBox::question(this, tr("Sair?"),
                              tr("Tem certeza que deseja sair?")) == QMessageBox::Yes) {
        event->accept();
    } else
        event->ignore();
#else
    event->accept();
#endif
}


// =============================================================================
// Retorna a pilha de comandos.
// =============================================================================
QUndoStack *MainWindow::undoStack()
{
    return m_appUndoStack;
}

DesignerSurface *MainWindow::designer()
{
    return m_designer;
}

PropertyEditor *MainWindow::propertyEditor()
{
    return m_propertyEditor;
}

QTextEdit *MainWindow::editor()
{
    return m_editor;
}

ToolBox *MainWindow::toolBox()
{
    return m_toolbox;
}
