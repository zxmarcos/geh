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
#ifndef TASKBUILDERSURFACE_H
#define TASKBUILDERSURFACE_H

#include <QWidget>
#include <QVector>
#include <QMap>
#include <QVariant>
#include <QPair>

class QComboBox;

typedef QPair<bool, QVariant> PairResult;

class TaskBuilderSurface : public QWidget
{
    Q_OBJECT
public:
    explicit TaskBuilderSurface(QWidget *parent = 0);

    void setBackgroundWidget(QWidget *widget);

public slots:
    void build();
signals:
    void doneCode(QString);
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual QSize sizeHint() const;
    virtual void contextMenuEvent(QContextMenuEvent *event);
private:

    struct WidgetOrder {
        QWidget *widget;
        QRect rect;
        QRect orderRect;
        QRect actionRect;
        QVariant param;
        int order;
        int action;
        bool operator==(const WidgetOrder& rhs) {
            return widget == rhs.widget;
        }
    };

    bool isMasked(QWidget *widget);

    bool isMultipleChoice(QWidget *widget);
    bool handleMultipleChoice(WidgetOrder& order, const QPoint& pos);
    int getDefaultAction(QWidget *widget);
    void insertOrder(QWidget *widget, QPoint pos = QPoint());

    PairResult getComboBoxItem(QComboBox *widget, QPoint pos);
    PairResult getCheckBoxAction(QPoint pos);

    QWidget *m_backgroundWidget;
    QWidgetList *m_widgets;
    QVector<WidgetOrder> m_order;
    QPixmap m_background;
    int m_currentOrder;

    QMap<int, QChar> m_abrev;
};

#endif // TASKBUILDERSURFACE_H
