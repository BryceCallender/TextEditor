#include "optionswindow.h"
#include "ui_optionswindow.h"

OptionsWindow::OptionsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsWindow)
{
    ui->setupUi(this);

    setWindowTitle("Settings");

    QVBoxLayout* textSettingsLayout = new QVBoxLayout();
    QVBoxLayout* codeSettingsLayout = new QVBoxLayout();

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

    ui->textSettingsWidget->setLayout(textSettingsLayout);

    //Code settings window

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
        ui->textSettingsWidget->show();
        ui->codeSettingsWidget->hide();
    }
    else if(item->text(column) == "Code Editor" || item->parent()->text(column) == "Code Editor")
    {
        ui->textSettingsWidget->hide();
        ui->codeSettingsWidget->show();
    }
}
