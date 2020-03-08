#include "optionswindow.h"
#include "ui_optionswindow.h"

OptionsWindow::OptionsWindow(QTabWidget *tabWidget, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsWindow)
{
    ui->setupUi(this);

    setWindowTitle("Settings");

    tabs = tabWidget;

    settings = SettingsManager::getInstance();

    textSettingsLayout = new QVBoxLayout();
    codeSettingsLayout = new QVBoxLayout();

    textSettingsLayout->addWidget(new QLabel("<h1>Text Editor Specific Settings</h1>"));
    textSettingsLayout->addWidget(new QLabel("<h3>Editor: Font</h3>"));
    textSettingsLayout->addWidget(new QLabel("Controls the default Font style for files."));

    QFontComboBox* comboBox = new QFontComboBox();
    comboBox->setCurrentFont(settings->getValue("text/font").value<QFont>());
    textSettingsLayout->addWidget(comboBox);

    textSettingsLayout->addWidget(new QLabel("<h3>Editor: Font Size</h3>"));
    textSettingsLayout->addWidget(new QLabel("Controls the default Font size."));
    QSpinBox* fontSizeSpinbox = new QSpinBox();
    fontSizeSpinbox->setValue(settings->getValue("text/fontSize").toInt());
    fontSizeSpinbox->setRange(8, 48);
    textSettingsLayout->addWidget(fontSizeSpinbox);

    textSettingsLayout->addWidget(new QLabel("<h3>Editor: Tab Length</h3>"));
    textSettingsLayout->addWidget(new QLabel("Controls the tab length of word documents. This is different than the tab length for the code editor."));

    QSpinBox* tabLengthSpinbox = new QSpinBox();
    tabLengthSpinbox->setValue(settings->getValue("text/tabLength").toInt());
    tabLengthSpinbox->setRange(2, 16);
    textSettingsLayout->addWidget(tabLengthSpinbox);

    //ui->textSettingsWidget->setLayout(textSettingsLayout);

    ui->context->addLayout(textSettingsLayout);

    //Code settings window
    codeSettingsLayout->addWidget(new QLabel("<h1>Code Editor Specific Settings</h1>"));

    codeSettingsLayout->addWidget(new QLabel("<h2>Code: Syntax Highlighter</h2>"));
    codeSettingsLayout->addWidget(new QLabel("Controls the highlighting of language specific keywords."));

    codeSettingsLayout->addWidget(new QLabel("<h3>Java Syntax Color:</h3>"));
    codeSettingsLayout->addWidget(new QLabel("Controls the color of java keywords."));

    QColorDialog* javaColorPicker = new QColorDialog();
    javaColorPicker->setOption(QColorDialog::NoButtons);
    javaColorPicker->setCurrentColor(settings->getValue("code/javaKeywordColor").value<QColor>());
    codeSettingsLayout->addWidget(javaColorPicker);

    connect(javaColorPicker,
            &QColorDialog::currentColorChanged,
            this,
            [=](const QColor &color) {
                SettingsManager::getInstance()->saveValue("code", "javaKeywordColor", color);
            }
    );

    codeSettingsLayout->addWidget(new QLabel("<h3>C++ Syntax Color:</h3>"));
    codeSettingsLayout->addWidget(new QLabel("Controls the color of c++ keywords."));

    QColorDialog* cppColorPicker = new QColorDialog();
    cppColorPicker->setOption(QColorDialog::NoButtons);
    cppColorPicker->setCurrentColor(settings->getValue("code/cppKeywordColor").value<QColor>());
    codeSettingsLayout->addWidget(cppColorPicker);

    connect(cppColorPicker,
            &QColorDialog::currentColorChanged,
            this,
            [=](const QColor &color) {
                SettingsManager::getInstance()->saveValue("code", "cppKeywordColor", color);
            }
    );

    codeSettingsLayout->addWidget(new QLabel("<h3>Python Syntax Color:</h3>"));
    codeSettingsLayout->addWidget(new QLabel("Controls the color of python keywords."));

    QColorDialog* pythonColorPicker = new QColorDialog();
    pythonColorPicker->setOption(QColorDialog::NoButtons);
    pythonColorPicker->setCurrentColor(settings->getValue("code/pythonKeywordColor").value<QColor>());
    codeSettingsLayout->addWidget(pythonColorPicker);

    connect(pythonColorPicker,
            &QColorDialog::currentColorChanged,
            this,
            [=](const QColor &color) {
                SettingsManager::getInstance()->saveValue("code", "pythonKeywordColor", color);
            }
    );

    codeSettingsLayout->addWidget(new QLabel("<h3>Code: Tab Length</h3>"));
    codeSettingsLayout->addWidget(new QLabel("Controls the tab length of code tabs. This is different than the tab length for the text editor."));

    tabLengthSpinbox = new QSpinBox();
    tabLengthSpinbox->setValue(defaultCodeTabLength);
    tabLengthSpinbox->setRange(2, 16);
    codeSettingsLayout->addWidget(tabLengthSpinbox);

    ui->context->addLayout(codeSettingsLayout);

}

OptionsWindow::~OptionsWindow()
{
    settings->saveSettings();
    delete ui;
    delete textSettingsLayout;
    delete codeSettingsLayout;
}

void OptionsWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    QString clickedText = item->text(column);

    if(item->text(column) == "Text Editor" || item->parent()->text(column) == "Text Editor")
    {

    }
    else if(item->text(column) == "Code Editor" || item->parent()->text(column) == "Code Editor")
    {

    }
}

void OptionsWindow::on_OptionsWindow_rejected()
{
    qDebug() << "Closed";
    for(int i = 0; i < tabs->count() - 1; i++)
    {
        TextTabWidget* tab = dynamic_cast<TextTabWidget*>(tabs->widget(i));
        CodeSyntaxHighlighter* highlighter = tab->getSyntaxHighlighter();
        CodeSyntaxHighlighter* highlighterType = tab->getSyntaxHighlighter();
        if((highlighterType = dynamic_cast<JavaSyntaxHighlighter*>(highlighter)) != nullptr)
        {
            highlighterType->updateKeywordColor(settings->getValue("code/javaKeywordColor").value<QColor>());
            qDebug() << "Java updated!";
        }
        else if((highlighterType = dynamic_cast<CPPSyntaxHighlighter*>(highlighter)) != nullptr)
        {
            highlighterType->updateKeywordColor(settings->getValue("code/cppKeywordColor").value<QColor>());
            qDebug() << "C++ updated!";
        }
        else if((highlighterType = dynamic_cast<PythonSyntaxHighlighter*>(highlighter)) != nullptr)
        {
            highlighterType->updateKeywordColor(settings->getValue("code/pythonKeywordColor").value<QColor>());
            qDebug() << "Python updated!";
        }
    }
}
