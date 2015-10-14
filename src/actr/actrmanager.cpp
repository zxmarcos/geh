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
#include <QtWidgets>
#include "actrmanager.h"
#include "../devices/virtualmouse.h"
#include "../devices/virtualkeyboard.h"

ActrManager::ActrManager(QWidget *parent) :
    QWidget(parent),
    m_proc(nullptr)
{
    createActions();
    createControls();
    connectActions();
}

ActrManager::~ActrManager()
{
    // Verifica se o processo ainda existe
    if (m_proc) {
        m_proc->write(":abort\n(quit)\n");
        if (!m_proc->waitForFinished()) {
            qDebug() << "Não foi possível terminar o sbcl!";
            m_proc->kill();
        }
    }
}

// =============================================================================
// Inicia o SBCL com o ACT-R
// =============================================================================
bool ActrManager::init()
{
    m_proc = new QProcess(this);
    m_proc->setArguments(QStringList()
                         << "--noinform"
                         // << "--noprint"
                         << "--eval" << "(setf *muffled-warnings* t)"
                         << "--load" << "actr6/load-act-r-6.lisp"
                         );
    m_proc->setProgram("sbcl");
    connect(m_proc, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
    connect(m_proc, SIGNAL(readyReadStandardError()), this, SLOT(readError()));

    connect(m_input, SIGNAL(returnPressed()), this, SLOT(writeInput()));
    m_proc->start();
    if (!m_proc->waitForStarted()) {
        qDebug() << "Falha ao iniciar o SBCL com o ACT-R 6";
        delete m_proc;
        m_proc = nullptr;
        return false;
    }

    // Envia o comando para estabelecer a conexão com o ambiente
    writeInput("(start-environment)");

    qDebug() << "SBCL e ACT-R 6 iniciados com sucesso!";
    connect(m_send, SIGNAL(clicked()), this, SLOT(writeInput()));

    return true;
}


// =============================================================================
// Mata o processo filho
// =============================================================================
void ActrManager::kill()
{
    if (m_proc)
        m_proc->kill();
}


// =============================================================================
// Lê a saída padrão (stdout)
// =============================================================================
void ActrManager::readOutput()
{
    while (m_proc->canReadLine()) {
        m_output->setTextColor(Qt::black);
        m_output->setFontWeight(QFont::Normal);
        QString all = m_proc->readLine();
        processInput(all);
        m_output->append(all);
    }
}


// =============================================================================
// Lê a saída de erros (stderr)
// =============================================================================
void ActrManager::readError()
{
    m_output->setTextColor(Qt::red);
    m_output->append(m_proc->readAllStandardError());
}

// =============================================================================
// Escreve na entrada padrão (stdin)
// =============================================================================
void ActrManager::writeInput()
{
    if (m_proc) {
        QString s(m_input->text() + QString("\n"));
        m_output->setTextColor(Qt::darkBlue);
        m_output->append(s);
        m_proc->write(s.toLatin1());
        m_input->clear();
    }
}

void ActrManager::writeInput(QString str)
{
    if (m_proc) {
        str.append("\n");
        m_output->setTextColor(Qt::darkBlue);
        m_output->append(str);
        m_proc->write(str.toLatin1());
    }
}


// =============================================================================
// Carrega um modelo no ACT-R
// =============================================================================
bool ActrManager::loadModel(QString name)
{
    m_environment->loadModel(name);
#if 0
    if (!m_proc)
        return false;
    QString s = QString("(load \"%1\")\n").arg(name);
    m_output->setTextColor(Qt::darkBlue);
    m_output->append(s);
    m_proc->write(s.toLatin1());
#endif
    return true;
}



// =============================================================================
// Abre um modelo no ACT-R
// =============================================================================
bool ActrManager::openModel()
{
#if 1
    m_environment->loadModel();
    return true;
#else
    QString name = QFileDialog::getOpenFileName(this, "Carregar modelo", "", "*.lisp");
    if (!name.isEmpty()) {
        return loadModel(name);
    }
    return false;
#endif
}

// =============================================================================
// Executa o modelo no ACT-R
// =============================================================================
void ActrManager::runModel(int ticks)
{
    writeInput(QString("(run %1)").arg(ticks));
}


// =============================================================================
// Limpa o console
// =============================================================================
void ActrManager::clear()
{
    m_output->clear();
}

void ActrManager::run()
{
    m_environment->runModel();
}


// =============================================================================
// Cria as ações
// =============================================================================
void ActrManager::createActions()
{
    m_openModel = new QAction(QIcon(tr(":/images/action/brick.png")),
                              tr("&Abrir"), this);
    m_openModel->setStatusTip(tr("Carrega um modelo cognitivo"));

    m_runModel = new QAction(QIcon(tr(":/images/action/brick_go.png")),
                             tr("&Executar"), this);
    m_runModel->setStatusTip(tr("Executa o modelo cognitivo"));

    m_clearConsole = new QAction(QIcon(tr(":/images/action/cross.png")),
                                 tr("&Limpar"), this);
    m_clearConsole->setStatusTip(tr("Limpa o console"));
}



// =============================================================================
// Cria os controles
// =============================================================================
void ActrManager::createControls()
{
    m_output = new QTextEdit(this);
    m_output->setReadOnly(true);
    QFont fixedFont("Ubuntu Mono");// = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    fixedFont.setPointSize(12);
    QFontMetrics mt(fixedFont);
    m_output->setFont(fixedFont);
    m_output->setTabStopWidth(mt.charWidth("x", 0) * 4);


    m_input = new QLineEdit(this);
    m_send = new QPushButton("Enviar");
    m_toolBar = new QToolBar("ACT-R", this);
    m_toolBar->setIconSize(QSize(20, 20));

    m_environment = new ActrEnvironment(this);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_toolBar);
    layout->addWidget(m_output, 1);
    QHBoxLayout *bottom = new QHBoxLayout();
    bottom->addWidget(m_input, 1);
    bottom->addWidget(m_send);
    layout->addLayout(bottom);
    layout->setMargin(3);
    setLayout(layout);

    m_toolBar->addAction(m_openModel);
    m_toolBar->addAction(m_runModel);
    m_toolBar->addSeparator();
    m_toolBar->addAction(m_clearConsole);
}



// =============================================================================
// Conecta as ações
// =============================================================================
void ActrManager::connectActions()
{
    connect(m_openModel, SIGNAL(triggered()), this, SLOT(openModel()));
    connect(m_runModel, SIGNAL(triggered()), this, SLOT(runModel()));
    connect(m_clearConsole, SIGNAL(triggered()), this, SLOT(clear()));
}

void ActrManager::processInput(const QString &str)
{
    QRegularExpression re("%%geh-send-event\\[(.*)\\]%%");
    QRegularExpression re_info("#@.*@#");

    if (re.match(str).hasMatch()) {
        m_output->setTextColor(Qt::blue);
        m_output->setFontWeight(QFont::Bold);
        static const QString mouseClick("mouse-click");
        QRegularExpression kpre("key-press,(.*)");
        QString cmd = re.match(str).captured(1);
        qDebug() << "Evento" << cmd;
        if (cmd == mouseClick) {
            VirtualMouse::get()->click();
        } else if (kpre.match(cmd).hasMatch()) {
            VirtualKeyboard::get()->press(kpre.match(cmd).captured(1));
        }

    } else if (re_info.match(str).hasMatch()) {
        m_output->setTextColor(Qt::red);
        m_output->setFontWeight(QFont::Bold);
    }

}


