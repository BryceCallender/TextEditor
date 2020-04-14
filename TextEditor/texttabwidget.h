#ifndef TEXTTABWIDGET_H
#define TEXTTABWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QGroupBox>
#include <QLineEdit>
#include <QShortcut>
#include <QtDebug>
#include <QPushButton>
#include <QLabel>
#include <QFileInfo>
#include <QPalette>
#include <QTextCursor>
#include <QFileSystemWatcher>
#include <QMessageBox>

#include "searchhighlighter.h"
#include "searcherandreplacer.h"
#include "javasyntaxhighlighter.h"
#include "cppsyntaxhighlighter.h"
#include "pythonsyntaxhighlighter.h"
#include "speciallineedit.h"
#include "specialtextedit.h"


/*! \class TextTabWidget
    \brief The Tab Widgets containing all the text edit information.

    This class is responsible for displaying the file contents to the user and the text they are currently writing.
 */
class TextTabWidget: public QWidget
{
    Q_OBJECT
public:
    explicit TextTabWidget(QWidget *parent = nullptr);

    //! Set tab file name
    /*!
      Sets the file name and sets up the syntax highlighter if the extension matches a supported language
      \param name File path to the file
     */
    void setTabsFileName(const QString &name);

    //! Tab File Name
    /*!
      Returns the Tabs file name
     */
    QString getTabFileName();

    //! Get Tab Name
    /*!
      Returns the tab name
     */
    QString getTabName();

    //! Get TextEdit Name
    /*!
      Returns the text edit for the current widget
     */
    QTextEdit* getTextEdit();

    //! Reveal Search
    /*!
      Reveals the search box for a user to search for text
     */
    void revealSearchBox();

    //! Reveal Replace Box
    /*!
      Reveals the search box(if hidden) and replace box for a user to search for text
     */
    void revealReplaceBox();

    //! Sets the text edit text
    /*!
      Sets the text edit text when transferring tabs to other widgets
     */
    void setTextEditText(const QString& text);

    //! Query Search
    /*!
      Sends the query to be searched and highlighted. Gets the Search/Replace ready
     */
    void searchTextForQuery(const QString& query);

    //! Replace Current Data Submission
    /*!
      Sends the query to be the Search/Replacer to replace current occurence.
     */
    void sendFindDataToReplacerAndReplaceCurrent();

    //! Replace All Data Submission
    /*!
      Sends the query to be the Search/Replacer to replace all occurences.
     */
    void sendFindDataToReplacerAndReplaceAll();

    //! Result text setting
    /*!
      Tells the class to set the results text
     */
    void setResultsText(const QString& text);

    //! Close Event
    /*!
      Handles hiding the search and replace box as well as rehighlighting from the color loss from the search highlighter
     */
    void handleCloseEvent();

    //! Get highlighter
    /*!
      Gets the CodeSyntaxHighlighter responsible for coloring the tab widget
     */
    CodeSyntaxHighlighter* getSyntaxHighlighter();

    //! Set highlighter
    /*!
      Sets the CodeSyntaxHighlighter responsible for coloring the tab widget
     */
    void setSyntaxHighlighter(CodeSyntaxHighlighter* highlighter);

    //! Tab Count
    /*!
      Sets the distance for when a user hits tab
     */
    void setTabStopDistance(unsigned int length);

    //! Format Changed
    /*!
      Responsible slot for when the cursor goes to a different format
     */
    void formatChanged(const QTextCharFormat &format);
private:
    QString fileName; /*!< File Name for the widget. */
    QString tabName; /*!< Tab name for the widget. */

    SpecialTextEdit *textEditArea; /*!< The text edit responsible for hosting all the text. */
    QGroupBox *groupBox; /*!< Holds the search/replacer widgets. */
    SpecialLineEdit *findText;
    QLineEdit *replaceText;
    QLabel *resultsLabel;

    QPushButton *replaceCurrentButton;
    QPushButton *replaceAllButton;
    QPushButton *regexExplainer;

    QPushButton *exitButton;

    QKeyEvent *backspaceButton;

    SearchHighlighter *highlighter; /*!< Search highlighter responsible for coloring the words in the document. */
    SearcherAndReplacer *replacer; /*!< Logic for searching and replacing. */
    CodeSyntaxHighlighter *codeHighlighter; /*!< Highlighter if the text is from cpp, java, or python. */

};

#endif // TEXTTABWIDGET_H
