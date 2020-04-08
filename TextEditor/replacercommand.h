#ifndef REPLACERCOMMAND_H
#define REPLACERCOMMAND_H

#include <QUndoCommand>


class ReplacerCommand: public QUndoCommand
{
public:
    ReplacerCommand(int numberOfReplacements, QUndoStack *undoStack);

    void undo() override;
    void redo() override;
private:
    QUndoStack *undoStack;
    int commandIterations;
};

#endif // REPLACERCOMMAND_H
