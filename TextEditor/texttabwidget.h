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

#include "searchhighlighter.h"

class TextTabWidget : public QTabBar
{
    Q_OBJECT
public:
    explicit TextTabWidget(QWidget *parent = nullptr);

    void setTabsFileName(QString name);
    QString getTabFileName();

    QTextEdit* getTextEdit();

    void revealSearchBox();
    void revealReplaceBox();

    void setTextEditText(const QString& text);
    void setTabNameText( int index, const QString& text);

    void searchTextForQuery(const QString& query);
private:
    QString fileName;

    QTextEdit *textEditArea;
    QGroupBox *groupBox;
    QLineEdit *findText;
    QLineEdit *replaceText;

    SearchHighlighter* highlighter;
};

#endif // TEXTTABWIDGET_H
