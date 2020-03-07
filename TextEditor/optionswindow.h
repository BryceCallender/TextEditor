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

namespace Ui {
class OptionsWindow;
}

class OptionsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsWindow(QWidget *parent = nullptr);
    ~OptionsWindow();

private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_OptionsWindow_rejected();

private:
    Ui::OptionsWindow *ui;
    QSettings settings;

    QFont defaultTextFont = QFont("Times New Roman");
    int defaultFontSize = 12;
    int defaultTextTabLength = 8;

    QColor javaKeywordColor = Qt::darkBlue;
    QColor cppKeywordColor = Qt::darkBlue;
    QColor pythonKeywordColor = Qt::darkBlue;
    int defaultCodeTabLength = 4;

    QVBoxLayout* textSettingsLayout;
    QVBoxLayout* codeSettingsLayout;
};

#endif // OPTIONSWINDOW_H
