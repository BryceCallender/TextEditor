#include "texttabwidget.h"

TextTabWidget::TextTabWidget(QWidget *parent) : QTabBar(parent)
{
    textEditArea = new QTextEdit();

    highlighter = new SearchHighlighter(textEditArea->document());
    replacer = new SearcherAndReplacer(textEditArea);

    QVBoxLayout* mainLayout = new QVBoxLayout();

    groupBox = new QGroupBox();

    groupBox->setTitle("Search");

    //The whole group box layout that the thing will encompass.
    QVBoxLayout* groupBoxLayout = new QVBoxLayout();
    findText = new QLineEdit();
    findText->setPlaceholderText("Find");

    //Add the find line edit
    groupBoxLayout->addWidget(findText);



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


    fileName = "New File.txt";
}

QString TextTabWidget::getTabFileName()
{
    return fileName;
}

void TextTabWidget::setTabsFileName(QString name)
{
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
    replaceText->show();
    replaceCurrentButton->show();
    replaceAllButton->show();

    replaceText->setFocus();
}

void TextTabWidget::searchTextForQuery(const QString& query)
{
    highlighter->searchText(query);
    replacer->populateAllExpressionMatchesAndMoveToFirst(textEditArea, query);
}

void TextTabWidget::sendFindDataToReplacerAndReplaceCurrent()
{
    replacer->replaceCurrent(findText->text(), replaceText->text());
}

void TextTabWidget::sendFindDataToReplacerAndReplaceAll()
{
    replacer->replaceAll(findText->text(), replaceText->text());
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


