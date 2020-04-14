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

    This class is responsible for searching the document for user queries.
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

    //! Travel to first regex match
    /*!
      This function takes in a matching regular expression to look for and will move to the first occurence in the document.
      \param match The regular expression to match the document against
     */
    void moveToFirstOccurence(QRegularExpressionMatch& match);

    //! Next Match
    /*!
      Will move the cursor and select the next occurence of the current regular expression matching criteria.
     */
    void moveToNextOccurence();

    //! Previous Match
    /*!
      Will move the cursor and select the previous occurence from the current selection using the criteria from search regular expression.
     */
    void moveBackOneOccurence();

    //! Replace Once
    /*!
      Will replace the currently selected item using the replacement text.
      \param findText What to search for
      \param replacementText What to replace the found text with
     */
    void replaceCurrent(const QString &findText, const QString &replacementText);

    //! Replace All
    /*!
      Will replace every single occurence of the searching text with the new text.
      \param findText What to search for
      \param replacementText What to replace the found text with
     */
    void replaceAll(const QString &findText, const QString& replacementText);

    //! Number of Matches
    /*!
      Displays the number of matches to a UI showing how many results were found.
     */
    int getNumberOfMatches();

    //! Current Match Index
    /*!
      Shows the user what match they are currently at.
     */
    int getCurrentIndex();

    //! Results Text
    /*!
      Sets the UI text to set the results of the search. E.g No results found, 1 of 5, etc...
     */
    void setResultsText();
private:
    //! Move Cursor
    /*!
      Moves the cursor and selects the match supplied.
      \param match The regular expression match which contains the start and length of match to use to move cursor
     */
    void moveCursorToOccurence(QRegularExpressionMatch& match);

    int currentIndex; /*!< Current index of the user when searching through results. */
    QTextEdit* textEdit; /*!< Supplied for searching through its contents. */
    QLabel* resultsLabel; /*!< UI Label to give feedback to the user on results from query. */

    QRegularExpression pattern; /*!< Pattern generated when user supplies text to query. */
    QVector<QRegularExpressionMatch> textMatches; /*!< Stores all the matches made from the user query. */
};

#endif // SEARCHERANDREPLACER_H
