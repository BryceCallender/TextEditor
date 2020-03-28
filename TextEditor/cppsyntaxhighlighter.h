#ifndef CPPSYNTAXHIGHLIGHTER_H
#define CPPSYNTAXHIGHLIGHTER_H

#include "codesyntaxhighlighter.h"

/*! \class CPPSyntaxHighlighter
    \brief A syntax highlighter for the language of C++.

    This is a concrete implementation of the CodeSyntaxHighlighter class for the language of C++.
 */
class CPPSyntaxHighlighter : public CodeSyntaxHighlighter
{
public:
    //! Constructor
    /*!
      This constructor will call the \a readKeywordFileAndPopulateList function.
     */
    explicit CPPSyntaxHighlighter(QTextDocument* parent);

    //! Concrete definition of parent pure virtual method.
    /*!
      This overrides the virtual function from the CodeSyntaxHighlighter class. It will read the C++ keywords and own up to finding those in the text edit.
     */
    void readKeywordFileAndPopulateList() override;

    //! DataStream extractor for mimedata retrieval of the class CodeSyntaxHighlighter
    /*!
      \param in data stream that will be reading in the data.
      \param highlighterData the syntax highlighter that will be populated with the stream data.
     */
    friend QDataStream& operator>>(QDataStream &in, CPPSyntaxHighlighter& highlighterData)
    {
        int numberOfRules;
        QVector<HighlightingRule> highlightingRules;

        QRegularExpression commentStartExpression;
        QRegularExpression commentEndExpression;

        QTextCharFormat keywordFormat;
        QTextCharFormat classFormat;
        QTextCharFormat singleLineCommentFormat;
        QTextCharFormat multiLineCommentFormat;
        QTextCharFormat quotationFormat;
        QTextCharFormat functionFormat;
        QTextCharFormat newAndDeleteFormat;

        in >> numberOfRules;

        //Null highlighter since there should be rules for highlighting syntax!
        if(numberOfRules == 0)
        {
            return in;
        }

        HighlightingRule rule;
        for(int i = 0; i < numberOfRules; i++)
        {
            in >> rule;
            highlightingRules.push_back(rule);
        }

        //Read in all the QTextCharFormats (auto serialized from Qt impl)
        in >> commentStartExpression >> commentEndExpression >> keywordFormat >> classFormat
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
        highlighterData.newAndDeleteFormat = newAndDeleteFormat;

        return in;
    }

    //! DataStream insert for mimedata storage of the class CodeSyntaxHighlighter.
    /*!
      \param in data stream that will be reading in the data.
      \param highlighterData the syntax highlighter data that will be written to the stream in bytes.
     */
    friend QDataStream& operator<<(QDataStream &out, CPPSyntaxHighlighter& highlighterData)
    {
        out << highlighterData.highlightingRules.size();

        for(HighlightingRule rule: highlighterData.highlightingRules)
        {
            out << rule;
        }

        out << highlighterData.commentStartExpression
            << highlighterData.commentEndExpression << highlighterData.keywordFormat
            << highlighterData.classFormat << highlighterData.singleLineCommentFormat
            << highlighterData.multiLineCommentFormat <<  highlighterData.quotationFormat
            << highlighterData.functionFormat << highlighterData.newAndDeleteFormat;

        return out;
    }

private:
    QTextCharFormat newAndDeleteFormat; /*!< The formatting for when the keyword 'new' or 'delete' is found. */
};

#endif // CPPSYNTAXHIGHLIGHTER_H
