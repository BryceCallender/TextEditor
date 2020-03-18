#ifndef JAVASYNTAXHIGHLIGHTER_H
#define JAVASYNTAXHIGHLIGHTER_H

#include "codesyntaxhighlighter.h"

/*! \class CPPSyntaxHighlighter
    \brief A syntax highlighter for the language of Java.

    This is a concrete implementation of the CodeSyntaxHighlighter class for the language of Java.
 */
class JavaSyntaxHighlighter : public CodeSyntaxHighlighter
{
public:
    //! Constructor
    /*!
      This constructor will call the \a readKeywordFileAndPopulateList function.
     */
    explicit JavaSyntaxHighlighter(QTextDocument* parent);

    //! Concrete definition of parent pure virtual method.
    /*!
      This overrides the virtual function from the CodeSyntaxHighlighter class. It will read the Java keywords and own up to finding those in the text edit.
     */
    void readKeywordFileAndPopulateList() override;
};

#endif // JAVASYNTAXHIGHLIGHTER_H
