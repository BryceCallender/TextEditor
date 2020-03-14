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
    textSettingsLayout->addWidget(new QLabel("<h3>Editor: Font Family</h3>"));
    textSettingsLayout->addWidget(new QLabel("Controls the default Font family for files."));

    fontFamilyComboBox = new QFontComboBox();
    fontFamilyComboBox->setCurrentFont(settings->getValue("text/fontFamily").value<QFont>());
    textSettingsLayout->addWidget(fontFamilyComboBox);

    connect(fontFamilyComboBox,
            &QFontComboBox::currentFontChanged,
            this,
            [=](const QFont& value) {
                SettingsManager::getInstance()->saveValue("text", "fontSize", value);
            }
    );

    textSettingsLayout->addWidget(new QLabel("<h3>Editor: Font Size</h3>"));
    textSettingsLayout->addWidget(new QLabel("Controls the default Font size."));
    fontSizeSpinbox = new QSpinBox();
    fontSizeSpinbox->setValue(settings->getValue("text/fontSize").toInt());
    fontSizeSpinbox->setRange(8, 48);

    connect(fontSizeSpinbox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            [=](int value) {
                SettingsManager::getInstance()->saveValue("text", "fontSize", value);
            }
    );

    textSettingsLayout->addWidget(fontSizeSpinbox);

    textSettingsLayout->addWidget(new QLabel("<h3>Editor: Tab Length</h3>"));
    textSettingsLayout->addWidget(new QLabel("Controls the tab length of word documents. This is different than the tab length for the code editor."));

    textTabLengthSpinbox = new QSpinBox();
    textTabLengthSpinbox->setValue(settings->getValue("text/tabLength").toInt());
    textTabLengthSpinbox->setRange(2, 16);
    textSettingsLayout->addWidget(textTabLengthSpinbox);

    connect(textTabLengthSpinbox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            [=](int value) {
                SettingsManager::getInstance()->saveValue("text", "tabLength", value);
            }
    );

    ui->context->addLayout(textSettingsLayout);

    //Code settings window
    codeSettingsLayout->addWidget(new QLabel("<h1>Code Editor Specific Settings</h1>"));

    codeSettingsLayout->addWidget(new QLabel("<h2>Code: Syntax Highlighter</h2>"));
    codeSettingsLayout->addWidget(new QLabel("Controls the highlighting of language specific keywords."));

    codeSettingsLayout->addWidget(new QLabel("<h3>Java Syntax Color:</h3>"));
    codeSettingsLayout->addWidget(new QLabel("Controls the color of java keywords."));

    javaColorPicker = new QColorDialog(this);
    javaColorPicker->setOptions(QColorDialog::NoButtons | QColorDialog::DontUseNativeDialog);
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

    cppColorPicker = new QColorDialog(this);
    cppColorPicker->setOptions(QColorDialog::NoButtons | QColorDialog::DontUseNativeDialog);
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

    pythonColorPicker = new QColorDialog(this);
    pythonColorPicker->setOptions(QColorDialog::NoButtons | QColorDialog::DontUseNativeDialog);
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

    codeTabLengthSpinBox = new QSpinBox();
    codeTabLengthSpinBox->setValue(settings->getValue("code/tabLength").toInt());
    codeTabLengthSpinBox->setRange(2, 16);
    codeSettingsLayout->addWidget(codeTabLengthSpinBox);

    connect(codeTabLengthSpinBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            [=](int value) {
                SettingsManager::getInstance()->saveValue("code", "tabLength", value);
            }
    );


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

    if(clickedText == "Font Family")
    {
        ui->scrollArea->ensureWidgetVisible(fontFamilyComboBox);
        fontFamilyComboBox->setFocus();
    }
    else if(clickedText == "Font Size")
    {
        ui->scrollArea->ensureWidgetVisible(fontSizeSpinbox);
        fontSizeSpinbox->setFocus();
    }
    else if(item->parent() != nullptr && item->parent()->text(column) == "Text Editor" && clickedText == "Tab Length")
    {
        ui->scrollArea->ensureWidgetVisible(textTabLengthSpinbox);
        textTabLengthSpinbox->setFocus();
    }
    else if(clickedText == "Java")
    {
        ui->scrollArea->ensureWidgetVisible(javaColorPicker);
        javaColorPicker->setFocus();
    }
    else if(clickedText == "C++")
    {
        ui->scrollArea->ensureWidgetVisible(cppColorPicker);
        cppColorPicker->setFocus();
    }
    else if(clickedText == "Python")
    {
        ui->scrollArea->ensureWidgetVisible(pythonColorPicker);
        pythonColorPicker->setFocus();
    }
    else if(item->parent() != nullptr && item->parent()->text(column) == "Code Editor" && clickedText == "Tab Length")
    {
        ui->scrollArea->ensureWidgetVisible(codeTabLengthSpinBox);
        codeTabLengthSpinBox->setFocus();
    }
}

void OptionsWindow::on_OptionsWindow_rejected()
{
    qDebug() << "Closed";

    //Update the code color per tab based on which code syntax highlighter each one has
    for(int i = 0; i < tabs->count() - 1; i++)
    {
        TextTabWidget* tab = dynamic_cast<TextTabWidget*>(tabs->widget(i));
        CodeSyntaxHighlighter* highlighter = tab->getSyntaxHighlighter();
        CodeSyntaxHighlighter* highlighterType = tab->getSyntaxHighlighter();

        //The tab has a highlighter object defined
        if(highlighter != nullptr)
        {
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
}
