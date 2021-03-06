#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "texttabwidget.h"

TextTabWidget::TextTabWidget(QWidget *parent): QWidget(parent)
{
    textEditArea = new SpecialTextEdit(this);

    highlighter = new SearchHighlighter(textEditArea->document());
    codeHighlighter = nullptr;

    resultsLabel = new QLabel("No results");
     //Takes in the current area it owns and the results label to give feedback to the user on found querys
    replacer = new SearcherAndReplacer(textEditArea, resultsLabel);

    QVBoxLayout* mainLayout = new QVBoxLayout();

    groupBox = new QGroupBox(this);

    groupBox->setTitle("Search");

    //The whole group box layout that the thing will encompass.
    QVBoxLayout* groupBoxLayout = new QVBoxLayout();

    QHBoxLayout* findLayout = new QHBoxLayout();
    findText = new SpecialLineEdit(this);
    findText->setPlaceholderText("Find");

    findLayout->addWidget(findText);
    findLayout->addWidget(resultsLabel);

    QPushButton* upArrow = new QPushButton(this);
    upArrow->setIcon(QPixmap(":/imgs/icon/up-arrow.svg"));

    connect(upArrow,
            &QPushButton::pressed,
            replacer,
            &SearcherAndReplacer::moveToNextOccurence);

    QPushButton* downArrow = new QPushButton(this);
    downArrow->setIcon(QPixmap(":/imgs/icon/down-arrow.svg"));

    connect(downArrow,
            &QPushButton::pressed,
            replacer,
            &SearcherAndReplacer::moveBackOneOccurence);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(downArrow);
    buttonLayout->addWidget(upArrow);
    buttonLayout->setSpacing(0);

    findLayout->addLayout(buttonLayout);

    exitButton = new QPushButton(this);
    exitButton->setIcon(QIcon(":/imgs/icon/close.png"));

    exitButton->setStyleSheet("QPushButton {background-color: rgba(255, 255, 255, 0);}"
                              "QPushButton:hover {background-color: lightgrey; }");

    findLayout->addWidget(exitButton);

    //Add the find line edit
    groupBoxLayout->addLayout(findLayout);


    //Begin of replace layout
    QHBoxLayout* replaceLayout = new QHBoxLayout();
    replaceText = new QLineEdit(this);
    replaceText->setPlaceholderText("Replace");

    replaceCurrentButton = new QPushButton("Current", this);
    replaceAllButton = new QPushButton("All", this);

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
    QShortcut *findShortcut = new QShortcut(QKeySequence::Find, this);

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
                     &QTextEdit::currentCharFormatChanged,
                     this,
                     &TextTabWidget::formatChanged);

//    QObject::connect(textEditArea,
//                     &QTextEdit::textChanged,
//                     this,
//                     &TextTabWidget::handleBracketAndParenthesisMatch);

    fileName = "New File.txt";

    QPalette p = textEditArea->palette();
    p.setColor(QPalette::Highlight, QColor(64, 148, 255));
    textEditArea->setPalette(p);

    //Sets the font based on the current settings and applies it.
    QFont currentFont = textEditArea->font();
    currentFont.setFamily(SettingsManager::getInstance()->getValue("text/fontFamily").toString());
    currentFont.setPointSize(SettingsManager::getInstance()->getValue("text/fontSize").toInt());
    textEditArea->setFont(currentFont);

    //Sets tab length based on the current settings and applies it as well.
    setTabStopDistance(SettingsManager::getInstance()->getValue("text/tabLength").toInt());
}

QString TextTabWidget::getTabFileName()
{
    return fileName;
}

QString TextTabWidget::getTabName()
{
    return tabName;
}

void TextTabWidget::setTabsFileName(const QString& name)
{
    QFileInfo fileInfo(name);

    QString suffix = fileInfo.suffix();
    if(suffix == "java")
    {
       codeHighlighter = new JavaSyntaxHighlighter(textEditArea->document());
    }
    else if(suffix == "cpp" || suffix == "h")
    {
        codeHighlighter = new CPPSyntaxHighlighter(textEditArea->document());
    }
    else if(suffix == "py")
    {
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

    //Regain the color!
    if(codeHighlighter != nullptr)
    {
        codeHighlighter->rehighlight();
    }

}

void TextTabWidget::handleBracketAndParenthesisMatch()
{
    QFileInfo fileInfo(fileName);

    QString suffix = fileInfo.suffix();
    if(suffix == "cpp" || suffix == "h" || suffix == "java")
    {
        if(textEditArea->toPlainText().endsWith("{"))
        {
            qDebug() << "{ found";
            QTextCursor cursor = textEditArea->textCursor();
            cursor.insertText("}");
            cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
            textEditArea->setTextCursor(cursor);
        }
    }
}

CodeSyntaxHighlighter* TextTabWidget::getSyntaxHighlighter()
{
    return codeHighlighter;
}

void TextTabWidget::setSyntaxHighlighter(CodeSyntaxHighlighter *highlighter)
{
    codeHighlighter = highlighter;
}

void TextTabWidget::setTabStopDistance(unsigned int length)
{
    //Default code tab count is 4 space units, some have 2, and then for basic essay writing it is going to be 8 units
    //Qt has a tab set as a stop distance of 80 pixels where the width of ' ' is 3 pixels. The stop distance is in terms of pixels as a qreal
    //Reference: https://doc.qt.io/qt-5/qtextedit.html#tabStopDistance-prop
    QFontMetrics metrics(textEditArea->font());
    textEditArea->setTabStopDistance(length * metrics.width(' '));
}

void TextTabWidget::setTextEditText(const QString &text)
{
    textEditArea->setPlainText(text);
}

QTextEdit* TextTabWidget::getTextEdit()
{
    return textEditArea;
}

void TextTabWidget::formatChanged(const QTextCharFormat &format)
{
    MainWindow* mainWindow;
    //If this has no dock widget take this route
    if(dynamic_cast<MainWindow*>(parentWidget()->parentWidget()) != nullptr) {
        mainWindow = reinterpret_cast<MainWindow*>(parentWidget()->parentWidget());
    }else { //everything else is inside of a dock widget so it has one more parent to get through
        mainWindow = reinterpret_cast<MainWindow*>(parentWidget()->parentWidget()->parentWidget());
    }
    QTextCursor cursor = textEditArea->textCursor();

    if(format.fontWeight() != QFont::Bold && mainWindow->get_UI().actionBold->isChecked())
    {
        mainWindow->get_UI().actionBold->setChecked(false);
    }
    else if (format.fontWeight() == QFont::Bold && !mainWindow->get_UI().actionBold->isChecked())
    {
        mainWindow->get_UI().actionBold->setChecked(true);
    }

    if(!format.fontItalic() && mainWindow->get_UI().actionItalic->isChecked())
    {
        mainWindow->get_UI().actionItalic->setChecked(false);
    }
    else if (format.fontItalic() && !mainWindow->get_UI().actionItalic->isChecked())
    {
        mainWindow->get_UI().actionItalic->setChecked(true);
    }

    if(!format.fontUnderline() && mainWindow->get_UI().actionUnderline->isChecked())
    {
        mainWindow->get_UI().actionUnderline->setChecked(false);
    }
    else if (format.fontUnderline() && !mainWindow->get_UI().actionUnderline->isChecked())
    {
        mainWindow->get_UI().actionUnderline->setChecked(true);
    }


    if(format.fontPointSize() != 0 && !cursor.hasSelection())
    {
        if(format.fontPointSize() != mainWindow->get_UI().fontSizeComboBox->currentText().toInt())
        {
            int index = mainWindow->get_UI().fontSizeComboBox->findText(QString::number(format.fontPointSize()));
            mainWindow->get_UI().fontSizeComboBox->setCurrentIndex(index);
        }
    }

    if(format.fontFamily() != NULL && !cursor.hasSelection())
    {
        if(format.fontFamily() != mainWindow->get_UI().fontComboBox->currentText())
        {
            int index = mainWindow->get_UI().fontComboBox->findText(format.fontFamily());
            mainWindow->get_UI().fontComboBox->setCurrentIndex(index);
        }

    }
}



