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
/*
 * http://qt-project.org/forums/viewthread/25714
 */
#include <QCoreApplication>
#include <QtWidgets>
#include <iostream>
#include "mainwindow.h"

#include "propertyeditor/clonedproperties.h"

// Carrega o estilo do arquivo de recursos e retorna em uma QString
QString loadAppStyle(const QString &name)
{
    QFile file(name);
    file.open(QFile::ReadOnly);

    return QString(file.readAll());
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // carrega nossas customizações
#if 0
    app.setStyleSheet(loadAppStyle(":/images/styler.css"));
#elif 1
    app.setStyle("fusion");
#endif

    MainWindow *window = new MainWindow;
    window->show();

    return app.exec();
}
