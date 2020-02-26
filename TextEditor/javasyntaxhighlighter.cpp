#include "javasyntaxhighlighter.h"

JavaSyntaxHighlighter::JavaSyntaxHighlighter(QTextDocument* parent) : CodeSyntaxHighlighter(parent)
{
    readKeywordFileAndPopulateList();
}

void JavaSyntaxHighlighter::readKeywordFileAndPopulateList()
{
    QFile javaKeywords(":/java_keywords.txt");

    if(javaKeywords.open(QFile::ReadOnly))
    {
        while(!javaKeywords.atEnd())
        {
            qDebug() << javaKeywords.readLine();
        }
    }
}
