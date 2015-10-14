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
#ifndef DESIGNERSURFACE_H
#define DESIGNERSURFACE_H


#include <QWidget>
#include <QList>
#include <QStringList>
#include <QMap>
#include <QShortcut>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <QDomElement>

class MainWindow;
class QSize;
class QPaintEvent;
class QPainter;
class QRect;
class QCheckBox;
class QSizeGrip;

#define EDIT_MODE_ONLY(event)  if (!m_editMode) { \
    event->ignore();    \
    return; \
    }

class DesignerSurface : public QWidget
{
	Q_OBJECT
public:
    explicit DesignerSurface(MainWindow* mainWindow, QWidget* parent = 0);
    QSize sizeHint() const;

    static const QStringList& getGenericProperties();
    void remove(QWidget* widget);

    // designerfile.cpp
    bool loadDocument(const QString filename);
    bool saveDocument(const QString filename);

    bool isEditMode() const;

    QList<QWidget*> formWidgets();

public slots:

    void setGrid(bool enabled);
    void setSnapToGrid(bool enabled);
    void setEditMode(bool enabled);
    void showTree();
    void deleteSelectedWidgets();
    void closeDocument();

signals:
    void selectedOneWidget(QWidget* widget);
    void selectedMultipleWidgets(QString message);
    void selectedNoneWidget(QString message);
    void widgetsDeleted(const QList<QWidget*>& list);
    void widgetsModified(const QList<QWidget*>& list);

protected:
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void paintEvent(QPaintEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void leaveEvent(QEvent* event);
    virtual void dropEvent(QDropEvent* event);
    virtual void dragEnterEvent(QDragEnterEvent* event);
    virtual void dragMoveEvent(QDragMoveEvent* event);
    virtual bool eventFilter(QObject* watched, QEvent* event);
    virtual void childEvent(QChildEvent* event);
    virtual void resizeEvent(QResizeEvent* event);

private:
    void loadWidgetsRecursive(QObject* object);
    void saveWidgetsToXMLIterator(QObject* current, QTextStream& s);
    void findSelection();
    bool isWidgetSelected(QWidget* widget);
    bool isMouseOverSizer(const QPoint& mouse);
    void drawGrid(QPainter& painter);
    void drawSelectionBoxes(QPainter& painter);
    void drawSelectionRect(QPainter& painter);
    void moveSelection(const QPoint& delta);
    void dragFromSelection();
    void resizeSelection(const QPoint &delta);
    void drawParentBox(QWidget* widget, QPainter& painter);
    QPoint roundToGridSnap(QPoint pos);
    void snapWidgetsToGrid();
    void snapWidgetsSizeToGrid();
    void setupViewMode();
    void setupEditMode();
    void setFilterChildEvents(bool filter);
    void filterChildRecursive(QObject* object, bool filter);
    QRect getSizerRect(QWidget* child);
    QRect mapToDesigner(QWidget* widget);
    bool isContainerWidget(QObject* widget);
    bool isFilteredWidget(QObject* widget);
    void showTreeIterator(QObject* widget);
    void dropAndReparent(QDropEvent *event);
    // comandos
    void startSelectionResize();
    void endSelectionResize();
    void startSelectionMove();
    void endSelectionMove();
    void startReparentDrag();
    QWidget* childAtMasked(QPoint pos, QWidget* mask);
    MainWindow* m_application;
    bool m_editMode;
    bool m_showGrid;
    bool m_snapToGrid;
    const QPoint m_gridSize;
    QPointF m_startPos;
    QPointF m_endPos;
    QSizeGrip* m_sizeGrip;
    QSize m_widgetSizer;
    int m_mouseState;
    enum {
        Normal,
        Selecting,
        Moving,
        Scaling,
        Reparenting,
    };
    const QRect m_selectionAdjust;
    QWidget* m_currentWidget;
    QWidget* m_lowerWidget;
    QList<QWidget*> m_widgetSelection;
    QStringList m_containerClasses;
    QStringList m_filterClasses;
    QMap<QWidget*, QSize> m_oldSize;
    QMap<QWidget*, QPoint> m_oldPos;
    QShortcut* m_deleteShortcut;
    static const QStringList sm_genericProperties;
    static const QString sm_dsSizeGripName;

    void toogleCheckBox(QCheckBox* widget);
    void saveWidgets(QWidget* object, QXmlStreamWriter& writer);
    void loadWidgets(QWidget* parent, QDomElement& element);

    static DesignerSurface *sm_instance;
public:
    static DesignerSurface *get() {
        return sm_instance;
    }
};

#endif
