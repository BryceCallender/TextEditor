#ifndef JAVASYNTAXHIGHLIGHTER_H
#define JAVASYNTAXHIGHLIGHTER_H

#include "codesyntaxhighlighter.h"

class JavaSyntaxHighlighter : public CodeSyntaxHighlighter
{
public:
    explicit JavaSyntaxHighlighter(QTextDocument* parent);

    void readKeywordFileAndPopulateList() override;
};

#endif // JAVASYNTAXHIGHLIGHTER_H
