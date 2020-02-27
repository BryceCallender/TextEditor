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
    findText = new QLineEdit();
    findText->setPlaceholderText("Find");

    findLayout->addWidget(findText);
    findLayout->addWidget(resultsLabel);

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

    fileName = "New File.txt";
    fileWatcher = new QFileSystemWatcher();

    QObject::connect(fileWatcher,
                     &QFileSystemWatcher::fileChanged,
                     this,
                     &TextTabWidget::markTextTabAsDirty);
}

QString TextTabWidget::getTabFileName()
{
    return fileName;
}

void TextTabWidget::setTabsFileName(const QString& name)
{
    //Reset the fileWatcher since its responsible for its own tab and the file name changed
    if(!fileWatcher->files().isEmpty())
    {
        fileWatcher->removePaths(fileWatcher->files());
    }

    fileWatcher->addPath(name);

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

void TextTabWidget::markTextTabAsDirty(const QString &newPath)
{
    qDebug() << "file changed"; //file saved take away the * showing it saved
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


