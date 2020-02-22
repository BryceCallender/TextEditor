#include "texttabwidget.h"

TextTabWidget::TextTabWidget(QWidget *parent) : QTabBar(parent)
{
    textEditArea = new QTextEdit();
    QVBoxLayout* mainLayout = new QVBoxLayout();

    groupBox = new QGroupBox();

    groupBox->setTitle("Search");

    QVBoxLayout* groupBoxLayout = new QVBoxLayout();

    findText = new QLineEdit();
    replaceText = new QLineEdit();

    findText->setPlaceholderText("Find");
    replaceText->setPlaceholderText("Replace");

    groupBoxLayout->addWidget(findText);
    groupBoxLayout->addWidget(replaceText);

    replaceText->hide();

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

    highlighter = new SearchHighlighter(textEditArea->document());

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
    replaceText->setFocus();
}

void TextTabWidget::searchTextForQuery(const QString& query)
{
    highlighter->searchText(query);
}

void TextTabWidget::setTextEditText(const QString &text)
{
    textEditArea->setPlainText(text);
}

void TextTabWidget::setTabNameText(int index, const QString &text)
{
    setTabText(index, text);

    qDebug() << text;
}

QTextEdit* TextTabWidget::getTextEdit()
{
    return textEditArea;
}


