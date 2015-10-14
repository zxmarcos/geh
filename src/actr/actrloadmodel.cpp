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
#include "actrenvironment.h"
#include "actrmanager.h"

static QString modelHandler = "load_model";
static QString returnHandler = "load_model_return";
static QString setupCommand =
"create list-handler ${HANDLER} ${RETURN} (lambda (x)) ()";

static QString runCommand =
"update ${HANDLER} (lambda (x) (declare (ignore x))"
    "(progn (geh-start-task) (format t \"%%geh-send-event[end]%%\")))";

static const char *loadCommand =
"update ${HANDLER}                                              \
    (lambda (x)                                                 \
        (declare (ignore x))                                    \
        (progn                                                  \
            (set-environment-busy)                              \
            (let ((result (safe-load \"${FILENAME}\" nil)))     \
                (set-environment-free)                          \
                (format t (if (= 1 (first result))              \
                    \"Modelo carregado com sucesso!\"           \
                    \"Falha ao carregar o modelo\"))            \
                result))                                        \
            (set-environment-free))";


static QString runModelHandler = "run_model";
static QString runReturnHandler = "run_model_return";
static QString runSetupCommand =
"create list-handler ${HANDLER} ${RETURN} (lambda (x)) ()";

void ActrEnvironment::setupLoadModel()
{
    QString cmd = setupCommand.replace("${HANDLER}", modelHandler)
                              .replace("${RETURN}", returnHandler);
    sendEnvironmentCmd(cmd);

    m_dispatcher[returnHandler] = [&](QString type, QString rest, QString args) {
        qDebug() << QString("Tipo:").append(type).append(", Valor:").append(rest);
    };

    cmd = runSetupCommand.replace("${HANDLER}", runModelHandler)
                         .replace("${RETURN}", runReturnHandler);
    sendEnvironmentCmd(cmd);

    m_dispatcher[runReturnHandler] = [&](QString type, QString rest, QString args) {
        qDebug() << QString("Tipo:").append(type).append(", Valor:").append(rest);
    };
}

void ActrEnvironment::loadModel()
{
    if (isValidConnection()) {
        QString name = QFileDialog::getOpenFileName(m_manager, "Modelo", "", "*.lisp");
        if (!name.isEmpty()) {
            QString cmd = QString(loadCommand)
                    .replace("${FILENAME}", name)
                    .replace("${HANDLER}", m_handlers[modelHandler])
                    .replace(QRegExp("\\s+")," ");
            sendEnvironmentCmd(cmd);
        }
    }
}

void ActrEnvironment::loadModel(const QString &name)
{
    if (isValidConnection()) {
        if (!name.isEmpty()) {
            QString cmd = QString(loadCommand)
                .replace("${FILENAME}", name)
                .replace("${HANDLER}", m_handlers[modelHandler])
                .replace(QRegExp("\\s+")," ");
            sendEnvironmentCmd(cmd);
        }
    }
}

void ActrEnvironment::runModel()
{
    if (isValidConnection()) {
        QString cmd = QString(runCommand)
            .replace("${HANDLER}", m_handlers[runModelHandler])
            .replace(QRegExp("\\s+")," ");
        sendEnvironmentCmd(cmd);
    }
}
