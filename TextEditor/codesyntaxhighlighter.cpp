#include "codesyntaxhighlighter.h"


CodeSyntaxHighlighter::CodeSyntaxHighlighter(QTextDocument *parent): QSyntaxHighlighter(parent)
{
}

void CodeSyntaxHighlighter::updateKeywordColor(const QColor &color)
{
    for(int i = 0; i < highlightingRules.size(); i++)
    {
        HighlightingRule rule = highlightingRules.at(i);

        if(rule.format == keywordFormat)
        {
            rule.format.setForeground(color);
            rule.format.setFontWeight(QFont::Bold);

            qDebug() << "Set" << rule.pattern.pattern() << "to color" << color;
        }

        highlightingRules[i] = rule;
    }

    keywordFormat.setForeground(color);

    rehighlight();
    qDebug() << "New Color: " << color;
}

int CodeSyntaxHighlighter::getRuleCount()
{
    return highlightingRules.size();
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
