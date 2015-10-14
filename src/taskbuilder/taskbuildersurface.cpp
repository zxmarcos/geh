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
#include <QVariant>
#include <iterator>
#include "taskbuildersurface.h"
#include "designersurface/designersurface.h"
#include "gerador/sequencecompiler.h"
#include "util.h"

TaskBuilderSurface::TaskBuilderSurface(QWidget *parent) :
    QWidget(parent),
    m_backgroundWidget(nullptr)
{
    m_abrev[SequenceNode::INVALID] = 'I';
    m_abrev[SequenceNode::SELECT] = 'S';
    m_abrev[SequenceNode::CHOOSE] = 'E';
    m_abrev[SequenceNode::CHECK] = 'M';
    m_abrev[SequenceNode::UNCHECK] = 'D';
    m_abrev[SequenceNode::PRESS] = 'P';
    m_abrev[SequenceNode::FILL] = 'T';
}

// =============================================================================
// Coloca um widget como o fundo do criador de tarefas
// =============================================================================
void TaskBuilderSurface::setBackgroundWidget(QWidget *widget)
{
    m_backgroundWidget = widget;
    m_background = m_backgroundWidget->grab();
    setMaximumSize(widget->size());
    setMinimumSize(widget->size());
    m_order.clear();
    m_currentOrder = 0;
}


// =============================================================================
// Cria a descrição da tarefa em LDT utilizando as informações do editor
// =============================================================================
void TaskBuilderSurface::build()
{
    QStringList listing;

    foreach (const WidgetOrder& order, m_order) {
        const QString objName = order.widget->objectName();

        if (isPushButton(order.widget))
            listing << QString("pressionar %1;").arg(objName);

        else if (isLineEdit(order.widget))
            listing << QString("preencher %1 \" \";").arg(objName);

        else if (isRadioButton(order.widget))
            listing << QString("selecionar %1;").arg(objName);

        else if (isComboBox(order.widget))
            listing << QString("escolher \"%1\" %2;").arg(order.param.toString()).arg(objName);

        else if (isCheckBox(order.widget)) {
            if (order.action == SequenceNode::CHECK)
                listing << QString("marcar %1;").arg(objName);
            else if (order.action == SequenceNode::UNCHECK)
                listing << QString("desmarcar %1;").arg(objName);
        }
    }
    emit doneCode(listing.join("\n"));
}

// =============================================================================
//
// =============================================================================
void TaskBuilderSurface::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
    if (event->button() != Qt::LeftButton)
        return;

    QWidget *widget = m_backgroundWidget->childAt(event->pos());

    if (isMasked(widget))
        return;

    if (widget->objectName().isEmpty()) {
        QMessageBox::warning(this, "Erro", "O objeto selecionado não possui um nome!\n"
                             "Dê um nome ao objeto no editor de propriedades.",
                             QMessageBox::Ok);
        return;
    }

    // Um pouco de C++11 :)
    for (auto it = std::begin(m_order); it != std::end(m_order); it++) {
        WidgetOrder& order = *it;
        if (order.widget == widget) {
            if (isMultipleChoice(order.widget)) {
                if (!handleMultipleChoice(order, event->globalPos()))
                    return;
                update();
            }
            return;
        }
    }

    insertOrder(widget, event->globalPos());
    update();
}


// =============================================================================
//
// =============================================================================
void TaskBuilderSurface::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
}

// =============================================================================
//
// =============================================================================
void TaskBuilderSurface::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();
}


// =============================================================================
// Desenha nossas caixas no fundo
// =============================================================================
void TaskBuilderSurface::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setClipRegion(event->region());
    p.drawPixmap(rect(), m_background);

    const QColor alphaColor(0, 0, 255, 20);
    p.setPen(Qt::blue);

    QFont font;
    font.setBold(true);
    p.setFont(font);


    foreach (const WidgetOrder& order, m_order) {
        p.setPen(Qt::blue);
        p.setBrush(alphaColor);
        p.drawRect(order.rect.adjusted(-2, -2, 2, 2));

        p.setBrush(Qt::white);
        p.drawRect(order.orderRect);
        p.drawText(order.orderRect, Qt::AlignHCenter| Qt::AlignVCenter,
                   QString::number(order.order + 1));

        p.setPen(Qt::red);
        p.drawRect(order.actionRect);
        p.drawText(order.actionRect, Qt::AlignHCenter| Qt::AlignVCenter,
                   m_abrev[order.action]);

    }
    event->accept();
}


// =============================================================================
// Retornamos o tamanho do widget de fundo caso ele esteja setado
// =============================================================================
QSize TaskBuilderSurface::sizeHint() const
{
    if (m_backgroundWidget != nullptr)
        return m_backgroundWidget->size();
    return QSize(100, 100);
}


// =============================================================================
// Nosso menu de contexto
// =============================================================================
void TaskBuilderSurface::contextMenuEvent(QContextMenuEvent *event)
{
    QWidget *widget = m_backgroundWidget->childAt(event->pos());

    if (isMasked(widget))
        return;

    QMenu menu(this);
    QAction *restart = menu.addAction(tr("Recomeçar aqui"));

    QAction *result = menu.exec(event->globalPos());
    // Para todos os widgets
    if (result == restart) {
        m_currentOrder = 0;
        m_order.clear();

        insertOrder(widget, event->globalPos());
        update();
    }
}


// =============================================================================
// Verifica se um widget é mascarado, isto é, não recebe eventos da LDT
// =============================================================================
bool TaskBuilderSurface::isMasked(QWidget *widget)
{
    if (widget == nullptr)
        return true;

    return (isGroupBox(widget) || isLabel(widget));
}

// =============================================================================
// Verifica se um widget tem múltiplas escolhas de ação
// =============================================================================
bool TaskBuilderSurface::isMultipleChoice(QWidget *widget)
{
    if (widget == nullptr)
        return false;

    return (isComboBox(widget) || isCheckBox(widget));
}

// =============================================================================
// Retorna a ação padrão para o componente
// =============================================================================
int TaskBuilderSurface::getDefaultAction(QWidget *widget)
{
    if (isPushButton(widget))
        return SequenceNode::PRESS;

    if (isLineEdit(widget))
        return SequenceNode::FILL;

    if (isRadioButton(widget))
        return SequenceNode::SELECT;

    if (isComboBox(widget))
        return SequenceNode::CHOOSE;

    if (isCheckBox(widget))
        return SequenceNode::CHECK;

    return SequenceNode::INVALID;
}


// =============================================================================
// Pergunta qual item da caixa de seleção será escolhido
// =============================================================================
PairResult TaskBuilderSurface::getComboBoxItem(QComboBox *widget, QPoint pos)
{
    QAbstractItemModel *model = widget->model();
    QMap<QAction*, QModelIndex> map;

    PairResult ret(false, QVariant());

    if (model->rowCount() < 1) {
        QMessageBox::warning(this, "Erro",
                             "Não existe nenhum item dentro da caixa de seleção!",
                             QMessageBox::Ok);
        return ret;
    }

    // Cria um menu na pilha
    QMenu menu(this);

    // Popula o menu com os items do combobox
    for (int i = 0; i < model->rowCount(); i++) {
        QModelIndex idx = model->index(i, 0);
        QAction *action = menu.addAction(model->data(idx).toString());
        map[action] = idx;
    }

    // executa o menu
    QAction *result = menu.exec(pos);

    auto it = map.find(result);
    if (it != std::end(map)) {
        QModelIndex idx = *it;
        ret = PairResult(true, model->data(idx));
    }

    return ret;
}


// =============================================================================
// Pergunta a opção para um checkbox
// =============================================================================
PairResult TaskBuilderSurface::getCheckBoxAction(QPoint pos)
{
    QMenu menu(this);

    QAction *check = menu.addAction("Marcar");
    QAction *uncheck = menu.addAction("Desmarcar");

    // executa o menu
    QAction *result = menu.exec(pos);

    PairResult ret(false, QVariant());

    if (result == uncheck) {
        ret = PairResult(true, SequenceNode::UNCHECK);
    } else if (result == check) {
        ret = PairResult(true, SequenceNode::CHECK);
    }
    return ret;
}

// =============================================================================
// Verifica se o objeto é do tipo multipla escolha e pergunta sobre elas.
// =============================================================================
bool TaskBuilderSurface::handleMultipleChoice(TaskBuilderSurface::WidgetOrder &order,
                                               const QPoint &pos)
{
    PairResult result;

    if (isComboBox(order.widget)) {
        result = getComboBoxItem(static_cast<QComboBox*>(order.widget), pos);
        if (result.first) {
            order.param = result.second;
        }
    } else if (isCheckBox(order.widget)) {
        result = getCheckBoxAction(pos);
        if (result.first) {
            order.action = result.second.toInt();
        }
    }

    return result.first;
}


// =============================================================================
// Adiciona o widget na lista de ordenação
// =============================================================================
void TaskBuilderSurface::insertOrder(QWidget *widget, QPoint pos)
{
    const int size = 15;

    WidgetOrder order;
    order.action = getDefaultAction(widget);
    order.order = m_currentOrder;
    order.widget = widget;
    order.rect = QRect(order.widget->mapTo(m_backgroundWidget, QPoint(0, 0)),
                       order.widget->size());

    if (isMultipleChoice(widget))
        if (!handleMultipleChoice(order, pos))
            return;

    // retângulo da ordem
    {
        QPoint p = order.rect.topRight() - QPoint(10, 5);
        order.orderRect = QRect(p, p + QPoint(size, size));
    }

    // retângulo da ação
    {
        QPoint p = order.rect.bottomRight() - QPoint(10, 5);
        order.actionRect = QRect(p, p + QPoint(size, size));
    }

    m_order.push_back(order);

    ++m_currentOrder;
}
