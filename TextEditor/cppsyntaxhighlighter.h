#ifndef CPPSYNTAXHIGHLIGHTER_H
#define CPPSYNTAXHIGHLIGHTER_H

#include "codesyntaxhighlighter.h"

class CPPSyntaxHighlighter : public CodeSyntaxHighlighter
{
public:
    explicit CPPSyntaxHighlighter(QTextDocument* parent);

    void readKeywordFileAndPopulateList() override;
private:
    QTextCharFormat newAndDeleteFormat;
};

#endif // CPPSYNTAXHIGHLIGHTER_H
