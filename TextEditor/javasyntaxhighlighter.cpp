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

    //Method definitions
    functionFormat.setForeground(QColor("#f35588"));

    //https://regex101.com/r/cHbJr0/1 (old)
    //https://regex101.com/r/Y3esca/2 (new)
//    rule.pattern = QRegularExpression("(((public|protected|private|static|abstract|final|synchronized|native)\\s+)*(void|byte|short|int|long|char|float|double|boolean|\\w+)(\[\\s*\])*\\s*\\w+\\s*(\\s*\(\\s*(((void|byte|short|int|long|char|float|double|boolean|\\w+)(\[\\s*\])*)\\s+\\w+\\s*)*\\s*\)\\s*))");
//    rule.pattern = QRegularExpression("((\\b(public|protected|private|static|abstract|final|synchronized|native)(\\s+(public|protected|private|static|abstract|final|synchronized|native))*)*)\\b((void|byte|short|int|long|char|float|double|boolean|\\w+)(\[\\s*\])*\\s*\\w+\\s*(\\s*\(\\s*(((void|byte|short|int|long|char|float|double|boolean|\\w+)(\[\\s*\])*)\\s+\\w+\\s*)*\\s*\)\\s*))*");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    //Class definition
    classFormat.setForeground(QColor(255,127,80));

    rule.pattern = QRegularExpression("\\s*(public|abstract|final)+\\s+\\bclass\\b\\s+\\w+\\s*(extends\\s+\\w+)?(\\s+)?(implements\\s+\\w+(,\\s*\\w+)*)?");
    rule.format = classFormat;
    highlightingRules.append(rule);

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
}
