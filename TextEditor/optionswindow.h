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
    //! Constructor
    /*!
      This constructor will construct the window and show the user the option window. It is also responsible for loading the settings and showing them to the user.
     */
    explicit OptionsWindow(QWidget *parent = nullptr);
    //! Destructor
    /*!
      The destructor will save the settings that the window has in each spot and then free up the resources that need to be freed.
     */
    ~OptionsWindow();

private slots:
    //! Item Clicked Slot
    /*!
      This will fire when a user clicks on the tree view items. It will auto focus and show the option if it iis out of view.
     */
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    //! OptionsWindow Closed slot
    /*!
      The window has been requested to close. This will update Code Specific settings such as updating language specific keywords to the new colors.
     */
    void on_OptionsWindow_rejected();

private:
    Ui::OptionsWindow *ui;

    QVBoxLayout* textSettingsLayout; /*!< Vertical Layout of the Text Editor settings. */
    QVBoxLayout* codeSettingsLayout; /*!< Vertical Layout of the Code Editor settings. */

    QFontComboBox* fontFamilyComboBox; /*!< Default font family when writing text documents. */
    QSpinBox* fontSizeSpinbox; /*!< Default Font size when writing text documents. */
    QSpinBox* textTabLengthSpinbox; /*!< Default length of a tab when writing text documents. */

    QColorDialog* javaColorPicker; /*!< Default color for Java keywords. */
    QColorDialog* cppColorPicker; /*!< Default color for C++ keywords. */
    QColorDialog* pythonColorPicker; /*!< Default color for Python keywords. */

    QSpinBox* codeTabLengthSpinBox; /*!< Default length of a tab when writing code. */

    SettingsManager* settings; /*!< Reference to the SettingsManager singleton class. */
};

#endif // OPTIONSWINDOW_H
