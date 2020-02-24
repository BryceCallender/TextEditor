#include "searchhighlighter.h"

SearchHighlighter::SearchHighlighter(QTextDocument* parent) : QSyntaxHighlighter(parent)
{
    format.setBackground(Qt::green);
}

void SearchHighlighter::highlightBlock(const QString& text)
{
    QRegularExpressionMatchIterator i = pattern.globalMatch(text);
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        setFormat(match.capturedStart(), match.capturedLength(), format);
    }
}

void SearchHighlighter::searchText(const QString &text)
{
    pattern = QRegularExpression(text);
    rehighlight();
}
