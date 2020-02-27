#include "cppsyntaxhighlighter.h"

CPPSyntaxHighlighter::CPPSyntaxHighlighter(QTextDocument *parent) : CodeSyntaxHighlighter(parent)
{
    readKeywordFileAndPopulateList();
}

void CPPSyntaxHighlighter::readKeywordFileAndPopulateList()
{
    QFile file(":/cpp_keywords.txt");

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

    //new and delete get a special color (plan is to make it super obvious they're using it
    newAndDeleteFormat.setForeground(Qt::red);

    rule.pattern = QRegularExpression(QStringLiteral("\\bnew|delete\\b"));
    rule.format = newAndDeleteFormat;
    highlightingRules.append(rule);
}


