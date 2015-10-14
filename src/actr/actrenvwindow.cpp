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
#include <QDebug>
#include "../devices/virtualmouse.h"
#include "../devices/virtualdesktop.h"
#include "actrenvironment.h"
#include "actrmanager.h"

static QString modelHandler = ".env_window";
static QString returnHandler = ".env_window";
static QString setupCommand =
"create env-window-handler ${HANDLER} ${RETURN}\
(lambda (x) \
    (declare (ignore x))    \
    (setf (environment-control-use-env-windows *environment-control*) t)\
    (list 'ignore)) ()";

void ActrEnvironment::setupEnvironmentWindow()
{
    QString cmd = setupCommand.replace("${HANDLER}", modelHandler)
                              .replace("${RETURN}", returnHandler);
    sendEnvironmentCmd(cmd);

    m_dispatcher[returnHandler] = [&](QString type, QString rest, QString args) {
        processEnvironmentWindow(rest, args);
    };
}


void ActrEnvironment::processEnvironmentWindow(QString cmd, QString args)
{
    qDebug() << cmd << args;
    if (cmd == QString("cursor")) {
        QRegExp mat("\\s*(\".*\")\\s+(\\d+)\\s+(\\d+).*$");
        if (mat.exactMatch(args)) {
            int x = mat.cap(2).toInt();
            int y = mat.cap(3).toInt();
            qDebug() << "Mover cursor" << x << y;
            VirtualMouse::get()->moveTo(x, y);
        }
    } else
    if (cmd == QString("attention")) {
        QRegExp mat("\\s*(\".*\")\\s+(\\d+)\\s+(\\d+).*$");
        if (mat.exactMatch(args)) {
            int x = mat.cap(2).toInt();
            int y = mat.cap(3).toInt();
            qDebug() << "Olhar para" << x << y;
            VirtualDesktop::get()->lookAt(QPoint(x, y));
        }
    } else
    if (cmd == QString("clearattention")) {
        qDebug() << "Limpar visão";
        VirtualDesktop::get()->clearVision();
    }
}
