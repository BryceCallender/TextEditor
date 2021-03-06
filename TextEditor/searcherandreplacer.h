#ifndef SEARCHERANDREPLACER_H
#define SEARCHERANDREPLACER_H

#include <QTextEdit>
#include <QRegularExpression>
#include <QDebug>
#include <QShortcut>
#include <QLabel>

class SearcherAndReplacer : public QWidget
{
    Q_OBJECT
public:
    explicit SearcherAndReplacer(QTextEdit* textEdit, QLabel* label);

    void populateAllExpressionMatchesAndMoveToFirst(QTextEdit* currentTextEdit, const QString& text);

    void moveToFirstOccurence(QRegularExpressionMatch& match);
    void moveToNextOccurence();
    void moveBackOneOccurence();

    void replaceCurrent(const QString &findText, const QString &replacementText);
    void replaceAll(const QString &findText, const QString& replacementText);

    int getNumberOfMatches();
    int getCurrentIndex();

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
