#include "javasyntaxhighlighter.h"

JavaSyntaxHighlighter::JavaSyntaxHighlighter(QTextDocument* parent) : CodeSyntaxHighlighter(parent)
{
    readKeywordFileAndPopulateList();
}

void JavaSyntaxHighlighter::readKeywordFileAndPopulateList()
{
    QFile file(":/java_keywords.txt");

    QVector<QString> keywordPatterns;

    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);

    if(file.open(QFile::ReadOnly))
    {
        QTextStream in(&file);
        while(!in.atEnd())
        {
            QString value = "\\b" + in.readLine() + "\\b"; // the b statements are boundary so it wont match substrings
            keywordPatterns.push_back(value.toLatin1());
        }
    }

    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    //""
    quotationFormat.setForeground(Qt::darkGreen);

    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    ////
    singleLineCommentFormat.setForeground(Qt::blue);

    rule.pattern = QRegularExpression("//.*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    //class definition
    classFormat.setForeground(QColor(255,127,80));

    rule.pattern = QRegularExpression("(public|abstract|final)?");
    rule.format = classFormat;
    highlightingRules.append(rule);

}
