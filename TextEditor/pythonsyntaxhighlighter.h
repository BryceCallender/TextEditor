#ifndef PYTHONSYNTAXHIGHLIGHTER_H
#define PYTHONSYNTAXHIGHLIGHTER_H

#include "codesyntaxhighlighter.h"

class PythonSyntaxHighlighter : public CodeSyntaxHighlighter
{
public:
    explicit PythonSyntaxHighlighter(QTextDocument* parent);

    void readKeywordFileAndPopulateList() override;
};

#endif // PYTHONSYNTAXHIGHLIGHTER_H
