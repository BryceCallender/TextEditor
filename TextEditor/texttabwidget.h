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

class TextTabWidget: public QWidget
{
    Q_OBJECT
public:
    explicit TextTabWidget(QWidget *parent = nullptr);

    void setTabsFileName(const QString &name);
    QString getTabFileName();
    QString getTabName();

    QTextEdit* getTextEdit();

    void revealSearchBox();
    void revealReplaceBox();

    void setTextEditText(const QString& text);

    void searchTextForQuery(const QString& query);

    void sendFindDataToReplacerAndReplaceCurrent();
    void sendFindDataToReplacerAndReplaceAll();

    void setResultsText(const QString& text);

    void handleCloseEvent();

    void handleBracketAndParenthesisMatch();

    CodeSyntaxHighlighter* getSyntaxHighlighter();
    void setSyntaxHighlighter(CodeSyntaxHighlighter* highlighter);

    void setTabStopDistance(unsigned int length);

    void formatChanged(const QTextCharFormat &format);

//    bool eventFilter(QObject *obj, QEvent *ev) override;

//    void keyPressEvent(QKeyEvent *event) override;
private:
    QString fileName;
    QString tabName;

    SpecialTextEdit *textEditArea;
    QGroupBox *groupBox;
    SpecialLineEdit *findText;
    QLineEdit *replaceText;
    QLabel *resultsLabel;

    QPushButton *replaceCurrentButton;
    QPushButton *replaceAllButton;
    QPushButton *regexExplainer;

    QPushButton *exitButton;

    QKeyEvent *backspaceButton;

    SearchHighlighter *highlighter;
    SearcherAndReplacer *replacer;
    CodeSyntaxHighlighter *codeHighlighter;

};

#endif // TEXTTABWIDGET_H
