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
#include <QFileSystemWatcher>

#include "searchhighlighter.h"
#include "searcherandreplacer.h"
#include "javasyntaxhighlighter.h"
#include "cppsyntaxhighlighter.h"
#include "pythonsyntaxhighlighter.h"
#include "speciallineedit.h"

class TextTabWidget : public QTabBar
{
    Q_OBJECT
public:
    explicit TextTabWidget(QWidget *parent = nullptr);

    void setTabsFileName(const QString &name);
    QString getTabFileName();

    QTextEdit* getTextEdit();

    void revealSearchBox();
    void revealReplaceBox();

    void setTextEditText(const QString& text);
    void setTabNameText(int index, const QString& text);

    void searchTextForQuery(const QString& query);

    void sendFindDataToReplacerAndReplaceCurrent();
    void sendFindDataToReplacerAndReplaceAll();

    void setResultsText(const QString& text);

    void markTextTabAsDirty(const QString& newPath);
private:
    QString fileName;

    QFileSystemWatcher *fileWatcher;

    QTextEdit *textEditArea;
    QGroupBox *groupBox;
    SpecialLineEdit *findText;
    QLineEdit *replaceText;
    QLabel *resultsLabel;

    QPushButton *replaceCurrentButton;
    QPushButton *replaceAllButton;

    SearchHighlighter *highlighter;
    SearcherAndReplacer *replacer;
    CodeSyntaxHighlighter *codeHighlighter;
};

#endif // TEXTTABWIDGET_H
