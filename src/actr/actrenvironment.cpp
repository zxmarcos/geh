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
#include "actrenvironment.h"
#include "actrmanager.h"
#include <QApplication>
#include <QMessageBox>

// =============================================================================
// ctor
// =============================================================================
ActrEnvironment::ActrEnvironment(ActrManager *parent) :
    QObject(parent),
    m_socket(nullptr)
{
    m_server = new QTcpServer(this);
    m_timer = new QTimer(this);
    m_port = 2621;
    m_manager = parent;

    initEnvironment();
}


// =============================================================================
// Envia um comando para o Act-R
// =============================================================================
void ActrEnvironment::sendEnvironmentCmd(const QString &cmd)
{
    if (isValidConnection()) {
        QString x = QString("(%1)<end>\n").arg(cmd);
        qDebug() << "SEND:"<< x.toLocal8Bit();
        m_socket->write(x.toLocal8Bit());
        m_socket->flush();
    }
}


// =============================================================================
// Aceita a conexão
// =============================================================================
void ActrEnvironment::newConnection()
{
    if (isValidConnection()) {
        qDebug() << "Conexão com o ambiente não aceita, pois já existe uma ativa";
        return;
    }
    m_socket = m_server->nextPendingConnection();

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readReady()));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(sendAliveConnectionAck()));
    m_timer->setInterval(15000);
//    m_timer->start();

    qDebug() << "Conexão aceita com o ambiente do ACT-R 6!";
    qDebug() << "Configurando o ambiente...";
    setupEnvironmentWindow();
    setupLoadModel();
    qDebug() << "Ambiente configurado";
}


// =============================================================================
// Envia um sinal em um período regular para manter a conexão ativa
// =============================================================================
void ActrEnvironment::sendAliveConnectionAck()
{
    if (m_socket != nullptr) {
        m_socket->write("(k-a)<end>\n");
    }
}


// =============================================================================
// Lê os dados que chegaram e os processa
// =============================================================================
void ActrEnvironment::readReady()
{
    QRegExp isCmd("(.*)<end>[\n\r]*$");
    int lineCounter = 1;

    while (m_socket->bytesAvailable()) {
        QString line = m_socket->readLine();
        //qDebug() << QString::number(lineCounter++).append(':') << line;
        if (isCmd.indexIn(line) != -1) {
            m_lastCommand += isCmd.cap(1);
            dispatchLastCommand();
        }
    }
}


// =============================================================================
// Envia o último comando identificado
// =============================================================================
void ActrEnvironment::dispatchLastCommand()
{
    QRegExp parser("^([a-z]+)([ ]+)([^ ]+)([ ]+)(.+)$");
    qDebug() << m_lastCommand;
    if (parser.exactMatch(m_lastCommand)) {
        QString dummy = parser.cap(0);
        QString cmd = parser.cap(1);
        QString arg1 = parser.cap(3);
        QString rest = parser.cap(5);

        //qDebug() << "DUMMY:" << dummy;
        //qDebug() << "CMD:"<< cmd;
        //qDebug() << "ARG1:"<< arg1;
        //qDebug() << "REST:"<< rest;

        if (cmd == QString("register")) {
            m_handlers[arg1] = rest;
            qDebug() << "Registrando" << arg1 << "como" << rest;

        } else if (cmd == QString("update")) {
            QRegExp mat("([^ ]+)([ ]+)([^ ]+)(.*)$");
            if (mat.exactMatch(rest)) {
                QString name = mat.cap(1);
                QString value = mat.cap(3);
                if (m_dispatcher.find(name) != m_dispatcher.end()) {
                    m_dispatcher[name](arg1, value, mat.cap(4));
                }
            } else {
                qDebug() << "Não casou:" << rest;
            }
        } else if (cmd == QString("close")) {
            m_socket->close();
            m_server->close();
            qApp->exit();
        }
    }
    m_lastCommand = "";
}


// =============================================================================
// Verifica se a conexão é válida
// =============================================================================
bool ActrEnvironment::isValidConnection()
{
    if (m_socket == nullptr)
        return false;

    if (!m_socket->isValid())
        return false;
    return true;
}


// =============================================================================
// Inicia o ambiente
// =============================================================================
void ActrEnvironment::initEnvironment()
{
    if (!m_server->listen(QHostAddress::Any, m_port)) {
        QMessageBox::critical(nullptr, tr("Erro crítico"),
                              QString("Não foi possível iniciar o ambiente para comunicação"
                                 " com o Act-R através da porta %0, verifique se a porta já"
                                 " está sendo utilizada por outro programa!").arg(m_port));
        qApp->exit(-1);
    } else {
        qDebug() << "Ambiente para ACT-R 6 iniciado com sucesso!";
        qDebug() << "--> Esperando conexão...";
    }
    connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}
