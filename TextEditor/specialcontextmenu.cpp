#include "specialcontextmenu.h"

SpecialContextMenu::SpecialContextMenu(QWidget *parent) : QWidget(parent)
{

}

void SpecialContextMenu::showContextPasteMenu(const QPoint& pos)
{
    //QPoint globalPos = getCurrentTabWidget()->getTextEdit()->mapToGlobal(pos);

    QMenu *pasteMenu = new QMenu(this);
    pasteMenu->addAction(QString(savedCopy[0]), this, SLOT(on_actionPaste_2_triggered()));

    if(savedCopy[1] != "")
        pasteMenu->addAction(QString(savedCopy[1]), this, SLOT(on_actionPaste_3_triggered()));
    if(savedCopy[2] != "")
        pasteMenu->addAction(QString(savedCopy[2]), this, SLOT(on_actionPaste_4_triggered()));

    //pasteMenu->exec(globalPos);

}

void SpecialContextMenu::showContextMenu(const QPoint& pos)
{
    //QPoint globalPos = getCurrentTabWidget()->getTextEdit()->mapToGlobal(pos);

    QMenu *pasteMenu = new QMenu(), *rightClick = new QMenu(this);

    rightClick->addAction(QString("Undo"), this, SLOT(on_actionUndo_triggered()));
    rightClick->addAction(QString("Redo"), this, SLOT(on_actionRedo_triggered()));
    rightClick->addSeparator();
    rightClick->addAction(QString("Cut"), this, SLOT(on_actionCut_triggered()));
    rightClick->addAction(QString("Copy"), this, SLOT(on_actionCopy_triggered()));

    pasteMenu->setTitle(QString("Paste"));
    pasteMenu->addAction(QString(savedCopy[0]), this, SLOT(on_actionPaste_2_triggered()));

    if(savedCopy[1] != "")
        pasteMenu->addAction(QString(savedCopy[1]), this, SLOT(on_actionPaste_3_triggered()));
    if(savedCopy[2] != "")
        pasteMenu->addAction(QString(savedCopy[2]), this, SLOT(on_actionPaste_4_triggered()));

    rightClick->addMenu(pasteMenu);

    //rightClick->exec(globalPos);

}
