#include "codesyntaxhighlighter.h"


CodeSyntaxHighlighter::CodeSyntaxHighlighter(QTextDocument *parent): QSyntaxHighlighter(parent)
{
}

void CodeSyntaxHighlighter::updateKeywordColor(const QColor &color)
{
    keywordFormat.setForeground(color);
    rehighlight();
    qDebug() << "New Color: " << color;
}

void CodeSyntaxHighlighter::highlightBlock(const QString &text)
{
    qDebug() << "Highlighting...";
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
