#include "replacercommand.h"

ReplacerCommand::ReplacerCommand(int numberOfReplacements, QUndoStack *undoStack)
{
    commandIterations = numberOfReplacements;
    this->undoStack = undoStack;
}

void ReplacerCommand::undo()
{
    for(int i = 0; i < commandIterations; i++)
    {
        undoStack->undo();
    }
}

void ReplacerCommand::redo()
{
    for(int i = 0; i < commandIterations; i++)
    {
        undoStack->redo();
    }
}
