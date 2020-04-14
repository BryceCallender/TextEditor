#ifndef CODESYNTAXHIGHLIGHER_H
#define CODESYNTAXHIGHLIGHER_H

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QRegularExpression>
#include <QStringLiteral>
#include <QDebug>

#include "settingsmanager.h"

/*! \class CodeSyntaxHighlighter
    \brief A syntax highlighter for class specific keywords.

    An abstract class responsible for highlighting language specific keywords. It'll highlight based on colors set in the settings window.
 */
class CodeSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    //! Constructor
    /*!
      The code syntax highlighter constructor is empty as this is an abstract class.
     */
    explicit CodeSyntaxHighlighter(QTextDocument* parent);

    //! Pure Virtual member.
    /*!
      This pure virtual function is how each syntax highlighter will read and populate the keywords it has jurisdiction over.
     */
    virtual void readKeywordFileAndPopulateList() = 0;

    //! Update the keyword color
    /*!
      \param color The new color to highlight with.
     */
    void updateKeywordColor(const QColor& color);

    //! Gets the amount of rules for this highlighter
    /*!
      Responsible for telling if the highlighter is essentially empty or not
     */
    int getRuleCount();
protected:
    //! Virtual method override from parent class, QSyntaxHighlighter
    /*!
      \param text the document text that will be highlighted
    */
    virtual void highlightBlock(const QString &text) override;

    /*! \struct HighlightingRule
        \brief A container to hold the pattern and the word formatting

        This struct holds the regular expression in order to find such keyword and the text format that should be applied when it finds the text that matches said pattern.
    */
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules; /*!< List of the highlighting rules for the language. */

    QRegularExpression commentStartExpression; /*!< The regular expression to find the start of a comment expression. */
    QRegularExpression commentEndExpression; /*!< The regular expression to find the end of a comment expression. */

    QTextCharFormat keywordFormat; /*!< The formatting for when a keyword is found. */
    QTextCharFormat classFormat; /*!< The formatting for when a class is found. */
    QTextCharFormat singleLineCommentFormat; /*!< The formatting for when a single line comment is found. */
    QTextCharFormat multiLineCommentFormat; /*!< The formatting for when a multi line comment is found. */
    QTextCharFormat quotationFormat; /*!< The formatting for when a string or char literal is found. */
    QTextCharFormat functionFormat; /*!< The formatting for when a function declaration is found. */
};

#endif // CODESYNTAXHIGHLIGHER_H
