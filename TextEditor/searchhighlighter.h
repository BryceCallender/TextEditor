#ifndef SEARCHHIGHLIGHTER_H
#define SEARCHHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class SearchHighlighter: public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit SearchHighlighter(QTextDocument* parent = nullptr);

    void searchText(const QString& text);
protected:
    virtual void highlightBlock(const QString &text) override;

private:
    QRegularExpression pattern;
    QTextCharFormat format;
};

#endif // SEARCHHIGHLIGHTER_H
