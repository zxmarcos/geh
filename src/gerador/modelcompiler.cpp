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
#include "modelcompiler.h"
#include "../designersurface/designersurface.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include "../mainwindow.h"
#include "../util.h"

ModelCompiler::ModelCompiler()
{
}

bool intersects(QPointF px, double r, QRectF rect)
{
    double dx = abs(px.x() - rect.x());
    double dy = abs(px.y() - rect.y());

    if (dx > (rect.width() / 2 + r)) { return false; }
    if (dy > (rect.height() / 2 + r)) { return false; }

    if (dx <= (rect.width() / 2)) { return true; }
    if (dy <= (rect.height() / 2)) { return true; }

    double dist = pow(dx - rect.width()/2, 2) +
            pow(dy - rect.height()/2,2);

    return (dist <= pow(r,2));
}

void ModelCompiler::compile(const TaskSequence &cseq)
{
    m_result.clear();
    m_result.reserve(1024*64);

    m_currentStep = 0;
    emitProlog();

    TaskSequence seq(cseq);

    // Processa as inversões de passos
    for (int i = 0; i < seq.size(); i++) {
        if (seq[i].error == HERR_INVERSION) {
            if ((i + 1) < seq.size()) {
                qSwap(seq[i], seq[i + 1]);
                i++;
            } else if ((i-1) >= 0) {
                qSwap(seq[i], seq[i - 1]);
            } else {
                QMessageBox::critical(MainWindow::get(), "Error",
                                      "Não é possível realizar a inversão do passo");
                return;
            }
        }
    }

    foreach (SequenceNode node, seq) {
        QWidget *object = DesignerSurface::get()->findChild<QWidget*>(node.object);
        if (!object) {
            QMessageBox::critical(MainWindow::get(), "Error",
                                  QString("Não é possível encontrar o componente %1").arg(node.object));
            return;
        }

        switch (node.error) {
        case HERR_OMISSION:
            emitOmission(node);
            continue;
        case HERR_REPEAT:
            emitRepeat(node);
            break;
        case HERR_INVERSION:
            emitInversion(node);
            break;
        case HERR_PERCEPT_CONFUSION:
            emitPerceptConfusion(node);
            break;
        case HERR_REDUCTION_INTENT:
            emitReductionIntent(node);
            break;
        default:
            break;
        }
        switch (node.action) {
        case SequenceNode::FILL:
            emitFill(node);
            break;
        case SequenceNode::SELECT:
        case SequenceNode::CHECK:
        case SequenceNode::UNCHECK:
        case SequenceNode::PRESS:
        {
            emitLookAt(DesignerSurface::get()->findChild<QWidget*>(node.object));
            emitPress(node);
            break;
        }
        default:
            qDebug() << "ModelCompiler: Operator not implemented";
            break;
        }
    }

    emitEpilog();
    m_result = m_result.replace("\t", "  ");
}

QString ModelCompiler::result()
{
    return m_result;
}

void ModelCompiler::emitOmission(SequenceNode &node)
{
    QString msg = QString("#@ Omitindo passo: %1 @#").arg(node.describe());
    // Move o cursor
    m_result.append(QString("(P %0\n").arg(generateStepPName()));
    m_result.append(QString(
                        R"(
                        =goal>
            isa passo
            ordem %0
            ==>
            =goal>
            ordem %1
            !output!("%2")
            )"
                    ).arg(m_currentStep).arg(m_currentStep+1).arg(msg));
    m_result.append(")\n");
    m_currentStep++;
}

void ModelCompiler::emitRepeat(SequenceNode &node)
{
    switch (node.action) {
    case SequenceNode::FILL:
        emitFill(node);
        break;
    case SequenceNode::SELECT:
    case SequenceNode::CHECK:
    case SequenceNode::UNCHECK:
    case SequenceNode::PRESS:
        emitLookAt(DesignerSurface::get()->findChild<QWidget*>(node.object));
        emitPress(node);
        break;
    default:
        qDebug() << "ModelCompiler: Operator not implemented";
        break;
    }
    QString msg = QString("#@ Repetindo o passo: %1 @#").arg(node.describe());
    // Move o cursor
    m_result.append(QString("(P %0\n").arg(generateStepPName()));
    m_result.append(QString(
                        R"(
                        =goal>
            isa passo
            ordem %0
            ==>
            =goal>
            ordem %1
            !output!("%2")
            )"
                    ).arg(m_currentStep).arg(m_currentStep+1).arg(msg));
    m_result.append(")\n");
    m_currentStep++;
}

void ModelCompiler::emitInversion(SequenceNode &node)
{
    QString msg = QString("#@ Passo invertido: %1 @#").arg(node.describe());
    // Move o cursor
    m_result.append(QString("(P %0\n").arg(generateStepPName()));
    m_result.append(QString(
                        R"(
                        =goal>
            isa passo
            ordem %0
            ==>
            =goal>
            ordem %1
            !output!("%2")
            )"
                    ).arg(m_currentStep).arg(m_currentStep+1).arg(msg));
    m_result.append(")\n");
    m_currentStep++;
}

void ModelCompiler::emitPerceptConfusion(SequenceNode &node)
{
    QList<QWidget*> widgets = DesignerSurface::get()->formWidgets();
    QWidget *object = DesignerSurface::get()->findChild<QWidget*>(node.object);
    QVector<QWidget*> others;

    const QString klass(object->metaObject()->className());
    foreach (QWidget *w, widgets) {
        if (QString(w->metaObject()->className()) == klass)
            others.push_back(w);
    }

    QVector<QWidget*> cases;
    foreach (QWidget *w, others) {
        if (intersects(object->geometry().center(), 100, w->geometry())) {
            if (w != object) {
                cases.push_back(w);
            }
        }
    }

    qDebug() << "Componentes eleitos para a confusão perceptiva: ";
    foreach(QWidget *w, cases) {
        qDebug() << w->objectName();
    }

    if (cases.empty()) {
        QMessageBox::information(MainWindow::get(), "Informação",
                                 QString("Não é possivel realizar o erro de confusão perceptiva no %0, "
                                         "não existem objetos parecidos dentro do radio de 200 pixels").arg(node.object));
        return;
    }

    SequenceNode original(node);
    node.object = cases[qrand() % cases.size()]->objectName();
    QString msg = QString("#@ Confusao perceptiva: %1 -> %2 @#").arg(original.describe()).arg(node.describe());
    // Move o cursor
    m_result.append(QString("(P %0\n").arg(generateStepPName()));
    m_result.append(QString(
                        R"(
                        =goal>
            isa passo
            ordem %0
            ==>
            =goal>
            ordem %1
            !output!("%2")
            )"
                    ).arg(m_currentStep).arg(m_currentStep+1).arg(msg));
    m_result.append(")\n");
    m_currentStep++;

}

void ModelCompiler::emitReductionIntent(SequenceNode &node)
{
    QString msg = QString("#@ Redução de intencionalidade: %1 @#").arg(node.describe());
    // Move o cursor
    m_result.append(QString("(P %0\n").arg(generateStepPName()));
    m_result.append(QString(
                        R"(
                        =goal>
            isa passo
            ordem %0
            ==>
            =goal>
            ordem -1
            !output!("%1")
            )"
                    ).arg(m_currentStep).arg(msg));
    m_result.append(")\n");
    m_currentStep++;
}

void ModelCompiler::emitProlog()
{
    {
        QFile file(":/lisp/geh-device.lisp");
        file.open(QFile::ReadOnly);
        m_result.append(file.readAll());
    }

    m_result.append(R"(
                    (defun geh-start-task()
                     (reset)
                     (let* ((window (open-exp-window "geh-window")))
                      (add-items-to-exp-window
                       )");

                      emitForm();
                     m_result.append(R"(
                                     )
                     (install-device window)
                     (proc-display)
                     (print-visicon)
                     (run 1000 :real-time t)))
            )");

            m_result.append(
                R"(
                (define-model geh-tarefa

                 (chunk-type passo ordem tipo estado tecla posicao widget)

                 (sgp :v t :needs-mouse t :show-focus t :vwt t :trace-detail medium)
                 )");
                emitDM();

}

void ModelCompiler::emitForm()
{
    QList<QWidget*> widgets = DesignerSurface::get()->formWidgets();

    if (widgets.empty()) {
        m_result.append("nil");
        return;
    }
    foreach (QWidget *w, widgets) {
        m_result.append(QString("(make-instance 'geh-widget :x-pos %1 :y-pos %2 :width %3 :height %4)\n")
                        .arg(w->x()).arg(w->y()).arg(w->width()).arg(w->height()));
    }
}

void ModelCompiler::emitFormLocs()
{
    QList<QWidget*> widgets = DesignerSurface::get()->formWidgets();

    foreach (QWidget *w, widgets) {
        m_result.append(
                    QString("\t(%1 isa visual-location screen-x %2 screen-y %3)\n")
                    .arg(w->objectName()).arg(w->x()).arg(w->y()));
    }
}

void ModelCompiler::emitDM()
{
    m_result.append("(add-dm\n");
    emitFormLocs();
    m_result.append("\t(meta isa passo ordem 0))\n");
}

void ModelCompiler::emitEpilog()
{
    //    emitGehCommon();
    m_result.append("\n(goal-focus meta)\n)\n");
}

void ModelCompiler::emitFill(SequenceNode& node)
{
    emitLookAt(DesignerSurface::get()->findChild<QWidget*>(node.object));
    emitPress(node);

    // Coloca a mão da posição de origem
    m_result.append(QString("(P %0\n").arg(generateStepPName()));
    m_result.append(QString(
                        R"(
                        =goal>
            isa passo
            ordem %0
            ?manual>
             state free
             ==>
             =goal>
            ordem %1
            +manual>
            isa hand-to-home
            )"
                    ).arg(m_currentStep).arg(m_currentStep+1));
    m_result.append(")\n");
    m_currentStep++;

    for (int i = 0; i < node.arg.size(); i++) {
        // Não teclamos o espaço
        if (node.arg.at(i) == ' ')
            continue;
        m_result.append(QString("(P %0\n").arg(generateStepPName()));
        m_result.append(QString(
                            R"(
                            =goal>
                isa passo
                ordem %0
                ?manual>
                 state free
                 ==>
                 =goal>
                ordem %1
                +manual>
                isa press-key
                key "%3"
                )"
                        ).arg(m_currentStep).arg(m_currentStep+1).arg(node.arg.at(i)));
        m_result.append(")\n");
        m_currentStep++;
    }
}

void ModelCompiler::emitPress(SequenceNode &node)
{
    emitHandToMouse();

    // Move o cursor
    m_result.append(QString("(P %0\n").arg(generateStepPName()));
    m_result.append(QString(
                        R"(
                        =goal>
            isa passo
            ordem %0
            ?manual>
             state free
             ==>
             =goal>
            ordem %1
            +manual>
            isa move-cursor
            loc %2
            )"
                    ).arg(m_currentStep).arg(m_currentStep+1).arg(node.object));
    m_result.append(")\n");
    m_currentStep++;

    // Realiza o click
    m_result.append(QString("(P %0\n").arg(generateStepPName()));
    m_result.append(QString(
                        R"(
                        =goal>
            isa passo
            ordem %0
            ?manual>
             state free
             ==>
             =goal>
            ordem %1
            +manual>
            isa click-mouse
            )"
                    ).arg(m_currentStep).arg(m_currentStep+1));
    m_result.append(")\n");
    m_currentStep++;
}

void ModelCompiler::emitLookAt(QWidget *widget)
{
    m_result.append(QString("(P %0\n").arg(generateStepPName()));
    m_result.append(QString(
                        R"(
                        =goal>
            isa passo
            ordem %0
            ?visual>
             state free
             ==>
             =goal>
            ordem %1
            +visual>
            isa move-attention
            screen-pos %2
            )"
                    ).arg(m_currentStep).arg(m_currentStep+1).arg(widget->objectName()));
    m_result.append(")\n");
    m_currentStep++;
}

void ModelCompiler::emitHandToMouse()
{
    // Coloca a mão no mouse
    m_result.append(QString("(P %0\n").arg(generateStepPName()));
    m_result.append(QString(
                        R"(
                        =goal>
            isa passo
            ordem %0
            ?manual>
             state free
             ==>
             =goal>
            ordem %1
            +manual>
            isa hand-to-mouse
            )"
                    ).arg(m_currentStep).arg(m_currentStep+1));
    m_result.append(")\n");
    m_currentStep++;
}

void ModelCompiler::emitGehDevice()
{
}

void ModelCompiler::emitGehCommon()
{
}

void ModelCompiler::emitStartTask()
{
}

void ModelCompiler::emitVisLocsChunks()
{
}

void ModelCompiler::emitVisObjsChunks()
{
}

QString ModelCompiler::generateStepPName()
{
    return QString("P-%1").arg(m_currentStep);
}
