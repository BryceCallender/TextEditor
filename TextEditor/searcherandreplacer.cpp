#include "searcherandreplacer.h"

SearcherAndReplacer::SearcherAndReplacer(QTextEdit* textEdit, QLabel *label): QWidget(textEdit)
{
    this->textEdit = textEdit;
    this->resultsLabel = label;
}

void SearcherAndReplacer::populateAllExpressionMatchesAndMoveToFirst(QTextEdit *currentTextEdit, const QString &text)
{
    textEdit = currentTextEdit;

    //Reset any matches that may have happened
    textMatches.clear();
    currentIndex = 0;

    //Set the pattern and match the pattern globally against the whole document text
    pattern = QRegularExpression(text);
    QRegularExpressionMatchIterator iterator = pattern.globalMatch(textEdit->toPlainText());

    while (iterator.hasNext())
    {
        QRegularExpressionMatch match = iterator.next();
        if(match.hasMatch())
        {
            textMatches.push_back(match);
        }
    }

    //Pass the first match (if we have one) to the cursor in order for it to find it and select it
    if(textMatches.size() > 0)
    {
        moveToFirstOccurence(textMatches.front());
    }
}

void SearcherAndReplacer::moveToFirstOccurence(QRegularExpressionMatch& match)
{
    moveCursorToOccurence(match);
}

void SearcherAndReplacer::moveToNextOccurence()
{
    if(textMatches.size() > 0)
    {
        currentIndex = ++currentIndex % textMatches.size();
        setResultsText();
        moveCursorToOccurence(textMatches[currentIndex]);
    }
}

void SearcherAndReplacer::moveCursorToOccurence(QRegularExpressionMatch &match)
{
    QTextCursor cursor = textEdit->textCursor();

    //Get to the beginning of the text
    cursor.movePosition(QTextCursor::Start);

    //Move until we reach the first texts captured starting point
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, match.capturedStart());

    //Keep anchor where the beginning was and move to the right length amount of times in order
    //for the cursor to select the text
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, match.capturedLength());

    textEdit->setTextCursor(cursor);
}

void SearcherAndReplacer::replaceCurrent(const QString& findText, const QString& replacementText)
{
    if(textMatches.size() > 0)
    {
        QRegularExpressionMatch match = textMatches.at(currentIndex);

        moveCursorToOccurence(match);

        QTextCursor cursor = textEdit->textCursor();
        cursor.insertText(replacementText);

        populateAllExpressionMatchesAndMoveToFirst(textEdit, findText); //Update the matches since the match start and end is no longer valid
    }
}

void SearcherAndReplacer::replaceAll(const QString &findText, const QString& replacementText)
{
    if(textMatches.size() > 0)
    {
        while(textMatches.size() > 0)
        {
            replaceCurrent(findText, replacementText);
        }
    }
}

int SearcherAndReplacer::getNumberOfMatches()
{
    return textMatches.length();
}

int SearcherAndReplacer::getCurrentIndex()
{
    return currentIndex;
}

void SearcherAndReplacer::setResultsText()
{
    if(textMatches.size() == 0)
    {
        resultsLabel->setText("No results");
    }
    else
    {
        resultsLabel->setText(QString::number(currentIndex + 1) + " of " + QString::number(textMatches.size()));
    }
}
