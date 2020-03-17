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

    friend QDataStream& operator>>(QDataStream &in, CodeSyntaxHighlighter& highlighterData)
    {
        QVector<HighlightingRule> highlightingRules;

        QRegularExpression commentStartExpression;
        QRegularExpression commentEndExpression;

        QTextCharFormat keywordFormat;
        QTextCharFormat classFormat;
        QTextCharFormat singleLineCommentFormat;
        QTextCharFormat multiLineCommentFormat;
        QTextCharFormat quotationFormat;
        QTextCharFormat functionFormat;

        in >> highlightingRules >> commentStartExpression >> commentEndExpression >> keywordFormat >> classFormat
           >> singleLineCommentFormat >> multiLineCommentFormat >>  quotationFormat >> functionFormat;

        highlighterData.highlightingRules = highlightingRules;
        highlighterData.commentStartExpression = commentStartExpression;
        highlighterData.commentEndExpression = commentEndExpression;
        highlighterData.keywordFormat = keywordFormat;
        highlighterData.classFormat = classFormat;
        highlighterData.singleLineCommentFormat = singleLineCommentFormat;
        highlighterData.multiLineCommentFormat = multiLineCommentFormat;
        highlighterData.quotationFormat = quotationFormat;
        highlighterData.functionFormat = functionFormat;

        return in;
    }

protected:
    virtual void highlightBlock(const QString &text) override;

    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    friend QDataStream& operator>>(QDataStream &in, HighlightingRule& rule) {
        QRegularExpression pattern;
        QTextCharFormat format;

        in >> pattern >> format;

        rule.pattern = pattern;
        rule.format = format;

        return in;
    }

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
