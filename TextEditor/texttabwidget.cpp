#include "texttabwidget.h"

TextTabWidget::TextTabWidget(QWidget *parent) : QTabBar(parent)
{
    textEditArea = new QTextEdit();

    highlighter = new SearchHighlighter(textEditArea->document());

    resultsLabel = new QLabel("No results");
     //Takes in the current area it owns and the results label to give feedback to the user on found querys
    replacer = new SearcherAndReplacer(textEditArea, resultsLabel);

    QVBoxLayout* mainLayout = new QVBoxLayout();

    groupBox = new QGroupBox();

    groupBox->setTitle("Search");

    //The whole group box layout that the thing will encompass.
    QVBoxLayout* groupBoxLayout = new QVBoxLayout();

    QHBoxLayout* findLayout = new QHBoxLayout();
    findText = new SpecialLineEdit();
    findText->setPlaceholderText("Find");

    findLayout->addWidget(findText);
    findLayout->addWidget(resultsLabel);

    exitButton = new QPushButton();
    exitButton->setIcon(QIcon(":/imgs/close.png"));

    exitButton->setStyleSheet("QPushButton {background-color: rgba(255, 255, 255, 0);}"
                              "QPushButton:hover {background-color: lightgrey; }");

    findLayout->addWidget(exitButton);

    //Add the find line edit
    groupBoxLayout->addLayout(findLayout);


    //Begin of replace layout
    QHBoxLayout* replaceLayout = new QHBoxLayout();
    replaceText = new QLineEdit();
    replaceText->setPlaceholderText("Replace");

    replaceCurrentButton = new QPushButton("Current");
    replaceAllButton = new QPushButton("All");

    replaceLayout->addWidget(replaceText);
    replaceLayout->addWidget(replaceCurrentButton);
    replaceLayout->addWidget(replaceAllButton);
    //End of replace layout


    replaceCurrentButton->setDisabled(true);
    replaceAllButton->setDisabled(true);


    //Add this layout to the group so that we have a line edit and 2 buttons in this layout
    groupBoxLayout->addLayout(replaceLayout);

    replaceText->hide();
    replaceCurrentButton->hide();
    replaceAllButton->hide();

    groupBox->setLayout(groupBoxLayout);

    mainLayout->addWidget(groupBox);
    mainLayout->addWidget(textEditArea);

    setLayout(mainLayout);

    groupBox->hide();

    // create shortcut
    QShortcut *findShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), this);

    QObject::connect(findShortcut,
                     &QShortcut::activated,
                     this,
                     &TextTabWidget::revealSearchBox);

    // create shortcut
    QShortcut *replaceShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_R), this);

    QObject::connect(replaceShortcut,
                     &QShortcut::activated,
                     this,
                     &TextTabWidget::revealReplaceBox);

    QObject::connect(findText,
                     &QLineEdit::textChanged,
                     this,
                     &TextTabWidget::searchTextForQuery);

    QObject::connect(findText,
                     &QLineEdit::returnPressed,
                     replacer,
                     &SearcherAndReplacer::moveToNextOccurence);

    QObject::connect(replaceText,
                     &QLineEdit::returnPressed,
                     this,
                     &TextTabWidget::sendFindDataToReplacerAndReplaceCurrent);

    QObject::connect(replaceCurrentButton,
                     &QPushButton::pressed,
                     this,
                     &TextTabWidget::sendFindDataToReplacerAndReplaceCurrent);

    QObject::connect(replaceAllButton,
                     &QPushButton::pressed,
                     this,
                     &TextTabWidget::sendFindDataToReplacerAndReplaceAll);

    QObject::connect(exitButton,
                     &QPushButton::pressed,
                     this,
                     &TextTabWidget::handleCloseEvent);

    QObject::connect(textEditArea,
                     &QTextEdit::textChanged,
                     this,
                     &TextTabWidget::handleBracketAndParenthesisMatch);

    fileName = "New File.txt";

    QPalette p = textEditArea->palette();
    p.setColor(QPalette::Highlight, QColor(64, 148, 255));
    textEditArea->setPalette(p);


    //Default code tab count is 4 space units, some have 2, and then for basic essay writing it is going to be 8 units
    //Qt has a tab set as a stop distance of 80 pixels where the width of ' ' is 3 pixels. The stop distance is in terms of pixels as a qreal
    //Reference: https://doc.qt.io/qt-5/qtextedit.html#tabStopDistance-prop
    QFontMetrics metrics(textEditArea->font());
    int tabSpaceCount = 4;
    textEditArea->setTabStopDistance(tabSpaceCount * metrics.width(' '));
}

QString TextTabWidget::getTabFileName()
{
    return fileName;
}

void TextTabWidget::setTabsFileName(const QString& name)
{
    QFileInfo fileInfo(name);

    QString suffix = fileInfo.suffix();
    if(suffix == "java") {
        codeHighlighter = new JavaSyntaxHighlighter(textEditArea->document());
    }else if(suffix == "cpp" || suffix == "h") {
        codeHighlighter = new CPPSyntaxHighlighter(textEditArea->document());
    }else if(suffix == "py") {
        codeHighlighter = new PythonSyntaxHighlighter(textEditArea->document());
    }

    fileName = name;
}

void TextTabWidget::revealSearchBox()
{
    groupBox->show();
    findText->setFocus();
}

void TextTabWidget::revealReplaceBox()
{
    groupBox->setTitle("Search/Replace");
    groupBox->show();
    replaceText->show();
    replaceCurrentButton->show();
    replaceAllButton->show();

    replaceText->setFocus();
}

void TextTabWidget::searchTextForQuery(const QString& query)
{
    highlighter->searchText(query);
    replacer->populateAllExpressionMatchesAndMoveToFirst(textEditArea, query);
    replacer->setResultsText();

    replaceCurrentButton->setDisabled(findText->text().isEmpty());
    replaceAllButton->setDisabled(findText->text().isEmpty());
}

void TextTabWidget::sendFindDataToReplacerAndReplaceCurrent()
{
    if(findText->text().isEmpty())
        return;

    replacer->replaceCurrent(findText->text(), replaceText->text());
}

void TextTabWidget::sendFindDataToReplacerAndReplaceAll()
{
    if(findText->text().isEmpty())
        return;

    replacer->replaceAll(findText->text(), replaceText->text());
}

void TextTabWidget::setResultsText(const QString &text)
{
    resultsLabel->setText(text);
}

void TextTabWidget::handleCloseEvent()
{
    groupBox->hide();
    replaceText->hide();

    //Do we want to clear
    findText->setText("");
    //replaceText->setText("");

    replaceCurrentButton->hide();
    replaceAllButton->hide();
}

void TextTabWidget::handleBracketAndParenthesisMatch()
{
    QFileInfo fileInfo(fileName);

    QString suffix = fileInfo.suffix();
    if(suffix == "cpp" || suffix == "h" || suffix == "java") {
        if(textEditArea->toPlainText().endsWith("{")) {
            textEditArea->textCursor().insertText("}");
            textEditArea->textCursor().movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
        }
    }
}

void TextTabWidget::setTextEditText(const QString &text)
{
    textEditArea->setPlainText(text);
}

void TextTabWidget::setTabNameText(int index, const QString &text)
{
    setTabText(index, text);
}

QTextEdit* TextTabWidget::getTextEdit()
{
    return textEditArea;
}


