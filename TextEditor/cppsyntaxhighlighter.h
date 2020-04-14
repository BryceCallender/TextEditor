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
private:
    QTextCharFormat newAndDeleteFormat; /*!< The formatting for when the keyword 'new' or 'delete' is found. */
};

#endif // CPPSYNTAXHIGHLIGHTER_H
