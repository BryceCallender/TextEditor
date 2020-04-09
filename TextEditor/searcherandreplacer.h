#ifndef SEARCHERANDREPLACER_H
#define SEARCHERANDREPLACER_H

#include <QTextEdit>
#include <QRegularExpression>
#include <QDebug>
#include <QShortcut>
#include <QLabel>
#include <QUndoCommand>

/*! \class SearcherAndReplacer
    \brief Find and Replace functionality.

    This class is responsible for searching the document and highlighting/replace words search for.
 */
class SearcherAndReplacer : public QWidget
{
    Q_OBJECT
public:
    //! Constructor
    /*!
      This sets up the necessary variables and makes sure to deallocate space assigned since this is a widget.
      \param textEdit the textedit that this is going to be searching in
      \param label the label that the result text will be fed into
     */
    explicit SearcherAndReplacer(QTextEdit* textEdit, QLabel* label);

    //! populateAllExpressionMatchesAndMoveToFirst
    /*!
      This searches the document for the text and populate the regular expression vector to store the data.
      \param textEdit the textedit that this is going to be searching in
      \param text the text that is being searched
     */
    void populateAllExpressionMatchesAndMoveToFirst(QTextEdit* currentTextEdit, const QString& text);

    //! Constructor
    /*!
      This constructor will call the \a readKeywordFileAndPopulateList function.
     */
    void moveToFirstOccurence(QRegularExpressionMatch& match);

    //! Constructor
    /*!
      This constructor will call the \a readKeywordFileAndPopulateList function.
     */
    void moveToNextOccurence();

    //! Constructor
    /*!
      This constructor will call the \a readKeywordFileAndPopulateList function.
     */
    void moveBackOneOccurence();

    //! Constructor
    /*!
      This constructor will call the \a readKeywordFileAndPopulateList function.
     */
    void replaceCurrent(const QString &findText, const QString &replacementText);

    //! Constructor
    /*!
      This constructor will call the \a readKeywordFileAndPopulateList function.
     */
    void replaceAll(const QString &findText, const QString& replacementText);

    //! Constructor
    /*!
      This constructor will call the \a readKeywordFileAndPopulateList function.
     */
    int getNumberOfMatches();

    //! Constructor
    /*!
      This constructor will call the \a readKeywordFileAndPopulateList function.
     */
    int getCurrentIndex();

    //! Constructor
    /*!
      This constructor will call the \a readKeywordFileAndPopulateList function.
     */
    void setResultsText();
private:
    void moveCursorToOccurence(QRegularExpressionMatch& match);

    int currentIndex;
    QTextEdit* textEdit;
    QLabel* resultsLabel;

    QRegularExpression pattern;
    QVector<QRegularExpressionMatch> textMatches;
};

#endif // SEARCHERANDREPLACER_H
