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
#include <iostream>
#include <ostream>
#include <cmath>
#include "designersurface.h"
#include "designercommands.h"
#include "widgetfactory.h"
#include "globalids.h"
#include "mainwindow.h"
#include "util.h"

using namespace std;

// o namespace dos comandos para a stack de reversão
namespace Cmd = DesignerSurfaceCommands;

const QString DesignerSurface::sm_dsSizeGripName(QObject::tr("designerSurfaceSizeGrip"));

DesignerSurface *DesignerSurface::sm_instance = nullptr;

// Macro útil para devolver o nome da classe de objeto do tipo Q_OBJECT
template <class T>
QString Q_NAME() {
    return QObject::tr(T::staticMetaObject.className());
}

// =============================================================================
// No construtor, ajustamos alguns valores para o funcionamento correto
// do nosso controle.
// =============================================================================
DesignerSurface::DesignerSurface(MainWindow* mainWindow,
                                 QWidget* parent)
    : QWidget(parent, Qt::SubWindow),
      m_application(mainWindow),
      m_gridSize(QPoint(8, 8)),
      m_widgetSizer(QSize(5, 5)),
      m_selectionAdjust(QRect(-3, -3, 3, 3))
{
    sm_instance = this;
    // nosso controle aceita Drag 'n Drop
    setAcceptDrops(true);

    // Importante para que possamos redimensionar o widget
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    // Instancia nosso QSizeGrip
    m_sizeGrip = new QSizeGrip(NULL);
    m_sizeGrip->setFixedSize(m_sizeGrip->sizeHint());
    m_sizeGrip->setObjectName(sm_dsSizeGripName);
    m_sizeGrip->setParent(this);

    // Define a cor de fundo do nosso frame
    setForegroundRole(QPalette::Window);

    // está padrão das variáveis
    m_showGrid = true;
    m_snapToGrid = true;
    m_editMode = true;
    m_lowerWidget = this;
    m_mouseState = Normal;
    m_currentWidget = NULL;

    // Precisa receber todos os eventos do mouse
    setMouseTracking(true);

    m_containerClasses << Q_NAME<DesignerSurface>() <<
                          Q_NAME<QGroupBox>();

    m_filterClasses << Q_NAME<QWidget>() <<
                       Q_NAME<QLabel>() <<
                       Q_NAME<QPushButton>() <<
                       Q_NAME<QLineEdit>() <<
                       Q_NAME<QCheckBox>() <<
                       Q_NAME<QRadioButton>() <<
                       Q_NAME<QGroupBox>() <<
                       Q_NAME<QComboBox>();
    setFocusPolicy(Qt::StrongFocus);
}

// =============================================================================
// Nossa dica sobre o tamanho mínimo do widget
// Nesse caso para que possamos ter o QSizeGrip no canto inferior esquerdo
// cálculamos no mínimo o tamanho dele 2x acrescido de 1
// =============================================================================
QSize DesignerSurface::sizeHint() const
{
    return QSize(m_sizeGrip->sizeHint().width() * 2 + 1,
                 m_sizeGrip->sizeHint().height() * 2 + 1);
}


// =============================================================================
// Mapeia as coordenadas de um widget dentro do nosso controle
// =============================================================================
QRect DesignerSurface::mapToDesigner(QWidget* widget)
{
    QPoint pos = widget->mapToGlobal(QPoint(0, 0));
    return QRect(this->mapFromGlobal(pos), widget->size());
}



// =============================================================================
// Verifica qual widget está abaixo em uma posição, excluindo a mascára.
// =============================================================================
QWidget* DesignerSurface::childAtMasked(QPoint pos, QWidget* mask)
{
    const QObjectList& widgets = children();
    foreach (QObject* object, widgets) {

        // verifica se o objeto é do tipo QWidget
        if (object->objectName() == sm_dsSizeGripName ||
                object == mask)
            continue;
        if (object->isWidgetType()) {
            QWidget* widget = qobject_cast<QWidget *>(object);
            if (widget->geometry().contains(pos))
                return widget;
        }
    }
    return NULL;
}


// =============================================================================
// Alterna o estado de uma CheckBox (marcado/desmarcado)
// =============================================================================
void DesignerSurface::toogleCheckBox(QCheckBox *widget)
{
    widget->toggle();
}


// =============================================================================
// Alinha a posição dos widgets selecionados a grade
// =============================================================================
void DesignerSurface::snapWidgetsToGrid()
{
    QWidget *widget;
    foreach (widget, m_widgetSelection) {
        widget->move(roundToGridSnap(widget->pos()));
    }
}

// =============================================================================
// Alinha as dimensões do widgets selecionados a grade
// =============================================================================
void DesignerSurface::snapWidgetsSizeToGrid()
{
    foreach (QWidget *widget, m_widgetSelection) {
        const QSize &size = widget->size();
        QPoint px = roundToGridSnap(QPoint(size.width(), size.height()));
        QSize nsize(px.x(), px.y());
        widget->resize(nsize);
    }
}

// =============================================================================
//
// =============================================================================
void DesignerSurface::leaveEvent(QEvent* event)
{
    Q_UNUSED(event);
    update();
}

// =============================================================================
// Processa quando uma tecla é pressionada.
// TODO: Atualizar o widget apenas nas áreas que foram afetadas.
// =============================================================================
void DesignerSurface::keyPressEvent(QKeyEvent* event)
{
    setFocus();
    switch (event->key()) {
    case Qt::Key_Down:
        moveSelection(QPoint(0, 1));
        break;
    case Qt::Key_Up:
        moveSelection(QPoint(0,-1));
        break;
    case Qt::Key_Left:
        moveSelection(QPoint(-1,0));
        break;
    case Qt::Key_Right:
        moveSelection(QPoint( 1,0));
        break;
    case Qt::Key_Delete:
        deleteSelectedWidgets();
        break;
    default:
        QWidget::keyPressEvent(event);
        return;
    }
    emit widgetsModified(m_widgetSelection);
    update();
}

// =============================================================================
// Processa quando uma tecla é liberada.
// TODO: Implementar o comando de deletar
// =============================================================================
void DesignerSurface::keyReleaseEvent(QKeyEvent* event)
{
    Q_UNUSED(event);
}

// =============================================================================
// Verifica se um objeto é um container
// =============================================================================
bool DesignerSurface::isContainerWidget(QObject* widget)
{
    if (!widget)
        return false;
    return m_containerClasses.contains(tr(widget->metaObject()->className()));
}

// =============================================================================
// Verifica se um objeto é filtrado pelo nosso componente
// =============================================================================
bool DesignerSurface::isFilteredWidget(QObject* widget)
{
    if (!widget)
        return false;
    return m_filterClasses.contains(tr(widget->metaObject()->className()));
}

// =============================================================================
// Evento para redimensionar
// Aqui além de redimensionar nosso widget, é necessário reposicionar o sizeGrip
// para que o mesmo apareça sempre no canto inferior direito.
// =============================================================================
void DesignerSurface::resizeEvent(QResizeEvent* event)
{
    QSize old = size();

    resize(event->size());
    event->accept();

    // é necessário reposicionar o sizeGrip no canto esquerdo inferior
    QSize it = size();
    QSize gs = m_sizeGrip->size();
    m_sizeGrip->move(it.width() - gs.width(),
                     it.height() - gs.height());

    Cmd::ResizeForm *resizeFormCmd = new Cmd::ResizeForm(this, old);
    m_application->undoStack()->push(resizeFormCmd);
}


// =============================================================================
// Como estamos utilizando os widgets padrões do Qt, é necessário filtrarmos
// os eventos que são enviados até eles, e alguns tem que ser manipulados pelo
// nosso controle.
// =============================================================================
bool DesignerSurface::eventFilter(QObject* watched,
                                  QEvent* event)
{

    switch (event->type())
    {
    // não aceitamos esses eventos
    // eles serão repassados a nós automaticamente (e corrigidos :)
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseMove:
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    case QEvent::FocusIn:
    case QEvent::FocusOut:
    case QEvent::InputMethod:
    case QEvent::InputMethodQuery:
    case QEvent::ContextMenu:
    case QEvent::MouseButtonDblClick:
        event->setAccepted(false);
        return true;

    case QEvent::ChildRemoved:
    case QEvent::ChildAdded:
    {
        QChildEvent* childEvent = static_cast<QChildEvent *>(event);

        // Verificamos em que tipo de objetos podemos instalar o filtro
        if (!isFilteredWidget(childEvent->child()))
            break;

        // Verifica se o objeto é um container
        if (!isContainerWidget(watched))
            break;

        if (event->type() == QEvent::ChildAdded) {
            childEvent->child()->installEventFilter(this);
        } else {
            childEvent->child()->removeEventFilter(this);
        }
        event->setAccepted(true);
        return true;
    }

    default:
        break;
    }

    return parent()->eventFilter(watched, event);
}

// =============================================================================
// Para nossa comodidade, toda vez que algum filho for inserido no controle,
// instalamos filtro de eventos
// =============================================================================
void DesignerSurface::childEvent(QChildEvent* event)
{
    EDIT_MODE_ONLY(event);
    QObject* child = event->child();

    // o unico widget que não filtramos é o QSizeGrip
    if (child->objectName() == sm_dsSizeGripName) {
        return;
    }

    if (child->isWidgetType()) {
        if (event->type() == QEvent::ChildAdded) {
            child->installEventFilter(this);
        } else if (event->type() == QEvent::ChildRemoved) {
            child->removeEventFilter(this);
        }
    }
    QWidget::childEvent(event);
}

// =============================================================================
// Instala/Desinstala os filtros nos widgets
// =============================================================================
void DesignerSurface::setFilterChildEvents(bool filter)
{
    filterChildRecursive(this, filter);
}

// =============================================================================
// Instala/Desinstala os filtros nos widgets recursivamente
// =============================================================================
void DesignerSurface::filterChildRecursive(QObject* object, bool filter)
{
    if (object->objectName() == sm_dsSizeGripName)
        return;

    // verifica se é do tipo widget
    if (object->isWidgetType() && isFilteredWidget(object) &&
            object != this) {
        // instala o filtro
        if (filter)
            object->installEventFilter(this);
        else
        // desinstala o filtro
            object->removeEventFilter(this);
    }

    // se for um container, vamos percorrer os filhos
    if (isFilteredWidget(object) || object == this) {
        const QObjectList& widgets = object->children();
        foreach (QObject* child, widgets) {
            filterChildRecursive(child, filter);
        }
    }
}





// =============================================================================
// Dado um widget filho, retorna o retângulo onde se encontra o redimensionar.
// =============================================================================
QRect DesignerSurface::getSizerRect(QWidget* child)
{
    const int w = m_widgetSizer.width();
    const int h = m_widgetSizer.height();

    QRect selRect = mapToDesigner(child).adjusted(m_selectionAdjust.x(),
                                                  m_selectionAdjust.y(),
                                                  m_selectionAdjust.width(),
                                                  m_selectionAdjust.height());

    return QRect(selRect.x() + selRect.width() - w,
                 selRect.y() + selRect.height() - h,
                 w, h);
}

// =============================================================================
// Habilita a visualização da grade
// =============================================================================
void DesignerSurface::setGrid(bool enabled)
{
    if (m_showGrid != enabled) {
        m_showGrid = enabled;
        update();
    }
}

// =============================================================================
// Habilita o alinhamento na grade
// =============================================================================
void DesignerSurface::setSnapToGrid(bool enabled)
{
    m_snapToGrid = enabled;
}

// =============================================================================
//  Arredonda uma coordenada para a grade
// =============================================================================
QPoint DesignerSurface::roundToGridSnap(QPoint pos)
{
    int px = (pos.x() / m_gridSize.x()) * m_gridSize.x();
    int py = (pos.y() / m_gridSize.y()) * m_gridSize.y();

    return QPoint(px, py);
}


// =============================================================================
// Habilita o modo de edição
// =============================================================================
void DesignerSurface::setEditMode(bool enabled)
{
    if (m_editMode == enabled)
        return;

    // Estávamos no modo de visualização
    if (!m_editMode) {
        setupEditMode();
    } else { // modo de edição
        setupViewMode();
    }
    update();
}


// =============================================================================
// Configura o modo de visão
// =============================================================================
void DesignerSurface::setupViewMode()
{
    m_editMode = false;
    m_sizeGrip->hide();
    setFilterChildEvents(false);
    m_widgetSelection.clear();
}

// =============================================================================
// Configura o modo de edição
// =============================================================================
void DesignerSurface::setupEditMode()
{
    m_editMode = true;
    m_sizeGrip->show();
    setFilterChildEvents(true);
}


// =============================================================================
// Retorna verdadeiro se estiver em modo de edição
// =============================================================================
bool DesignerSurface::isEditMode() const
{
    return m_editMode;
}

QList<QWidget*> DesignerSurface::formWidgets()
{
    QList<QWidget*> widgets;

    foreach (QObject *obj, children()) {
        if (!obj->isWidgetType())
            continue;
        QWidget *w = qobject_cast<QWidget*>(obj);
        if (isFilteredWidget(w))
            widgets.push_back(w);
    }

    return widgets;
}

// =============================================================================
//
// =============================================================================
void DesignerSurface::showTree()
{
    cout << "Exibindo os widgets" << endl;
    showTreeIterator(this);
}

// =============================================================================
//
// =============================================================================
void DesignerSurface::showTreeIterator(QObject* object)
{
    // somente os widgets filtrados ou o proprio designer.
    if (isFilteredWidget(object) || object == this) {
        cout << '<' << getClassName(object).toStdString() << '>' << endl;
        const QObjectList& widgets = object->children();
        foreach (QObject* child, widgets) {
            showTreeIterator(child);
        }
        cout << "</" << getClassName(object).toStdString() << '>' << endl;
    }
}

// =============================================================================
// Deleta os widgets selcionados
// =============================================================================
void DesignerSurface::deleteSelectedWidgets()
{
    if (m_widgetSelection.empty())
        return;

    emit widgetsDeleted(m_widgetSelection);
    Cmd::DeleteWidgets* deleteCmd = new Cmd::DeleteWidgets(this, m_widgetSelection);
    m_application->undoStack()->push(deleteCmd);
}

// =============================================================================
// Propriedades genéricas editáveis
// =============================================================================
const QStringList DesignerSurface::sm_genericProperties(
    { "objectName","enabled","pos","size","visible","toolTip","text", "title" });

// =============================================================================
// Retorna a lista das propriedades genéricas
// =============================================================================
const QStringList& DesignerSurface::getGenericProperties()
{
    return sm_genericProperties;
}


// =============================================================================
// Deleta um widget das listas
// =============================================================================
void DesignerSurface::remove(QWidget* widget)
{
    if (m_currentWidget == widget)
        m_currentWidget = NULL;
    if (m_lowerWidget == widget)
        m_lowerWidget = this;
    if (m_widgetSelection.contains(widget)) {
        m_widgetSelection.removeOne(widget);
    }
}

