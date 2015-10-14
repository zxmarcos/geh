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
#ifndef QSTRINGLISTEDIT_H
#define QSTRINGLISTEDIT_H

#include <QDialog>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>

class QStringListEditDialog;

class QStringListEdit : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QStringList m_list READ list WRITE setList USER true)
public:
    explicit QStringListEdit(QWidget *parent = 0);
    QStringList list() const;

public slots:
    void setList(const QStringList &list);
private:
    QStringList m_list;
    QStringListEditDialog *m_editor;
};


class QStringListEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QStringListEditDialog(QWidget *parent = 0);

signals:

public slots:
private:
    QTextEdit *m_textEdit;
    QPushButton *m_ok;
    QPushButton *m_cancel;
};

#endif // QSTRINGLISTEDIT_H
