#ifndef ERROREDITOR_H
#define ERROREDITOR_H

#include <QDialog>
#include "gerador/sequencecompiler.h"

namespace Ui {
class ErrorEditor;
}

class ErrorEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ErrorEditor(QWidget *parent = 0);
    ~ErrorEditor();

public slots:
    virtual int exec(TaskSequence *seq);
private:
    Ui::ErrorEditor *ui;
};

#endif // ERROREDITOR_H
