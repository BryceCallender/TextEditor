#ifndef CODESYNTAXHIGHLIGHER_H
#define CODESYNTAXHIGHLIGHER_H

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QRegularExpression>
#include <QStringLiteral>
#include <QDebug>

#include "settingsmanager.h"

class CodeSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit CodeSyntaxHighlighter(QTextDocument* parent);

    virtual void readKeywordFileAndPopulateList() = 0;

    void updateKeywordColor(const QColor& color);

protected:
    virtual void highlightBlock(const QString &text) override;

    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};

#endif // CODESYNTAXHIGHLIGHER_H
