#include "codesyntaxhighlighter.h"


CodeSyntaxHighlighter::CodeSyntaxHighlighter(QTextDocument *parent): QSyntaxHighlighter(parent)
{
}

void CodeSyntaxHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules))
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
