#ifndef CODESYNTAXHIGHLIGHER_H
#define CODESYNTAXHIGHLIGHER_H

#include <QSyntaxHighlighter>
#include <QTextDocument>

class CodeSyntaxHighligher : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit CodeSyntaxHighligher(QTextDocument* parent);
};

#endif // CODESYNTAXHIGHLIGHER_H
