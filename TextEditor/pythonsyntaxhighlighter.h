#ifndef PYTHONSYNTAXHIGHLIGHTER_H
#define PYTHONSYNTAXHIGHLIGHTER_H

#include "codesyntaxhighlighter.h"

/*! \class PythonSyntaxHighlighter
    \brief A syntax highlighter for the language of Python.

    This is a concrete implementation of the CodeSyntaxHighlighter class for the language of Python.
 */
class PythonSyntaxHighlighter : public CodeSyntaxHighlighter
{
public:
    //! Constructor
    /*!
      This constructor will call the \a readKeywordFileAndPopulateList function.
     */
    explicit PythonSyntaxHighlighter(QTextDocument* parent);

    //! Concrete definition of parent pure virtual method.
    /*!
      This overrides the virtual function from the CodeSyntaxHighlighter class. It will read the Python keywords and own up to finding those in the text edit.
     */
    void readKeywordFileAndPopulateList() override;
};

#endif // PYTHONSYNTAXHIGHLIGHTER_H
