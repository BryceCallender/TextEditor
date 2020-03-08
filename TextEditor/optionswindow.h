#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <QDialog>
#include <QSettings>
#include <QTreeWidget>
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QColorDialog>
#include <QSpinBox>
#include <QFontComboBox>

#include "settingsmanager.h"
#include "texttabwidget.h"

namespace Ui {
class OptionsWindow;
}

class OptionsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsWindow(QTabWidget* tabWidget, QWidget *parent = nullptr);
    ~OptionsWindow();

private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_OptionsWindow_rejected();

private:
    Ui::OptionsWindow *ui;

    QFont defaultTextFont;
    int defaultFontSize;
    int defaultTextTabLength;

    QColor javaKeywordColor;
    QColor cppKeywordColor;
    QColor pythonKeywordColor;
    int defaultCodeTabLength;

    QVBoxLayout* textSettingsLayout;
    QVBoxLayout* codeSettingsLayout;

    QTabWidget* tabs;

    SettingsManager* settings;
};

#endif // OPTIONSWINDOW_H
