#ifndef SEARCHHIGHLIGHTER_H
#define SEARCHHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QDebug>
#include <QTextEdit>

/*! \class SearchHighlighter
    \brief Highlight searches.

    This class is responsible for searching the document and highlighting words that a user searched.
 */
class SearchHighlighter: public QSyntaxHighlighter
{
    Q_OBJECT
public:
    //! Constructor
    /*!
      Sets the color of the highlighter
      \param parent the textedit that this is going to be searching in
     */
    explicit SearchHighlighter(QTextDocument* parent = nullptr);

    //! Search
    /*!
      Search document for the text to highlight
      \param text the text to highlight
     */
    void searchText(const QString& text);
protected:
    //! Block Highlighter
    /*!
      Using the document text highlight based on the user searched regular expression
      \param text the text to highlight
     */
    virtual void highlightBlock(const QString &text) override;

private:
    QRegularExpression pattern; /*!< User query regular expression pattern. */
    QTextCharFormat format; /*!< Format rules for when a text is highlighted. */
};

#endif // SEARCHHIGHLIGHTER_H
