#include "optionswindow.h"
#include "ui_optionswindow.h"

OptionsWindow::OptionsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsWindow)
{
    ui->setupUi(this);

    setWindowTitle("Settings");

    textSettingsLayout = new QVBoxLayout();
    codeSettingsLayout = new QVBoxLayout();

    textSettingsLayout->addWidget(new QLabel("Text Editor Specific Settings"));
    textSettingsLayout->addWidget(new QLabel("Editor: Font\nControls the default Font style for files."));

    QFontComboBox* comboBox = new QFontComboBox();
    comboBox->setCurrentFont(defaultTextFont);
    textSettingsLayout->addWidget(comboBox);

    textSettingsLayout->addWidget(new QLabel("Editor: Font Size\nControls the default Font size."));
    QSpinBox* fontSizeSpinbox = new QSpinBox();
    fontSizeSpinbox->setValue(defaultFontSize);
    fontSizeSpinbox->setRange(8, 48);
    textSettingsLayout->addWidget(fontSizeSpinbox);

    textSettingsLayout->addWidget(new QLabel("Editor: Tab Length\nControls the tab length of word documents. This is different than the tab length for the code editor."));
    QSpinBox* tabLengthSpinbox = new QSpinBox();
    tabLengthSpinbox->setValue(defaultTextTabLength);
    tabLengthSpinbox->setRange(2, 16);
    textSettingsLayout->addWidget(tabLengthSpinbox);

    //ui->textSettingsWidget->setLayout(textSettingsLayout);

    ui->context->addLayout(textSettingsLayout);

    //Code settings window
    codeSettingsLayout->addWidget(new QLabel("Code Editor Specific Settings"));
    codeSettingsLayout->addWidget(new QLabel("Syntax Highlighter:\nControls the highlighting of language specific keywords."));

    codeSettingsLayout->addWidget(new QLabel("Java Syntax Color:\nControls the color of java keywords."));
    QColorDialog* javaColorPicker = new QColorDialog();
    javaColorPicker->setOption(QColorDialog::NoButtons);
    javaColorPicker->setCurrentColor(javaKeywordColor);
    codeSettingsLayout->addWidget(javaColorPicker);

    codeSettingsLayout->addWidget(new QLabel("C++ Syntax Color:\nControls the color of c++ keywords."));
    QColorDialog* cppColorPicker = new QColorDialog();
    cppColorPicker->setOption(QColorDialog::NoButtons);
    cppColorPicker->setCurrentColor(cppKeywordColor);
    codeSettingsLayout->addWidget(cppColorPicker);

    codeSettingsLayout->addWidget(new QLabel("Python Syntax Color:\nControls the color of python keywords."));
    QColorDialog* pythonColorPicker = new QColorDialog();
    pythonColorPicker->setOption(QColorDialog::NoButtons);
    pythonColorPicker->setCurrentColor(pythonKeywordColor);
    codeSettingsLayout->addWidget(pythonColorPicker);

    codeSettingsLayout->addWidget(new QLabel("Code: Tab Length\nControls the tab length of code tabs. This is different than the tab length for the text editor."));
    tabLengthSpinbox = new QSpinBox();
    tabLengthSpinbox->setValue(defaultCodeTabLength);
    tabLengthSpinbox->setRange(2, 16);
    codeSettingsLayout->addWidget(tabLengthSpinbox);

    ui->context->addLayout(codeSettingsLayout);

}

OptionsWindow::~OptionsWindow()
{
    settings.sync();
    delete ui;
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
}
