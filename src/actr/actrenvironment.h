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
#ifndef ACTRENVIRONMENT_H
#define ACTRENVIRONMENT_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QMap>
#include <functional>

class ActrManager;

class ActrEnvironment : public QObject
{
    Q_OBJECT
public:
    explicit ActrEnvironment(ActrManager *parent = 0);

    void sendEnvironmentCmd(const QString& cmd);

signals:

public slots:
    void loadModel();
    void loadModel(const QString& name);
    void runModel();
private slots:
    void newConnection();
    void sendAliveConnectionAck();
    void readReady();
private:
    void setupLoadModel();
    void setupEnvironmentWindow();
    void processEnvironmentWindow(QString cmd, QString args);
    void dispatchLastCommand();
    bool isValidConnection();
    short m_port;
    void initEnvironment();
    QTcpServer* m_server;
    QTcpSocket* m_socket;
    QTimer* m_timer;
    QString m_lastCommand;
    QMap<QString, QString> m_handlers;
    QMap<QString, std::function<void(QString, QString, QString)>> m_dispatcher;
    ActrManager *m_manager;

};

#endif // ACTRENVIRONMENT_H
